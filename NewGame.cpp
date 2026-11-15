#include "NewGame.h"

enum class Difficulty
{
    Easy,
    Medium,
    Hard,
    Expert
};

constexpr int xDiff[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
constexpr int yDiff[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

std::optional <Difficulty> difficulty;
std::vector <std::vector <int>> bombs;
// bombs[i][j]:
// 0: no bomb & haven't opened
// 1: have bomb & haven't opened
// 2: no bomb & have opened
// 3: no bomb & flaged
// 4: have bomb & flaged
// 5: no bomb & marked
// 6: have bomb & marked
int squareSize, bombCnt;

int GetConsoleHeight()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut, &csbi))
    {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return -1;
}

void cleanInputBuffer()
{
    std::cin.clear();
    while (std::cin.rdbuf()->in_avail() > 0)
    {
        std::cin.ignore(1);
    }
}

void NewGame()
{
    difficulty.reset();
    bombs.clear();
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
            if (diffOption.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && diffOption.dwEventFlags == 0 && diffOption.dwMousePosition.X >= 0 && diffOption.dwMousePosition.X <= 20)
            {
                switch (diffOption.dwMousePosition.Y)
                {
                    case 1:
                        squareSize = 8;
                        bombCnt = 10;
                        difficulty = Difficulty::Easy;
                        break;
                    case 2:
                        squareSize = 16;
                        bombCnt = 40;
                        difficulty = Difficulty::Medium;
                        break;
                    case 3:
                        squareSize = 24;
                        bombCnt = 99;
                        difficulty = Difficulty::Hard;
                        break;
                    case 4:
                        squareSize = 32;
                        bombCnt = 200;
                        difficulty = Difficulty::Expert;
                        break;
                    default:
                        difficulty.reset();
                        break;
                }
                if (difficulty.has_value())
                {
                    break;
                }
            }
        }
    }
    bombs.resize(squareSize);
    for (int i = 0; i < squareSize; i++)
    {
        bombs[i].resize(squareSize);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> bombDis(0, squareSize-1);
    int bombX, bombY;
    for (int i = 1; i <= bombCnt; i++)
    {
        do 
        {
            bombX = bombDis(gen);
            bombY = bombDis(gen);
        } 
        while (bombs[bombX][bombY] == 1 || ((bombX == 0 || bombX == squareSize-1) && (bombY == 0 || bombY == squareSize-1)));
        bombs[bombX][bombY] = 1;
    }
    while (true)
    {
        printf("\x1b[2J\x1b[H");
        int consoleBottom = GetConsoleHeight();
        while (squareSize + 3 > consoleBottom)
        {
            printf("终端过小，无法容纳地图。\n拉大终端后按下 Enter 重试...");
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            consoleBottom = GetConsoleHeight();
            printf("\x1b[2J\x1b[H");
        }
        int openedCnt = 0;
        for (int i = 0; i < squareSize; i++)
        {
            for (int j = 0; j < squareSize; j++)
            {
                switch (bombs[i][j])
                {
                case 0:
                case 1:
                    printf(".");
                    break;
                case 2:
                {
                    int bombsAroundCnt = 0;
                    for (int k = 0; k < 8; k++)
                    {
                        int kWayI = i + xDiff[k], kWayJ = j + yDiff[k];
                        if (kWayI >= 0 && kWayI < squareSize && kWayJ >= 0 && kWayJ < squareSize)
                        {
                            int bombAtKWay = bombs[kWayI][kWayJ];
                            bombsAroundCnt += (bombAtKWay == 1 || bombAtKWay == 4 || bombAtKWay == 6);
                        }
                    }
                    printf("%d", bombsAroundCnt);
                    if (bombsAroundCnt == 0)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            int kWayI = i + xDiff[k], kWayJ = j + yDiff[k];
                            if (kWayI >= 0 && kWayI < squareSize && kWayJ >= 0 && kWayJ < squareSize)
                            {
                                bombs[kWayI][kWayJ] = 2;
                            }
                        }
                    }
                    openedCnt++;
                    break;
                }
                case 3:
                case 4:
                    printf("*");
                    openedCnt++;
                    break;
                case 5:
                case 6:
                    printf("?");
                    break;
                }
            }
            printf("\n");
        }
        if (openedCnt == squareSize * squareSize)
        {
            printf("翻开所有区块。游戏结束。\n");
            cleanInputBuffer();
            printf("按下 Enter 回到开始页面...");
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            return;
        }
        if (consoleBottom == -1)
        {
            printf("\x1b[47;30m");
            printf("1: 标记为雷  2: 可能为雷  3: 翻开\x1b[K\n");
            printf(".: 未翻开  [Number]: 雷数  *: 标记为雷  ?: 可能为雷\x1b[K\n");
            printf("\x1b[0m");
        }
        else
        {
            printf("\x1b[%d;%dH", consoleBottom - 1, 1);
            printf("\x1b[47;30m");
            printf("1: 标记为雷  2: 可能为雷  3: 翻开\x1b[K\n");
            printf(".: 未翻开  [Number]: 雷数  *: 标记为雷  ?: 可能为雷\x1b[K");
            printf("\x1b[%d;%dH", squareSize + 1, 1);
            printf("\x1b[0m");
        }
        printf("\x1b[%d;%dH\33[2K\r", squareSize + 1, 1);
        printf("操作 [1] 标记为雷  [2] 可能为雷  [3] 翻开: ");
        std::string op;
        std::cin >> op;
        while (op != "1" && op != "2" && op != "3")
        {
            printf("\x1b[%d;%dH\33[2K\r", squareSize + 1, 1);
            printf("对不起，请重试: ");
            std::cin >> op;
        }
        if (consoleBottom != -1)
        {
            printf("\x1b[%d;%dH", consoleBottom - 1, 1);
            printf("\33[2K\r\x1b[47;30mEscape: 取消操作\x1b[K");
            printf("\x1b[%d;%dH", squareSize + 1, 1);
            printf("\x1b[0m");
        }
        printf("\x1b[%d;%dH\33[2K\r", squareSize + 1, 1);
        printf("轻点要操作的区块以继续 (按下 Escape 以取消)...");
        INPUT_RECORD oprationPosition;
        while (true)
        {
            ReadConsoleInput(hIn, &oprationPosition, 1, &events);
            if (oprationPosition.EventType == KEY_EVENT && oprationPosition.Event.KeyEvent.bKeyDown && oprationPosition.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
            {
                break;
            }
            if (oprationPosition.EventType == MOUSE_EVENT)
            {
                MOUSE_EVENT_RECORD oprationPos = oprationPosition.Event.MouseEvent;
                if (oprationPos.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && oprationPos.dwEventFlags == 0)
                {
                    int posX = oprationPos.dwMousePosition.Y;
                    int posY = oprationPos.dwMousePosition.X;
                    if (posX >= 0 && posX < squareSize && posY >= 0 && posY < squareSize)
                    {
                        if (op == "1")
                        {
                            int bombAtPos = bombs[posX][posY];
                            switch (bombAtPos)
                            {
                            case 0:
                            case 2:
                            case 3:
                            case 5:
                                bombs[posX][posY] = 3;
                                break;
                            case 1:
                            case 4:
                            case 6:
                                bombs[posX][posY] = 4;
                                break;
                            }
                        }
                        else if (op == "2")
                        {
                            int bombAtPos = bombs[posX][posY];
                            switch (bombAtPos)
                            {
                            case 0:
                            case 2:
                            case 3:
                            case 5:
                                bombs[posX][posY] = 5;
                                break;
                            case 1:
                            case 4:
                            case 6:
                                bombs[posX][posY] = 6;
                                break;
                            }
                        }
                        else if (op == "3")
                        {
                            int bombAtPos = bombs[posX][posY];
                            switch (bombAtPos)
                            {
                            case 0:
                            case 2:
                            case 3:
                            case 5:
                                bombs[posX][posY] = 2;
                                break;
                            case 1:
                            case 4:
                            case 6:
                            {
                                printf("\x1b[2J\x1b[H");
                                for (int i = 0; i < squareSize; i++)
                                {
                                    for (int j = 0; j < squareSize; j++)
                                    {
                                        if (i == posX && j == posY)
                                        {
                                            printf("X");
                                        }
                                        else
                                        {
                                            int bombAtIJ = bombs[i][j];
                                            switch (bombAtIJ)
                                            {
                                            case 0:
                                            case 2:
                                            case 3:
                                            case 5:
                                                printf(".");
                                                break;
                                            case 1:
                                            case 4:
                                            case 6:
                                                printf("*");
                                                break;
                                            }
                                        }
                                    }
                                    printf("\n");
                                }
                                printf("踩中地雷。游戏结束。地雷位置: (%d, %d)。\n", posX+1, posY+1);
                                cleanInputBuffer();
                                printf("按下 Enter 回到开始页面...");
                                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                                return;
                            }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}
