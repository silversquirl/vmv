#ifndef __VMV_SOUND_IO_H__
#define __VMV_SOUND_IO_H__

#include <soundio/soundio.h>


struct soundinfo {
  struct SoundIo *soundio;
  struct SoundIoDevice *device;
  struct SoundIoInStream *instream;
};

int init_input(struct soundinfo *out_soundinfo);
void destroy_input(struct soundinfo *info);

#endif
