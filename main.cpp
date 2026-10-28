#include <Windows.h>
#include <cstdio>

bool EnableVTMode()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return false;
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return false;
    }
    return true;
}

int main()
{
    if (!EnableVTMode())
    {
        return 1;
    }
    printf("\x1b[?1049h");
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hIn, &mode);
    mode |= ENABLE_MOUSE_INPUT;
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hIn, mode);
    INPUT_RECORD inputRecord;
    DWORD events;
    while (true)
    {
        ReadConsoleInput(hIn, &inputRecord, 1, &events);
        if (inputRecord.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
        }
    }
    printf("\x1b[?1049l");
    return 0;
}