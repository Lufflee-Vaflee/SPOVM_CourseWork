#pragma once

#include <ncurses.h>
#include <string>

//this will be temporary place for all procedures unrelated to already existing objects while i dont find better place for them

namespace YAExplorer
{

chtype getcorner(int x, int y, bool U, bool B, bool L, bool R);

void smart_wborder(WINDOW* win);

WINDOW* create_newwin(int height, int width, int starty, int startx, chtype lu_corner = ACS_ULCORNER, chtype ru_corner = ACS_URCORNER, chtype ll_corner = ACS_LLCORNER, chtype rl_corner = ACS_LRCORNER);

void destroy_win(WINDOW *local_win);

std::string supress(std::string str, unsigned int to);

}

