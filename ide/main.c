// ide/main.c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include "file_io.h"

#include "console.h"
#include "process.h"

#include "buffer.h"
#include "editor.h"

#define WINDOW_CLASS_NAME "NearohIDEWindowClass"
#define WINDOW_TITLE      "Nearoh IDE"

typedef enum {
    IDE_RUN_NORMAL,
    IDE_RUN_TOKENS,
    IDE_RUN_AST,
    IDE_RUN_DEBUG
} IdeRunMode;

typedef enum {
    TOOLBAR_BUTTON_NONE,
    TOOLBAR_BUTTON_RUN,
    TOOLBAR_BUTTON_TOKENS,
    TOOLBAR_BUTTON_AST,
    TOOLBAR_BUTTON_DEBUG
} ToolbarButton;

static TextBuffer editor_buffer;
static EditorView editor_view;
static ConsoleBuffer console_buffer;

static char repo_root[MAX_PATH];
static char nearoh_exe_path[MAX_PATH];

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

static RECT get_editor_rect(HWND hwnd);
static void clamp_editor_scroll(HWND hwnd);
static void editor_changed(HWND hwnd);
static void run_current_file(HWND hwnd, IdeRunMode mode);

static RECT toolbar_button_rect(int index);
static void draw_toolbar_button(HDC dc, int index, const char* text);
static ToolbarButton toolbar_hit_test(POINT p);

static char current_file_path[MAX_PATH];

static RECT get_console_rect(HWND hwnd) {
    RECT client_rect;
    RECT console_rect;

    GetClientRect(hwnd, &client_rect);

    console_rect = client_rect;
    console_rect.top = client_rect.bottom - 180;

    return console_rect;
}

static void set_repo_paths(void) {
    DWORD length;
    int i;
    int slash_count = 0;

    length = GetModuleFileNameA(NULL, repo_root, MAX_PATH);

    if (length == 0 || length >= MAX_PATH) {
        lstrcpyA(repo_root, ".");
        lstrcpyA(nearoh_exe_path, "cmake-build-debug\\nearoh.exe");
        return;
    }

    for (i = (int)length - 1; i >= 0; i--) {
        if (repo_root[i] == '\\' || repo_root[i] == '/') {
            slash_count++;

            if (slash_count == 2) {
                repo_root[i] = '\0';
                break;
            }
        }
    }

    lstrcpyA(nearoh_exe_path, repo_root);
    lstrcatA(nearoh_exe_path, "\\cmake-build-debug\\nearoh.exe");
}

static void run_current_file(HWND hwnd, IdeRunMode mode) {
    char command[2048];
    char output[65536];
    int exit_code;
    const char* mode_flag = "";
    const char* mode_name = "Run";

    if (!file_save_from_buffer(current_file_path, &editor_buffer)) {
        console_clear(&console_buffer);
        console_append(&console_buffer, "Save failed before run.\n");
        InvalidateRect(hwnd, NULL, TRUE);
        return;
    }

    switch (mode) {
        case IDE_RUN_NORMAL:
            mode_flag = "";
            mode_name = "Run";
            break;

        case IDE_RUN_TOKENS:
            mode_flag = "--tokens ";
            mode_name = "Tokens";
            break;

        case IDE_RUN_AST:
            mode_flag = "--ast ";
            mode_name = "AST";
            break;

        case IDE_RUN_DEBUG:
            mode_flag = "--debug ";
            mode_name = "Debug";
            break;
    }

    console_clear(&console_buffer);

    console_append(&console_buffer, "=== Nearoh ");
    console_append(&console_buffer, mode_name);
    console_append(&console_buffer, " ===\n\n");

    wsprintfA(
        command,
        "\"%s\" %s\"%s\"",
        nearoh_exe_path,
        mode_flag,
        current_file_path
    );

    console_append(&console_buffer, "> ");
    console_append(&console_buffer, command);
    console_append(&console_buffer, "\n\n");

    exit_code = process_run_capture(command, repo_root, output, sizeof(output));

    console_append(&console_buffer, output);
    console_append(&console_buffer, "\n");

    if (exit_code == 0) {
        console_append(&console_buffer, "[process exited successfully]\n");
    } else {
        char exit_text[128];
        wsprintfA(exit_text, "[process exited with code %d]\n", exit_code);
        console_append(&console_buffer, exit_text);
    }

    InvalidateRect(hwnd, NULL, TRUE);
}

static void set_default_file_path(void) {
    DWORD length;
    int i;

    length = GetModuleFileNameA(NULL, current_file_path, MAX_PATH);

    if (length == 0 || length >= MAX_PATH) {
        lstrcpyA(current_file_path, "test.nr");
        return;
    }

    for (i = (int)length - 1; i >= 0; i--) {
        if (current_file_path[i] == '\\' || current_file_path[i] == '/') {
            current_file_path[i + 1] = '\0';
            break;
        }
    }

    lstrcatA(current_file_path, "test.nr");
}

static void clamp_editor_scroll(HWND hwnd) {
    RECT editor_rect = get_editor_rect(hwnd);

    int total_lines = buffer_line_count(&editor_buffer);
    int content_height = total_lines * editor_view.line_height;
    int visible_height = editor_rect.bottom - editor_rect.top;

    int max_scroll = content_height - visible_height + 40;

    if (max_scroll < 0) {
        max_scroll = 0;
    }

    if (editor_view.scroll_y < 0) {
        editor_view.scroll_y = 0;
    }

    if (editor_view.scroll_y > max_scroll) {
        editor_view.scroll_y = max_scroll;
    }
}

static RECT get_editor_rect(HWND hwnd) {
    RECT client_rect;
    RECT console_rect;
    RECT editor_rect;

    GetClientRect(hwnd, &client_rect);

    console_rect = client_rect;
    console_rect.top = client_rect.bottom - 180;

    editor_rect = client_rect;
    editor_rect.top = 42;
    editor_rect.bottom = console_rect.top;

    return editor_rect;
}

static void editor_changed(HWND hwnd) {
    RECT editor_rect = get_editor_rect(hwnd);

    editor_ensure_cursor_visible(&editor_buffer, &editor_view, editor_rect);
    clamp_editor_scroll(hwnd);
    InvalidateRect(hwnd, NULL, TRUE);
}

static RECT toolbar_button_rect(int index) {
    RECT r;

    r.left = 120 + index * 92;
    r.top = 8;
    r.right = r.left + 82;
    r.bottom = 34;

    return r;
}

static void draw_toolbar_button(HDC dc, int index, const char* text) {
    RECT r;
    HBRUSH brush;
    HBRUSH old_brush;
    HPEN pen;
    HPEN old_pen;

    r = toolbar_button_rect(index);

    brush = CreateSolidBrush(RGB(48, 48, 56));
    old_brush = SelectObject(dc, brush);

    pen = CreatePen(PS_SOLID, 1, RGB(72, 72, 84));
    old_pen = SelectObject(dc, pen);

    Rectangle(dc, r.left, r.top, r.right, r.bottom);

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(235, 235, 240));
    DrawTextA(dc, text, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(dc, old_pen);
    DeleteObject(pen);

    SelectObject(dc, old_brush);
    DeleteObject(brush);
}

static ToolbarButton toolbar_hit_test(POINT p) {
    RECT r;

    r = toolbar_button_rect(0);
    if (PtInRect(&r, p)) {
        return TOOLBAR_BUTTON_RUN;
    }

    r = toolbar_button_rect(1);
    if (PtInRect(&r, p)) {
        return TOOLBAR_BUTTON_TOKENS;
    }

    r = toolbar_button_rect(2);
    if (PtInRect(&r, p)) {
        return TOOLBAR_BUTTON_AST;
    }

    r = toolbar_button_rect(3);
    if (PtInRect(&r, p)) {
        return TOOLBAR_BUTTON_DEBUG;
    }

    return TOOLBAR_BUTTON_NONE;
}

static void paint_window(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC dc;
    RECT client_rect;

    HBRUSH background_brush;
    HBRUSH top_bar_brush;
    HBRUSH console_brush;
    HBRUSH editor_brush;

    dc = BeginPaint(hwnd, &ps);

    GetClientRect(hwnd, &client_rect);

    background_brush = CreateSolidBrush(RGB(20, 20, 24));
    FillRect(dc, &client_rect, background_brush);
    DeleteObject(background_brush);

    RECT top_bar = client_rect;
    top_bar.bottom = 42;

    top_bar_brush = CreateSolidBrush(RGB(32, 32, 38));
    FillRect(dc, &top_bar, top_bar_brush);
    DeleteObject(top_bar_brush);

    RECT console_rect = client_rect;
    console_rect.top = client_rect.bottom - 180;

    console_brush = CreateSolidBrush(RGB(12, 12, 15));
    FillRect(dc, &console_rect, console_brush);
    DeleteObject(console_brush);

    RECT editor_rect = client_rect;
    editor_rect.top = 42;
    editor_rect.bottom = console_rect.top;

    editor_brush = CreateSolidBrush(RGB(24, 24, 30));
    FillRect(dc, &editor_rect, editor_brush);
    DeleteObject(editor_brush);

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(235, 235, 240));
    TextOutA(dc, 16, 13, "Nearoh IDE", 10);

    draw_toolbar_button(dc, 0, "Run");
    draw_toolbar_button(dc, 1, "Tokens");
    draw_toolbar_button(dc, 2, "AST");
    draw_toolbar_button(dc, 3, "Debug");

    editor_paint(dc, editor_rect, &editor_buffer, &editor_view);
    console_paint(dc, console_rect, &console_buffer);

    EndPaint(hwnd, &ps);
}

int WINAPI WinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    LPSTR cmd_line,
    int show_cmd
) {
    (void)prev_instance;
    (void)cmd_line;

    WNDCLASSA wc;
    HWND window;
    MSG msg;

    set_repo_paths();

    buffer_init(&editor_buffer);
    editor_view_init(&editor_view);

    console_init(&console_buffer);
    console_append(&console_buffer, "Nearoh IDE console ready.\nCtrl+R to run current file.\n");

    set_default_file_path();

    if (!file_load_into_buffer(current_file_path, &editor_buffer)) {
        buffer_insert_text(
            &editor_buffer,
            "// Nearoh IDE\n"
            "print(\"hello from Nearoh\")\n"
        );
    }

    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc = window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_IBEAM);
    wc.hbrBackground = NULL;

    if (!RegisterClassA(&wc)) {
        MessageBoxA(NULL, "Failed to register window class.", "Nearoh IDE Error", MB_ICONERROR);
        buffer_free(&editor_buffer);
        return 1;
    }

    window = CreateWindowExA(
        0,
        WINDOW_CLASS_NAME,
        WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1200,
        800,
        NULL,
        NULL,
        instance,
        NULL
    );

    if (window == NULL) {
        MessageBoxA(NULL, "Failed to create window.", "Nearoh IDE Error", MB_ICONERROR);
        buffer_free(&editor_buffer);
        return 1;
    }

    ShowWindow(window, show_cmd);
    UpdateWindow(window);

    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    buffer_free(&editor_buffer);
    console_free(&console_buffer);

    return (int)msg.wParam;
}

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_PAINT:
            paint_window(hwnd);
            return 0;

        case WM_SIZE:
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_CHAR:
            if (wparam >= 32 && wparam <= 126) {
                buffer_insert_char(&editor_buffer, (char)wparam);
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wparam == '\r') {
                buffer_insert_char(&editor_buffer, '\n');
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wparam == '\b') {
                buffer_backspace(&editor_buffer);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;

        case WM_KEYDOWN:
            if (wparam == VK_LEFT) {
                buffer_move_left(&editor_buffer);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            if (wparam == VK_RIGHT) {
                buffer_move_right(&editor_buffer);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            if (wparam == VK_DELETE) {
                buffer_delete(&editor_buffer);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            if (wparam == VK_UP) {
                buffer_move_up(&editor_buffer);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            if (wparam == VK_DOWN) {
                buffer_move_down(&editor_buffer);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            if (wparam == VK_PRIOR) { // Page Up
                editor_view.scroll_y -= 8 * editor_view.line_height;
                clamp_editor_scroll(hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            if (wparam == VK_NEXT) { // Page Down
                editor_view.scroll_y += 8 * editor_view.line_height;
                clamp_editor_scroll(hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            if ((GetKeyState(VK_CONTROL) & 0x8000) && wparam == 'S') {
                if (file_save_from_buffer(current_file_path, &editor_buffer)) {
                    MessageBoxA(hwnd, "Saved.", "Nearoh IDE", MB_OK);
                } else {
                    MessageBoxA(hwnd, "Save failed.", "Nearoh IDE Error", MB_ICONERROR);
                }

                return 0;
            }

            if ((GetKeyState(VK_CONTROL) & 0x8000) && wparam == 'O') {
                if (file_load_into_buffer(current_file_path, &editor_buffer)) {
                    editor_view.scroll_y = 0;
                    editor_changed(hwnd);
                } else {
                    MessageBoxA(hwnd, "Open failed.", "Nearoh IDE Error", MB_ICONERROR);
                }

                return 0;
            }

            if ((GetKeyState(VK_CONTROL) & 0x8000) && wparam == 'R') {
                run_current_file(hwnd, IDE_RUN_NORMAL);
                return 0;
            }

            if ((GetKeyState(VK_CONTROL) & 0x8000) && wparam == 'T') {
                run_current_file(hwnd, IDE_RUN_TOKENS);
                return 0;
            }

            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_MOUSEWHEEL: {
            POINT mouse;
            RECT console_rect;
            int delta;
            int lines;

            mouse.x = GET_X_LPARAM(lparam);
            mouse.y = GET_Y_LPARAM(lparam);
            ScreenToClient(hwnd, &mouse);

            console_rect = get_console_rect(hwnd);

            delta = GET_WHEEL_DELTA_WPARAM(wparam);
            lines = delta / WHEEL_DELTA;

            if (PtInRect(&console_rect, mouse)) {
                console_scroll(&console_buffer, -lines * 18 * 3);
            } else {
                editor_view.scroll_y -= lines * editor_view.line_height;
                clamp_editor_scroll(hwnd);
            }

            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            POINT mouse;
            ToolbarButton button;

            mouse.x = (short)LOWORD(lparam);
            mouse.y = (short)HIWORD(lparam);

            button = toolbar_hit_test(mouse);

            if (button != TOOLBAR_BUTTON_NONE) {
                switch (button) {
                    case TOOLBAR_BUTTON_RUN:
                        run_current_file(hwnd, IDE_RUN_NORMAL);
                        return 0;

                    case TOOLBAR_BUTTON_TOKENS:
                        run_current_file(hwnd, IDE_RUN_TOKENS);
                        return 0;

                    case TOOLBAR_BUTTON_AST:
                        run_current_file(hwnd, IDE_RUN_AST);
                        return 0;

                    case TOOLBAR_BUTTON_DEBUG:
                        run_current_file(hwnd, IDE_RUN_DEBUG);
                        return 0;

                    default:
                        return 0;
                }
            }

            {
                RECT editor_rect;

                editor_rect = get_editor_rect(hwnd);

                if (PtInRect(&editor_rect, mouse)) {
                    HDC dc;

                    dc = GetDC(hwnd);

                    editor_set_cursor_from_point(
                        dc,
                        &editor_buffer,
                        &editor_view,
                        editor_rect,
                        mouse.x,
                        mouse.y
                    );

                    ReleaseDC(hwnd, dc);

                    editor_changed(hwnd);
                    return 0;
                }
            }

            return 0;
        }

        case WM_SETCURSOR: {
            POINT mouse;
            RECT editor_rect;
            RECT console_rect;

            GetCursorPos(&mouse);
            ScreenToClient(hwnd, &mouse);

            editor_rect = get_editor_rect(hwnd);
            console_rect = get_console_rect(hwnd);

            if (toolbar_hit_test(mouse) != TOOLBAR_BUTTON_NONE) {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }

            if (PtInRect(&editor_rect, mouse)) {
                SetCursor(LoadCursor(NULL, IDC_IBEAM));
                return TRUE;
            }

            if (PtInRect(&console_rect, mouse)) {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }

            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return TRUE;
        }

        default:
            return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
}