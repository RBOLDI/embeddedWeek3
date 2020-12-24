// w4.2.2ncursesio

#ifndef _NCURSESIO_HPP_
#define _NCURSESIO_HPP_
#define ESC 27
#define ENTER 10
#define SHIFT_RIGHT 402
#define SHIFT_LEFT	393

#define WINDOW_1 	0
#define WINDOW_2	1
#define WINDOW_NUM	2

#define SELECTED_WIN 1
#define UNSELECTED_WIN 2
//TODO: zorg dat ncurses beschikbaar is
#include <iostream>
#include <curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Maak #define voor handige delay-functie in msec
#define thread_delay(nMSec) \
{ \
struct timespec oTime, oTimeLeft; \
oTime.tv_sec = 0; \
oTime.tv_nsec = (nMSec) * 1000000L; \
nanosleep(&oTime, &oTimeLeft); \
};
/*********************************************************************************
INITIALIZE WINDOWS
*********************************************************************************/
//WINDOW CLASS POINTERS
extern WINDOW *nWindows[WINDOW_NUM];
extern WINDOW * mainwin, * childWin_1, * childWin_2, * childwinh;

int initMainWindow();
int initHeader(void);
int initChildWindow_1(void);
int initChildWindow_2(void);

/*********************************************************************************
WINDOW MANAGER
*********************************************************************************/
uint8_t nextActiveWindow(void);
uint8_t previousActiveWindow(void);
bool closeCurrentWindow(void);
bool windowManager(char *currProgOutput);
bool startNcurses(void);
bool closeNcurses(void);

#endif // _NCURSESIO_HPP_