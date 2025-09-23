#ifndef FILE_OPTIONS
#define FILE_OPTIONS

typedef struct {
  int (*open)(const char *path, int flags);
  int (*close)(int file_descriptor);
  int (*read)(int file_descriptor, void *buffer, int buffer_size);
  int (*write)(int file_descriptor, const void *buffer, int buffer_size);
  int (*ioctl)(int file_descriptor, int command, void *args);
} file_options_t;

#endif