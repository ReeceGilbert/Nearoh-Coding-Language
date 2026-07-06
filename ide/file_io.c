#include "file_io.h"
#include <errno.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

int file_load_into_buffer(const char* path, TextBuffer* buffer) {
    FILE* file;
    long size;
    char* text;
    size_t read_count;

    if (path == NULL || buffer == NULL) {
        return 0;
    }

    file = fopen(path, "rb");

    if (file == NULL) {
        return 0;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return 0;
    }

    size = ftell(file);

    if (size < 0) {
        fclose(file);
        return 0;
    }

    rewind(file);

    text = (char*)malloc((size_t)size + 1);

    if (text == NULL) {
        fclose(file);
        return 0;
    }

    read_count = fread(text, 1, (size_t)size, file);
    fclose(file);

    if (read_count != (size_t)size) {
        free(text);
        return 0;
    }

    text[size] = '\0';

    buffer->length = 0;
    buffer->cursor = 0;

    if (buffer->data != NULL) {
        buffer->data[0] = '\0';
    }

    if (!buffer_insert_text(buffer, text)) {
        free(text);
        return 0;
    }

    free(text);
    return 1;
}

int file_save_from_buffer(const char* path, TextBuffer* buffer) {
    FILE* file;
    size_t written;

    if (path == NULL || buffer == NULL || buffer->data == NULL) {
        printf("save failed: bad path or buffer\n");
        return 0;
    }

    file = fopen(path, "wb");

    if (file == NULL) {
        printf("save failed: could not open '%s': %s\n", path, strerror(errno));
        return 0;
    }

    written = fwrite(buffer->data, 1, (size_t)buffer->length, file);
    fclose(file);

    if (written != (size_t)buffer->length) {
        printf("save failed: wrote %zu of %d bytes\n", written, buffer->length);
        return 0;
    }

    return 1;
}