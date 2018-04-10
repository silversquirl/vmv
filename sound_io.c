#include "sound_io.h"
#include "debug.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void read_callback(struct SoundIoInStream *instream, int frame_count_min, int frame_count_max) {
  // We need to read at least frame_count_min frames, or else they'll be dropped
  // However, we need to read frame_count_max frames to make sure audio enters at the right rate
  int frames_left = frame_count_max;
  struct SoundIoChannelArea *areas;
  while (frames_left) {
    int frame_count = frames_left;
    if (soundio_instream_begin_read(instream, &areas, &frame_count))
      return;

    if (!frame_count)
      return;
    
    if (!areas) {
      int *nul = {0};
      for(int i = 0; i < frame_count * instream->bytes_per_frame; i++) {
        write(*(int *)instream->userdata, nul, 1);
      }
    } else {
      for (int frame = 0; frame < frame_count; frame++) {
        for (int ch = 0; ch < instream->layout.channel_count; ch++) {
          write(*(int *)instream->userdata, areas[ch].ptr, instream->bytes_per_sample);
          areas[ch].ptr += areas[ch].step;
        }
      }
    }
    if (soundio_instream_end_read(instream))
      return;

    frames_left -= frame_count;
  }
}

bool startsWith(const char *str, const char *pre) {
  size_t lenpre = strlen(pre), lenstr = strlen(str);
  return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

bool endsWith(const char *str, const char *post) {
  size_t postoffset = strlen(str) - strlen(post);
  return postoffset < 0 ? false : strncmp(post, str + postoffset, strlen(post)) == 0;
}

struct SoundIoDevice *find_device(struct SoundIo *soundio) {
  int output_id = soundio_default_output_device_index(soundio);
  struct SoundIoDevice *output = soundio_get_output_device(soundio, output_id);

  int count = soundio_input_device_count(soundio);
  for (int i = 0; i < count; i++) {
    struct SoundIoDevice *device = soundio_get_input_device(soundio, i);

    if (startsWith(device->id, output->id)) {
      // Default output monitor device
      soundio_device_unref(output);
      return device;
    }

    soundio_device_unref(device);
  }

  for (int i = 0; i < count; i++) {
    struct SoundIoDevice *device = soundio_get_input_device(soundio, i);

    if (endsWith(device->id, "monitor")) {
      // Some kind of monitor device
      soundio_device_unref(output);
      return device;
    }

    soundio_device_unref(device);
  }
  soundio_device_unref(output);

  // Just use default input as a last resort
  int input_id = soundio_default_input_device_index(soundio);
  return soundio_get_input_device(soundio, input_id);
}

int init_input(int pipe_write, struct soundinfo *out_soundinfo) {
  struct SoundIo *soundio = soundio_create();
  if (!soundio)
    return -1;

  if (soundio_connect(soundio))
    return -1;

  soundio_flush_events(soundio);

  struct SoundIoDevice *device = find_device(soundio);

  soundio_device_sort_channel_layouts(device);
  if (!soundio_device_supports_sample_rate(device, 44100))
    return -1;

  if (!soundio_device_supports_format(device, SoundIoFormatS16LE))
    return -1;

  struct SoundIoInStream *instream = soundio_instream_create(device);
  instream->format = SoundIoFormatS16LE;
  instream->sample_rate = 44100;
  instream->read_callback = read_callback;

  int *pipe_write_mem = malloc(sizeof(pipe_write));
  *pipe_write_mem = pipe_write;

  instream->userdata = pipe_write_mem;
  
  if (soundio_instream_open(instream))
    return -1;

  if (soundio_instream_start(instream))
    return -1;

  struct soundinfo sinfo = {
    .soundio = soundio,
    .device = device,
    .instream = instream,
  };
  
  *out_soundinfo = sinfo;

  return 0;
}

void destroy_input(struct soundinfo *info) {
  free(info->instream->userdata);
  soundio_instream_destroy(info->instream);
  soundio_device_unref(info->device);
  soundio_destroy(info->soundio);
}

