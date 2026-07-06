#include "editor.h"

#include <string.h>

void editor_view_init(EditorView* view) {
    view->x = 20;
    view->y = 62;
    view->line_height = 20;
    view->char_width = 8;
    view->scroll_y = 0;
}

static void draw_cursor(HDC dc, int x, int y, int line_height) {
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(240, 240, 240));
    HPEN old_pen = SelectObject(dc, pen);

    MoveToEx(dc, x, y, NULL);
    LineTo(dc, x, y + line_height);

    SelectObject(dc, old_pen);
    DeleteObject(pen);
}

void editor_paint(HDC dc, RECT rect, TextBuffer* buffer, EditorView* view) {
    const char* text;
    int i;
    int line_start;
    int line;
    int col;
    int cursor_line;
    int cursor_col;
    int visible_y;

    HRGN old_clip;
    HRGN editor_clip;

    if (buffer == NULL || buffer->data == NULL) {
        return;
    }

    old_clip = CreateRectRgn(0, 0, 0, 0);
    GetClipRgn(dc, old_clip);

    editor_clip = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
    SelectClipRgn(dc, editor_clip);

    text = buffer->data;

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(235, 235, 240));

    line_start = 0;
    line = 0;
    col = 0;
    cursor_line = 0;
    cursor_col = 0;

    for (i = 0; i <= buffer->length; i++) {
        if (i == buffer->cursor) {
            cursor_line = line;
            cursor_col = col;
        }

        if (text[i] == '\n' || text[i] == '\0') {
            int line_len = i - line_start;
            visible_y = view->y + (line * view->line_height) - view->scroll_y;

            if (visible_y + view->line_height >= rect.top && visible_y <= rect.bottom) {
                TextOutA(dc, view->x, visible_y, text + line_start, line_len);
            }

            line++;
            col = 0;
            line_start = i + 1;
        } else {
            col++;
        }
    }

    {
        SIZE cursor_size;
        int cursor_line_start = 0;
        int scan_line = 0;
        int j;

        for (j = 0; j < buffer->cursor; j++) {
            if (text[j] == '\n') {
                scan_line++;
                cursor_line_start = j + 1;
            }
        }

        GetTextExtentPoint32A(
            dc,
            text + cursor_line_start,
            buffer->cursor - cursor_line_start,
            &cursor_size
        );

        int cursor_x = view->x + cursor_size.cx;
        int cursor_y = view->y + cursor_line * view->line_height - view->scroll_y;

        if (cursor_y >= rect.top && cursor_y <= rect.bottom) {
            draw_cursor(dc, cursor_x, cursor_y, view->line_height);
        }

        SelectClipRgn(dc, old_clip);
        DeleteObject(editor_clip);
        DeleteObject(old_clip);
    }
}

void editor_ensure_cursor_visible(TextBuffer* buffer, EditorView* view, RECT editor_rect) {
    const char* text;
    int i;
    int line = 0;
    int cursor_y;
    int editor_top;
    int editor_bottom;

    if (buffer == NULL || buffer->data == NULL || view == NULL) {
        return;
    }

    text = buffer->data;

    for (i = 0; i < buffer->cursor && i < buffer->length; i++) {
        if (text[i] == '\n') {
            line++;
        }
    }

    cursor_y = view->y + (line * view->line_height) - view->scroll_y;

    editor_top = editor_rect.top;
    editor_bottom = editor_rect.bottom - view->line_height;

    if (cursor_y < editor_top) {
        view->scroll_y -= editor_top - cursor_y;

        if (view->scroll_y < 0) {
            view->scroll_y = 0;
        }
    }

    if (cursor_y > editor_bottom) {
        view->scroll_y += cursor_y - editor_bottom;
    }
}