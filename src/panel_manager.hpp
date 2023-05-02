#pragma once

#include <ncurses.h>

#include <exception>
#include <string>
#include <list>
#include <filesystem>
#include <cstring>

#include "stuff.hpp"

#define TAB_SIZE_DEFAULT 20
#define TAB_SIZE_MAX 30
#define NEW_TAB  "NEW TAB"

namespace YAExplorer
{

//i guess i need better name for this class
class panelManager
{
    public:

    panelManager(WINDOW* parent, int height, int width, int starty, int startx, chtype ul = ACS_ULCORNER, chtype ur = ACS_TTEE, chtype ll = ACS_LTEE, chtype lr = ACS_BTEE);

    panelManager(panelManager &other) = delete;
    void operator=(const panelManager&) = delete;


    private:

    WINDOW* current;
    WINDOW* tabbar;
    std::list<std::string> tabs;
    std::list<WINDOW*> bars;

    std::list<std::string>::iterator cur_tab;

    int height;
    int width;

    int tabs_common_size = strlen(NEW_TAB);
    int tabs_size = TAB_SIZE_MAX;

    void registrate_tab(std::string name);

    void redraw_tabs();

    public:

    ~panelManager()
    {
        wborder(current, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	    wrefresh(current);
	    delwin(current);
    }

};

};