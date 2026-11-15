#pragma once

#include <windows.h>
#include <cstdio>
#include <optional>
#include <vector>
#include <random>
#include <iostream>
#include <limits>

enum class Difficulty;

int GetConsoleHeight();
void cleanInputBuffer();

void NewGame();
