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

    int gutter_width = 56;
    int text_x = rect.left + gutter_width + 10;
    int number_x = rect.left + 8;

    int saved_dc;
    HRGN editor_clip;

    if (buffer == NULL || buffer->data == NULL) {
        return;
    }

    saved_dc = SaveDC(dc);

    editor_clip = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
    SelectClipRgn(dc, editor_clip);

    text = buffer->data;

    SetBkMode(dc, TRANSPARENT);

    {
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(50, 50, 58));
        HPEN old_pen = SelectObject(dc, pen);

        MoveToEx(dc, rect.left + gutter_width, rect.top, NULL);
        LineTo(dc, rect.left + gutter_width, rect.bottom);

        SelectObject(dc, old_pen);
        DeleteObject(pen);
    }

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
            int display_line = line + 1;
            char number_text[32];

            visible_y = view->y + (line * view->line_height) - view->scroll_y;

            if (visible_y + view->line_height >= rect.top && visible_y <= rect.bottom) {
                wsprintfA(number_text, "%d", display_line);

                SetTextColor(dc, RGB(120, 120, 130));
                TextOutA(
                    dc,
                    number_x,
                    visible_y,
                    number_text,
                    lstrlenA(number_text)
                );

                SetTextColor(dc, RGB(235, 235, 240));
                TextOutA(
                    dc,
                    text_x,
                    visible_y,
                    text + line_start,
                    line_len
                );
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
        int cursor_x;
        int cursor_y;

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

        cursor_x = text_x + cursor_size.cx;
        cursor_y = view->y + cursor_line * view->line_height - view->scroll_y;

        if (cursor_y >= rect.top && cursor_y <= rect.bottom) {
            draw_cursor(dc, cursor_x, cursor_y, view->line_height);
        }
    }

    DeleteObject(editor_clip);
    RestoreDC(dc, saved_dc);
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

void editor_set_cursor_from_point(
    HDC dc,
    TextBuffer* buffer,
    EditorView* view,
    RECT editor_rect,
    int mouse_x,
    int mouse_y
) {
    const char* text;
    int gutter_width = 56;
    int text_x = editor_rect.left + gutter_width + 10;

    int target_line;
    int current_line;
    int i;
    int line_start;
    int line_end;
    int best_cursor;
    int best_distance;

    if (buffer == NULL || buffer->data == NULL || view == NULL) {
        return;
    }

    text = buffer->data;

    target_line = (mouse_y + view->scroll_y - view->y) / view->line_height;

    if (target_line < 0) {
        target_line = 0;
    }

    current_line = 0;
    line_start = 0;

    for (i = 0; i <= buffer->length; i++) {
        if (current_line == target_line) {
            break;
        }

        if (text[i] == '\n' || text[i] == '\0') {
            current_line++;
            line_start = i + 1;
        }
    }

    if (line_start > buffer->length) {
        buffer->cursor = buffer->length;
        return;
    }

    line_end = line_start;

    while (line_end < buffer->length &&
           text[line_end] != '\n' &&
           text[line_end] != '\0') {
        line_end++;
           }

    if (mouse_x <= text_x) {
        buffer->cursor = line_start;
        return;
    }

    best_cursor = line_start;
    best_distance = 2147483647;

    for (i = line_start; i <= line_end; i++) {
        SIZE size;
        int x;
        int distance;

        GetTextExtentPoint32A(
            dc,
            text + line_start,
            i - line_start,
            &size
        );

        x = text_x + size.cx;
        distance = mouse_x - x;

        if (distance < 0) {
            distance = -distance;
        }

        if (distance < best_distance) {
            best_distance = distance;
            best_cursor = i;
        }
    }

    buffer->cursor = best_cursor;
}