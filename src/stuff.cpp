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
    case 0b1100:
        result = ACS_VLINE;
        break;
    case 0b0011:
        result = ACS_HLINE;
        break;
    default:
        return 0;
        break;
    }

    return result;
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

void smart_wborder_no_bottom(WINDOW* win)
{
    int x, y;
    int maxx, maxy;
    getbegyx(win, y, x);
    getmaxyx(win, maxy, maxx);

    wattron(win, COLOR_PAIR(1));
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);

    chtype ul = getcorner(x, y, false, true, false, true);
    chtype ur = getcorner(x + maxx - 1, y, false, true, true, false);
    chtype bl = getcorner(x, y + maxy - 1, true, false, false, false);
    chtype br = getcorner(x + maxx - 1, y + maxy - 1, true, false, false, false);

    wattron(win, COLOR_PAIR(1));
    wborder(win, 0, 0, 0, ' ', ul, ur, bl, br);
    wattroff(win, COLOR_PAIR(1));
}

WINDOW* create_newwin(int height, int width, int starty, int startx, chtype lu_corner, chtype ru_corner, chtype ll_corner, chtype rl_corner)
{
    WINDOW *local_win;

	local_win = subwin(stdscr, height, width, starty, startx);
    wattron(local_win, COLOR_PAIR(1));
    wbkgd(local_win, COLOR_PAIR(1));
	smart_wborder(local_win);
    wattroff(local_win, COLOR_PAIR(1));
	wrefresh(local_win);

	return local_win;
}

void destroy_win(WINDOW *local_win) // !!WARNING!! this thing is not warried about neigbours borders and deleting them all, correct borders deleting is under user responsibility
{
    wclear(local_win);
    wbkgd(local_win, COLOR_PAIR(1));
    wattron(local_win, COLOR_PAIR(1));
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wattroff(local_win, COLOR_PAIR(1));
	wrefresh(local_win);
	delwin(local_win);
}

std::string supress(std::string str, unsigned int to)
{
    if(str.size() <= to)
        return str;

    int overflow = str.size() - to;
    overflow = 2; 

    std::string result("");
    int first_part_size = (to - overflow) / 2;
    int second_part_size = (to - overflow) - first_part_size;
    result += str.substr(0, first_part_size);

    result += "..";

    result += str.substr(str.size() - second_part_size, second_part_size);

    return result;
}

}

