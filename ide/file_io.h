#ifndef NEAROH_IDE_FILE_IO_H
#define NEAROH_IDE_FILE_IO_H

#include "buffer.h"

int file_load_into_buffer(const char* path, TextBuffer* buffer);
int file_save_from_buffer(const char* path, TextBuffer* buffer);

#endif