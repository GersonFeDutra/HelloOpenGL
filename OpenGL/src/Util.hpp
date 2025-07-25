#pragma once

#include <cstdio>
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
extern HANDLE _hConsole;
extern WORD _saved_attributes;
#define SET_CLI_RED() SetConsoleTextAttribute(_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY)
#define SET_CLI_GREEN() SetConsoleTextAttribute(_hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define SET_CLI_YELLOW() SetConsoleTextAttribute(_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY)
#define RESET_CLI() SetConsoleTextAttribute(_hConsole, _saved_attributes)
#else
#define SET_CLI_RED() std::cerr << "\033[31m"
#define SET_CLI_GREEN() std::cerr << "\033[32m"
#define SET_CLI_YELLOW() std::cerr << "\033[33m"
#define RESET_CLI() std::cerr << "\033[m"
#endif

#ifdef _MSC_VER
#define my_assert(E) if (!(E)) __debugbreak();
#else
#include <cassert>
#define my_assert(E) assert(E)
#endif

template <typename... Args>
constexpr void print_warning(const char* message, Args...args) {
    SET_CLI_YELLOW();
    fprintf(stderr, message, args...);
    fputc('\n', stderr);
    RESET_CLI();
}

template <typename... Args>
constexpr void print_error(const char* message, Args...args) {
    SET_CLI_RED();
    fprintf(stderr, message, args...);
    fputc('\n', stderr);
    RESET_CLI();
}

template <typename... Args>
constexpr void print_success(const char* message, Args...args) {
    SET_CLI_GREEN();
    fprintf(stderr, message, args...);
    fputc('\n', stderr);
    RESET_CLI();
}

// for GCC/ Clang must define _DEBUG flags on build
#ifdef _DEBUG

#define GLCall(x) GLClearError(); \
    x; \
    my_assert(GLLogCall(#x, __FILE__, __LINE__))
constexpr bool DEBUG = 1;
#define warn(E) SET_CLI_YELLOW(); \
    std::cerr << "[Warning] " << E << __FILE__ << ':' << __LINE__ << std::endl; RESET_CLI()

#else

#define GLCall(x) x
constexpr bool DEBUG = 0;
#define warn(E)

#endif // _DEBUG

enum { ERR_GLEW_NOT_OK = 1 }; // Exit Error codes


void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
