#ifndef __VMV_AUDIO_H__
#define __VMV_AUDIO_H__

#include <stdio.h>

#define BAR_MAX 1024

extern struct buffer {
  size_t len;
  unsigned *buf;
} bars;

void process_audio(void);
void audio_init(FILE *source_file);

#endif
