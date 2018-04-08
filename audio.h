#ifndef __VISUALISER_AUDIO_H__
#define __VISUALISER_AUDIO_H__

#define BAR_MAX 1024

extern struct buffer {
  size_t len;
  unsigned *buf;
} bars;

void process_audio(void);
void audio_init(void);

#endif
