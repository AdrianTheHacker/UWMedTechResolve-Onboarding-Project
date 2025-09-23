#ifndef FILE_OPTIONS
#define FILE_OPTIONS

typedef struct {
  int (*open)();
  int (*close)();
  int (*read)(void *buffer, int buffer_size);
  int (*write)(const void *buffer, int buffer_size);
  int (*ioctl)(int command, void *args);
} file_options_t;

#endif