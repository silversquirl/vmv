#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fftw3.h>
#include "audio.h"

#define CHUNK_SIZE 1024
#define FFT_SIZE (CHUNK_SIZE / 2 + 1)
#define SCALE_RATIO 0.2 // Weight put on new maximum
#define NBAR 10

size_t read_chunk(int16_t *buf, size_t nmemb, FILE *f) {
  size_t n = fread(buf, sizeof *buf, nmemb, f);
  int8_t *tmp = (int8_t *)buf;
  for (size_t i = 0; i < n; ++i) {
    int8_t *data = tmp + i * sizeof *buf;
    int16_t x = data[0] | data[1] << 8;
    buf[i] = x;
  }
  return n;
}

struct buffer bars;

// Audio processing state
unsigned scale;
FILE *src;
int16_t abuf[CHUNK_SIZE];
double *in;
complex *out;
fftw_plan p;

static inline void bar_calc(void) {
  double idx_coef = (bars.len - 1) / log(FFT_SIZE - 1);
  double x = 0;
  unsigned max = 0;
  for (size_t i = 0, old = 0, n = 0; i <= FFT_SIZE; ++i, ++n) {
    size_t cur = idx_coef * log1p(i);
    if (old != cur) {
      x /= n;
      bars.buf[cur] = x;
      if (x > max) max = x;

      x = (bars.buf[cur] - bars.buf[old]) / (double)(cur - old);
      for (size_t j = old + 1; j < cur; ++j)
        bars.buf[j] = bars.buf[j - 1] + x;

      x = 0;
      n = 0;
      old = cur;
    } else {
      x += cabs(out[i]);
    }
  }
  
  scale = max * SCALE_RATIO + scale * (1 - SCALE_RATIO);
  double val_coef = (double)BAR_MAX / scale;

  for (size_t i = 0; i < bars.len; ++i) {
    bars.buf[i] *= val_coef;
    printf("%u\n", scale);
  }
}

void process_audio(void) {
  size_t n = read_chunk(abuf, CHUNK_SIZE, src);
  for (size_t i = 0; i < n; ++i) {
    in[i] = abuf[i];
  }
  fftw_execute(p);
  bar_calc();
}

void audio_init(FILE *source_file) {
  src = source_file;

  scale = INT16_MAX;

  in = fftw_malloc(CHUNK_SIZE * sizeof *in);
  out = fftw_alloc_complex(FFT_SIZE);

  printf("Initialising FFT plan... ");
  fflush(stdout);
  p = fftw_plan_dft_r2c_1d(CHUNK_SIZE, in, out, FFTW_MEASURE);
  puts("Done");

  bars.buf = calloc(NBAR, sizeof *bars.buf);
  bars.len = NBAR;
}

