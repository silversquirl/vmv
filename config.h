// A program that sends raw 16 bit little endian 44.1kHz audio to stdout
char * const audio_producer[] = { "arecord", "-f", "cd", "-t", "raw", NULL };
