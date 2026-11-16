#pragma once

#include <windows.h>
#include <cstdio>
#include <optional>
#include <vector>
#include <random>
#include <iostream>
#include <limits>

namespace Game
{
	enum class Difficulty;

	int GetConsoleHeight();
	void cleanInputBuffer();

	void NewGame();
}
