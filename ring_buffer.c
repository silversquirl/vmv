#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ring_buffer.h"
#include "debug.h"

void rb_write(struct ring_buffer *rb, void *data, ssize_t len) {
  if (len < 0) len = strlen(data);
  while (len) {
    size_t max = rb->end - rb->write;
    size_t n = max < len ? max : len;
    memcpy(rb->write, data, n);
    rb->write += n;
    data += n;
    len -= n;
    // Wrap
    if (rb->write >= rb->end) rb->write = rb->start + (rb->write - rb->end);
  }
}

void rb_read(struct ring_buffer *rb, void *data, ssize_t len) {
  if (len < 0) len = strlen(data);
  while (len) {
    size_t max = rb->end - rb->read;
    size_t n = max < len ? max : len;
    memcpy(data, rb->read, n);
    rb->read += n;
    data += n;
    len -= n;
    // Wrap
    if (rb->read >= rb->end) rb->read = rb->start + (rb->read - rb->end);
  }
}

int rb_init(struct ring_buffer *rb, size_t len) {
  if (!(rb->start = malloc(len))) return -1;
  rb->read = rb->start;
  rb->write = rb->start;
  rb->end = rb->start + len;
  return 0;
}

