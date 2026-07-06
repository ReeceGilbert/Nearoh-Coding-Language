#ifndef NEAROH_IDE_CONSOLE_H
#define NEAROH_IDE_CONSOLE_H

#include <windows.h>

typedef struct {
    char* text;
    int length;
    int capacity;
    int scroll_y;
} ConsoleBuffer;

void console_init(ConsoleBuffer* console);
void console_free(ConsoleBuffer* console);
void console_clear(ConsoleBuffer* console);
int console_append(ConsoleBuffer* console, const char* text);
void console_paint(HDC dc, RECT rect, ConsoleBuffer* console);
void console_scroll(ConsoleBuffer* console, int amount);

#endif