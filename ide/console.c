#include "console.h"

#include <stdlib.h>
#include <string.h>

#define CONSOLE_START_CAPACITY 4096

static int console_ensure_capacity(ConsoleBuffer* console, int needed) {
    char* new_text;
    int new_capacity;

    if (needed <= console->capacity) {
        return 1;
    }

    new_capacity = console->capacity;

    while (new_capacity < needed) {
        new_capacity *= 2;
    }

    new_text = (char*)realloc(console->text, (size_t)new_capacity);

    if (new_text == NULL) {
        return 0;
    }

    console->text = new_text;
    console->capacity = new_capacity;

    return 1;
}

void console_init(ConsoleBuffer* console) {
    console->capacity = CONSOLE_START_CAPACITY;
    console->length = 0;
    console->scroll_y = 0;
    console->text = (char*)malloc((size_t)console->capacity);

    if (console->text != NULL) {
        console->text[0] = '\0';
    } else {
        console->capacity = 0;
    }
}

void console_free(ConsoleBuffer* console) {
    if (console == NULL) {
        return;
    }

    free(console->text);
    console->text = NULL;
    console->length = 0;
    console->capacity = 0;
}

void console_clear(ConsoleBuffer* console) {
    if (console == NULL || console->text == NULL) {
        return;
    }

    console->scroll_y = 0;
    console->length = 0;
    console->text[0] = '\0';
}

int console_append(ConsoleBuffer* console, const char* text) {
    int text_len;

    if (console == NULL || console->text == NULL || text == NULL) {
        return 0;
    }

    text_len = (int)strlen(text);

    if (!console_ensure_capacity(console, console->length + text_len + 1)) {
        return 0;
    }

    memcpy(console->text + console->length, text, (size_t)text_len + 1);
    console->length += text_len;

    return 1;
}

void console_paint(HDC dc, RECT rect, ConsoleBuffer* console) {
    int x;
    int y;
    int line_start;
    int i;
    int line_height;
    const char* text;
    HRGN old_clip;
    HRGN console_clip;

    if (console == NULL || console->text == NULL) {
        return;
    }

    old_clip = CreateRectRgn(0, 0, 0, 0);
    GetClipRgn(dc, old_clip);

    console_clip = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
    SelectClipRgn(dc, console_clip);

    text = console->text;
    x = rect.left + 12;
    y = rect.top + 12 - console->scroll_y;
    line_height = 18;
    line_start = 0;

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(210, 210, 215));

    for (i = 0; i <= console->length; i++) {
        if (text[i] == '\n' || text[i] == '\0') {
            int line_len = i - line_start;

            TextOutA(dc, x, y, text + line_start, line_len);

            y += line_height;
            line_start = i + 1;

            if (y > rect.bottom) {
                break;
            }
        }
    }

    SelectClipRgn(dc, old_clip);
    DeleteObject(console_clip);
    DeleteObject(old_clip);
}

void console_scroll(ConsoleBuffer* console, int amount) {
    if (console == NULL) {
        return;
    }

    console->scroll_y += amount;

    if (console->scroll_y < 0) {
        console->scroll_y = 0;
    }
}