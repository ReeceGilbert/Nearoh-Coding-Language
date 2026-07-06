#include "buffer.h"

#include <stdlib.h>
#include <string.h>

#define BUFFER_START_CAPACITY 1024

static int buffer_ensure_capacity(TextBuffer* buffer, int needed) {
    char* new_data;
    int new_capacity;

    if (needed <= buffer->capacity) {
        return 1;
    }

    new_capacity = buffer->capacity;

    while (new_capacity < needed) {
        new_capacity *= 2;
    }

    new_data = (char*)realloc(buffer->data, (size_t)new_capacity);

    if (new_data == NULL) {
        return 0;
    }

    buffer->data = new_data;
    buffer->capacity = new_capacity;

    return 1;
}

void buffer_init(TextBuffer* buffer) {
    buffer->capacity = BUFFER_START_CAPACITY;
    buffer->length = 0;
    buffer->cursor = 0;

    buffer->data = (char*)malloc((size_t)buffer->capacity);

    if (buffer->data != NULL) {
        buffer->data[0] = '\0';
    } else {
        buffer->capacity = 0;
    }
}

void buffer_free(TextBuffer* buffer) {
    if (buffer == NULL) {
        return;
    }

    free(buffer->data);

    buffer->data = NULL;
    buffer->length = 0;
    buffer->capacity = 0;
    buffer->cursor = 0;
}

int buffer_insert_char(TextBuffer* buffer, char c) {
    if (buffer == NULL || buffer->data == NULL) {
        return 0;
    }

    if (!buffer_ensure_capacity(buffer, buffer->length + 2)) {
        return 0;
    }

    memmove(
        buffer->data + buffer->cursor + 1,
        buffer->data + buffer->cursor,
        (size_t)(buffer->length - buffer->cursor + 1)
    );

    buffer->data[buffer->cursor] = c;
    buffer->cursor++;
    buffer->length++;

    return 1;
}

int buffer_insert_text(TextBuffer* buffer, const char* text) {
    int i;

    if (buffer == NULL || text == NULL) {
        return 0;
    }

    for (i = 0; text[i] != '\0'; i++) {
        if (!buffer_insert_char(buffer, text[i])) {
            return 0;
        }
    }

    return 1;
}

void buffer_backspace(TextBuffer* buffer) {
    if (buffer == NULL || buffer->data == NULL) {
        return;
    }

    if (buffer->cursor <= 0) {
        return;
    }

    memmove(
        buffer->data + buffer->cursor - 1,
        buffer->data + buffer->cursor,
        (size_t)(buffer->length - buffer->cursor + 1)
    );

    buffer->cursor--;
    buffer->length--;
}

void buffer_delete(TextBuffer* buffer) {
    if (buffer == NULL || buffer->data == NULL) {
        return;
    }

    if (buffer->cursor >= buffer->length) {
        return;
    }

    memmove(
        buffer->data + buffer->cursor,
        buffer->data + buffer->cursor + 1,
        (size_t)(buffer->length - buffer->cursor)
    );

    buffer->length--;
}

void buffer_move_left(TextBuffer* buffer) {
    if (buffer != NULL && buffer->cursor > 0) {
        buffer->cursor--;
    }
}

void buffer_move_right(TextBuffer* buffer) {
    if (buffer != NULL && buffer->cursor < buffer->length) {
        buffer->cursor++;
    }
}

const char* buffer_text(const TextBuffer* buffer) {
    if (buffer == NULL || buffer->data == NULL) {
        return "";
    }

    return buffer->data;
}

static int buffer_line_start_from_cursor(TextBuffer* buffer) {
    int i = buffer->cursor;

    while (i > 0 && buffer->data[i - 1] != '\n') {
        i--;
    }

    return i;
}

static int buffer_line_end_from_pos(TextBuffer* buffer, int pos) {
    int i = pos;

    while (i < buffer->length && buffer->data[i] != '\n') {
        i++;
    }

    return i;
}

void buffer_move_up(TextBuffer* buffer) {
    int current_start;
    int current_col;
    int prev_end;
    int prev_start;
    int prev_len;

    if (buffer == NULL || buffer->data == NULL) {
        return;
    }

    current_start = buffer_line_start_from_cursor(buffer);

    if (current_start == 0) {
        return;
    }

    current_col = buffer->cursor - current_start;

    prev_end = current_start - 1;
    prev_start = prev_end;

    while (prev_start > 0 && buffer->data[prev_start - 1] != '\n') {
        prev_start--;
    }

    prev_len = prev_end - prev_start;

    if (current_col > prev_len) {
        current_col = prev_len;
    }

    buffer->cursor = prev_start + current_col;
}

void buffer_move_down(TextBuffer* buffer) {
    int current_start;
    int current_end;
    int current_col;
    int next_start;
    int next_end;
    int next_len;

    if (buffer == NULL || buffer->data == NULL) {
        return;
    }

    current_start = buffer_line_start_from_cursor(buffer);
    current_end = buffer_line_end_from_pos(buffer, current_start);

    if (current_end >= buffer->length) {
        return;
    }

    current_col = buffer->cursor - current_start;

    next_start = current_end + 1;
    next_end = buffer_line_end_from_pos(buffer, next_start);
    next_len = next_end - next_start;

    if (current_col > next_len) {
        current_col = next_len;
    }

    buffer->cursor = next_start + current_col;
}

int buffer_line_count(TextBuffer* buffer) {
    int count = 1;
    int i;

    if (buffer == NULL || buffer->data == NULL) {
        return 1;
    }

    for (i = 0; i < buffer->length; i++) {
        if (buffer->data[i] == '\n') {
            count++;
        }
    }

    return count;
}