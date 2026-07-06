#ifndef NEAROH_IDE_EDITOR_H
#define NEAROH_IDE_EDITOR_H

#include <windows.h>
#include "buffer.h"

typedef struct {
    int x;
    int y;
    int line_height;
    int char_width;
    int scroll_y;
} EditorView;

void editor_view_init(EditorView* view);
void editor_paint(HDC dc, RECT rect, TextBuffer* buffer, EditorView* view);
void editor_ensure_cursor_visible(TextBuffer* buffer, EditorView* view, RECT editor_rect);

#endif