#ifndef __VMV_RING_BUFFER_H__
#define __VMV_RING_BUFFER_H__

#include <stdlib.h>
#include <stdio.h>

struct ring_buffer {
  char *start, *read, *write, *end;
};

void rb_write(struct ring_buffer *rb, void *data, ssize_t len);
void rb_read(struct ring_buffer *rb, void *data, ssize_t len);
int rb_init(struct ring_buffer *rb, size_t len);

#endif
