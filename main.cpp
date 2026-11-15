#include "main.h"

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
    SetConsoleTitleA("PBomb");
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    INPUT_RECORD inputRecord;
    DWORD events;
    while (true)
    {
        printf("\x1b[2J\x1b[H");
        printf("PBomb %s 开始菜单\n", VERSION.c_str());
        printf("新游戏\n");
        printf("退出\n");
        while (true)
        {
            ReadConsoleInput(hIn, &inputRecord, 1, &events);
            if (inputRecord.EventType == MOUSE_EVENT)
            {
                MOUSE_EVENT_RECORD option = inputRecord.Event.MouseEvent;
                if (option.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && option.dwEventFlags == 0 && option.dwMousePosition.X >= 0 && option.dwMousePosition.X <= 10 && option.dwMousePosition.Y == 1)
                {
                    NewGame();
                    break;
                }
                else if (option.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && option.dwEventFlags == 0 && option.dwMousePosition.X >= 0 && option.dwMousePosition.X <= 10 && option.dwMousePosition.Y == 2)
                {
                    printf("\x1b[?1049l");
                    return 0;
                }
            }
        }
    }
}