#include <ncurses.h>
#include "stuff.hpp"

namespace YAExplorer
{

chtype getcorner(int x, int y, bool U, bool B, bool L, bool R)
{

    U = (mvinch(y - 1, x) == (ACS_VLINE | COLOR_PAIR(1))) || U;
    B = (mvinch(y + 1, x) == (ACS_VLINE | COLOR_PAIR(1))) || B;
    L = (mvinch(y, x - 1) == (ACS_HLINE | COLOR_PAIR(1))) || L;
    R = (mvinch(y, x + 1) == (ACS_HLINE | COLOR_PAIR(1))) || R;

    int flags = 0;
    chtype result;

    flags += (int)U;
    flags <<= 1;
    flags += (int)B;
    flags <<= 1;
    flags += (int)L;
    flags <<= 1;
    flags += (int)R;

    switch (flags)
    {
    case 0b1111:
        result = ACS_PLUS;
        break;
    case 0b0111:
        result = ACS_TTEE;
        break;
    case 0b1011:
        result = ACS_BTEE;
        break;
    case 0b1101:
        result = ACS_LTEE;
        break;
    case 0b1110:
        result = ACS_RTEE;
        break;
    case 0b0101:
        result = ACS_ULCORNER;
        break;
    case 0b0110:
        result = ACS_URCORNER;
        break;
    case 0b1001:
        result = ACS_LLCORNER;
        break;
    case 0b1010:
        result = ACS_LRCORNER;
        break;
    default:
        return 0;
        break;
    }

    return 0;
}

void smart_wborder(WINDOW* win)
{
    int x, y;
    int maxx, maxy;
    getbegyx(win, y, x);
    getmaxyx(win, maxy, maxx);

    chtype ul = getcorner(x, y, false, true, false, true);
    chtype ur = getcorner(x + maxx - 1, y, false, true, true, false);
    chtype bl = getcorner(x, y + maxy - 1, true, false, false, true);
    chtype br = getcorner(x + maxx - 1, y + maxy - 1, true, false, true, false);

    wattron(win, COLOR_PAIR(1));
    wborder(win, 0, 0, 0, 0, ul, ur, bl, br);
    wattroff(win, COLOR_PAIR(1));
}

WINDOW* create_newwin(int height, int width, int starty, int startx, chtype lu_corner, chtype ru_corner, chtype ll_corner, chtype rl_corner)
{
    WINDOW *local_win;

	local_win = subwin(stdscr, height, width, starty, startx);
    wattron(local_win, COLOR_PAIR(1));
    wbkgd(local_win, COLOR_PAIR(1));
	wborder(local_win, 0, 0, 0, 0, lu_corner, ru_corner, ll_corner, rl_corner);
    wattroff(local_win, COLOR_PAIR(1));
	wrefresh(local_win);

	return local_win;
}

void destroy_win(WINDOW *local_win)
{
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}
}

