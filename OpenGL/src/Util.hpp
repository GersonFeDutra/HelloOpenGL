#pragma once

#if defined(_WIN32)
#include <windows.h>
extern HANDLE _hConsole;
extern WORD _saved_attributes;
#define SET_CLI_GREEN() SetConsoleTextAttribute(_hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define SET_CLI_RED() SetConsoleTextAttribute(_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
#define RESET_CLI() SetConsoleTextAttribute(_hConsole, _saved_attributes);
#else
#define SET_CLI_RED() std::cout << "\033[32m";
#define RESET_CLI() std::cout << "\033[m";
#endif

#ifdef _MSC_VER
#define my_assert(E) if (!(E)) __debugbreak();
#else
#include <cassert>
#define my_assert(E) assert(E)
#endif

// for GCC/ Clang must define _DEBUG flags on build
#ifdef _DEBUG
#define GLCall(x) GLClearError(); \
    x; \
    my_assert(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

enum { ERR_GLEW_NOT_OK = 1 }; // Exit Error codes


void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
