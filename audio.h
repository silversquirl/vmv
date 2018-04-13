#ifndef __VMV_AUDIO_H__
#define __VMV_AUDIO_H__

#include <stdio.h>
#include "sound_io.h"

#define BAR_MAX 1024

extern struct buffer {
  size_t len;
  unsigned *buf;
} bars;

void process_audio(float delta);
int audio_init(struct soundinfo *sinfo);

#endif
