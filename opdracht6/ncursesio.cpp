// Embedded Systems | HvA 2020
// Opdracht 4 week 2
//
// Rowan Bolding
// Bart-Jan van Strien
//
// Compile: g++ -Wall -o ncurses ncurses.cpp -lncurses
// Run    : ./ncurses
#include "ncursesio.hpp"

using namespace std;

//Main window parameters:
static int width; 
static int height;
static int halfwidth;
static int max_x;
static int max_y;

int initMainWindow();
int initHeader(void);
int initChildWindow_1(void);
int initChildWindow_2(void);

uint8_t nextActiveWindow(void);
uint8_t previousActiveWindow(void);
bool closeCurrentWindow(void);
bool windowManager(char *currProgOutput);

bool window(WINDOW *childWin, char *currProgOutput);
bool closeWindow(WINDOW *childWin);

bool startNcurses(void);
bool closeNcurses(void);

/*********************************************************************************
INITIALIZE WINDOWS
*********************************************************************************/
int initMainWindow(){

    if ( (mainwin = initscr()) == NULL )
    {
        fprintf(stderr, "ERROR INIT NCURSES\n");
        exit(EXIT_FAILURE);
    }
    box(mainwin, '|', '-');

    getmaxyx(stdscr,max_y,max_x);
    width = max_x;
	height = max_y;
    halfwidth = width / 2;

    noecho();
    start_color();
    keypad(mainwin, TRUE);

    init_pair(1, COLOR_RED,COLOR_BLACK);
    init_pair(2, COLOR_WHITE,COLOR_BLACK);
	return 0;
}

int initHeader(void){
	// 
	char str[255];
	uint8_t str_len = 0;
    childwinh = subwin(mainwin, 10, width - 4, 1, 2);
    box(childwinh, '|', '-');
    mvwaddstr(childwinh, 1, 3, "8888888888     888               d8b           .d88888b.   .d8888b.   |");
    mvwaddstr(childwinh, 2, 3, "888            888               Y8P          d88P' 'Y88b d88P  Y88b  |");
    mvwaddstr(childwinh, 3, 3, "888            888                            888     888 Y88b.       |");
    mvwaddstr(childwinh, 4, 3, "8888888    .d88888 888  888  888 888 88888b.  888     888  'Y888b.    |");
    mvwaddstr(childwinh, 5, 3, "888       d88' 888 888  888  888 888 888 '88b 888     888     'Y88b.  |");
    mvwaddstr(childwinh, 6, 3, "888       888  888 888  888  888 888 888  888 888     888       '888  |");
    mvwaddstr(childwinh, 7, 3, "888       Y88b 888 Y88b 888 d88P 888 888  888 Y88b. .d88P Y88b  d88P  |");
    mvwaddstr(childwinh, 8, 3, "8888888888 'Y88888  'Y8888888P'  888 888  888  'Y88888P'   'Y8888P'   |");
    mvwaddstr(childwinh, 6, 76, "Embedded Systems | HvA 2020");
    mvwaddstr(childwinh, 7, 76, "Rowan Bolding");
    mvwaddstr(childwinh, 8, 76, "Bart-Jan van Strien");
	strcpy(str, "Press SHIFT+Right_arrow -> to switch to next program");
	str_len = strlen(str);
	mvwaddstr(childwinh, 6, max_x - (str_len+7), str);
	strcpy(str, "Press SHIFT+Left_arrow <- to switch to previous program");
	str_len = strlen(str);
	mvwaddstr(childwinh, 7, max_x - (str_len+7), str);
	strcpy(str, "Press ESC to exit");
	str_len = strlen(str);
    mvwaddstr(childwinh, 8, max_x - (str_len+7), str);
	
	return 0;
}

int initChildWindow_1(void){
	attron(COLOR_PAIR(SELECTED_WIN));
    childWin_1 = subwin(mainwin, height - 12, halfwidth - 2, 11, 2);
    box(childWin_1, '|', '-');
    attroff(COLOR_PAIR(SELECTED_WIN));
    mvwaddstr(childWin_1, 1, 0, "WINDOW 1");
	return 0;
}

int initChildWindow_2(void){
	attron(COLOR_PAIR(UNSELECTED_WIN));
    childWin_2 = subwin(mainwin, height - 12, halfwidth - 3, 11, halfwidth + 1);
    box(childWin_2, '|', '-');
    attroff(COLOR_PAIR(UNSELECTED_WIN));
	mvwaddstr(childWin_1, 1, 0, "WINDOW 2");
    // mvwaddstr(childWin_2, 6, 7, "Verbinden...");
    //mvwaddstr(childWin_2, 7, 11, "met XMEGA...");
    //mvwaddstr(childWin_2, 8, 8, "Rowan Bolding");
    //mvwaddstr(childWin_2, 9, 5, "Bart-Jan van Strien");
	return 0;
}

/*********************************************************************************
WINDOW MANAGER
*********************************************************************************/
uint8_t activeWindow_index = WINDOW_1;

uint8_t nextActiveWindow(void){
	activeWindow_index++;
	if(activeWindow_index == WINDOW_NUM) activeWindow_index = WINDOW_1;
	return activeWindow_index;
}

uint8_t previousActiveWindow(void){
	if(activeWindow_index == WINDOW_1) activeWindow_index = WINDOW_NUM-1;
	activeWindow_index--;
	return activeWindow_index;
}

bool closeCurrentWindow(void){
	closeWindow(nWindows[activeWindow_index]);
	return false;
}

bool windowManager(char *currProgOutput){
	window(nWindows[activeWindow_index], currProgOutput);
	return false;
}

bool window(WINDOW *childWin, char *currProgOutput){
	char string[255];
	if(currProgOutput == NULL) {
		wmove(childWin,3,1);
		wclrtoeol(childWin);
		wmove(childWin,4,1);
		wclrtoeol(childWin);
		wmove(childWin,5,1);
		wclrtoeol(childWin);
		sprintf(string,"WINDOW/PROGRAM_%u:", (activeWindow_index+1));
		mvwaddstr(childWin, 3, 3, string);
		mvwaddstr(childWin, 4, 3, "No program found...");
		attron(COLOR_PAIR(UNSELECTED_WIN));
		box(childWin, '|', '-');
		attroff(COLOR_PAIR(UNSELECTED_WIN));
		wrefresh(childWin);
		return false;
	}
	else {
		wmove(childWin,3,1);
		wclrtoeol(childWin);
		wmove(childWin,4,1);
		wclrtoeol(childWin);
		wmove(childWin,5,1);
		wclrtoeol(childWin);
		sprintf(string,"WINDOW/PROGRAM_%u ACTIVE:", (activeWindow_index+1));
		mvwaddstr(childWin, 3, 3, string);
		sprintf(string,"%s", currProgOutput);
		mvwaddstr(childWin, 4, 3, string);
		attron(COLOR_PAIR(UNSELECTED_WIN));
		box(childWin, '|', '-');
		attroff(COLOR_PAIR(UNSELECTED_WIN));
		wrefresh(childWin);
		return false;
	}
}

bool closeWindow(WINDOW *childWin){
	char string[100];
	wmove(childWin,3,1);
	wclrtoeol(childWin);
	wmove(childWin,4,1);
	wclrtoeol(childWin);
	wmove(childWin,5,1);
	wclrtoeol(childWin);
	sprintf(string,"WINDOW/PROGRAM_%u CLOSED!!", (activeWindow_index+1));
	mvwaddstr(childWin, 3, 3, string);
	sprintf(string,"SELECT WINDOW/PROGRAM_%u TO CONTINUE...", (activeWindow_index+1));
	mvwaddstr(childWin, 4, 3, string);
	attron(COLOR_PAIR(UNSELECTED_WIN));
	box(childWin, '|', '-');
	attroff(COLOR_PAIR(UNSELECTED_WIN));
	wrefresh(childWin);
	return false;
}

bool startNcurses(void){
	printw("w4.2.2ncursesio\n\n"); refresh();
	initMainWindow();
	initHeader();
	initChildWindow_1();
    initChildWindow_2();
    refresh();
	
	//start met childWindow 1 (childWin_1)
    wmove(childWin_1,1,1);
    wclrtoeol(childWin_1);
    attron(COLOR_PAIR(UNSELECTED_WIN));
    box(childWin_1, '|', '-');
    attroff(COLOR_PAIR(UNSELECTED_WIN));
	return false;
}

bool closeNcurses(void){
	delwin(childwinh);
    delwin(childWin_1);
    delwin(childWin_2);
    delwin(mainwin);
    endwin();
    refresh();
    return false;
}