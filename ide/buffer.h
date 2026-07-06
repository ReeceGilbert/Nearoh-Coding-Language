#ifndef NEAROH_IDE_BUFFER_H
#define NEAROH_IDE_BUFFER_H

typedef struct {
    char* data;
    int length;
    int capacity;

    int cursor;
} TextBuffer;

int buffer_line_count(TextBuffer* buffer);

void buffer_init(TextBuffer* buffer);
void buffer_free(TextBuffer* buffer);

void buffer_move_up(TextBuffer* buffer);
void buffer_move_down(TextBuffer* buffer);

int buffer_insert_char(TextBuffer* buffer, char c);
int buffer_insert_text(TextBuffer* buffer, const char* text);

void buffer_backspace(TextBuffer* buffer);
void buffer_delete(TextBuffer* buffer);

void buffer_move_left(TextBuffer* buffer);
void buffer_move_right(TextBuffer* buffer);

const char* buffer_text(const TextBuffer* buffer);

#endif