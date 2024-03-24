#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "Memory.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <set>
#include <queue>
#include <algorithm>
#include <format>
using namespace std;

#define DEBUG_MODE

extern FILE* log_out;
extern HANDLE h_pipe;

void write_log(const char* format, ...);
void write_log(const wchar_t* format, ...);