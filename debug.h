#ifndef __VISUALISER_DEBUG_H__
#define __VISUALISER_DEBUG_H__

#ifdef DEBUG
#undef DEBUG
#define DEBUG(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#endif
