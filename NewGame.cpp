#include "NewGame.h"

enum class Difficulty
{
    Easy,
    Medium,
    Hard,
    Expert
};

void NewGame()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD events;
    printf("\x1b[2J\x1b[H");
    printf("难度...\n");
    printf("毫无压力 - 8x8 10*\n");
    printf("一般般 - 16x16 40*\n");
    printf("小心翼翼 - 24x24 99*\n");
    printf("无人能敌 - 32x32 200*\n");
    INPUT_RECORD difficultyOption;
    while (true)
    {
        ReadConsoleInput(hIn, &difficultyOption, 1, &events);
        if (difficultyOption.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD diffOption = difficultyOption.Event.MouseEvent;
            if (diffOption.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && diffOption.dwEventFlags == 0)
            {
                switch (diffOption.dwMousePosition.Y)
                {
                    case 1:
                        // Handle "毫无压力" selection
                        break;
                    case 2:
                        // Handle "一般般" selection
                        break;
                    case 3:
                        // Handle "小心翼翼" selection
                        break;
                    case 4:
                        // Handle "无人能敌" selection
                        break;
                    default:
                        break;
                }
                break;
            }
        }
    }
}