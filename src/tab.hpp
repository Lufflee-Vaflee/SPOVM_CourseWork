#pragma once

#include <ncurses.h>
#include <panel.h>
#include <exception>
#include <cstring>

namespace YAExplorer
{

class dirPanel
{
    public:

    dirPanel(int height, int width, int starty, int startx, chtype ul = ACS_ULCORNER, chtype ur = ACS_TTEE, chtype ll = ACS_LTEE, chtype lr = ACS_BTEE)
    {
        current = newwin(height, width, starty, startx);
        wattron(current, COLOR_PAIR(1));
        wbkgd(current, COLOR_PAIR(1));
	    wborder(current, 0, 0, 0, 0, ul, ur, ll, lr);
        wattroff(current, COLOR_PAIR(1));
	    wrefresh(current);
    }

    dirPanel(dirPanel &other) = delete;
    void operator=(const dirPanel&) = delete;


    private:

    WINDOW* current;
    



    public:

    ~dirPanel()
    {
        wborder(current, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	    wrefresh(current);
	    delwin(current);
    }
};

};