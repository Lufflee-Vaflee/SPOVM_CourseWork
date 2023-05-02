#pragma once

#include <ncurses.h>

#include <exception>
#include <string>
#include <list>
#include <filesystem>
#include <cstring>

#include "stuff.hpp"

#define TAB_SIZE_MAX 30
#define NEW_TAB  "NEW TAB"

namespace YAExplorer
{

//i guess i need better name for this class
//these thing should manage all tabs of left or ride panels of app
//it needs to:
//adding new tabs
//removing tabs
//changing tabs positions
//display correctly upbar representing all tabs with acsent on current chosed tab

//rules:
//its always display one special tab representing new tab button at the end of tab bar
//current tab always have TAB_SIZE_MAX while other tabs must shrink corresponding to panel width
//add check for allowoble minimal panel size - ? 


class panelManager
{
    public:

    panelManager(WINDOW* parent, int height, int width, int starty, int startx);

    panelManager(panelManager &other) = delete;
    void operator=(const panelManager&) = delete;


    private:

    WINDOW* current;
    WINDOW* tabbar;

    std::list<std::string> tabs; //defenetly bad idea
    std::list<WINDOW*> bars;

    std::list<std::string>::iterator cur_tab;

    int height;
    int width;

    int tabs_common_size = strlen(NEW_TAB);
    int tabs_size = TAB_SIZE_MAX;


    void registrate_tab(std::string name);                      //that thing only REGISTER, NOT DRAWING new tab, only for internal class use
    void remove_tab(std::list<std::string>::iterator removed);  //most likely deleting tab will be current chosen, os iterator will be good idea
    void set_current(std::list<std::string>::iterator);
    void redraw_tabs();                                         //as evety possible change will defenitly cause all tabs structure is easiear to reedraw all tabs, only for internal use


    public:

    ~panelManager()
    {
        wborder(current, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	    wrefresh(current);
	    delwin(current);
    }

};

};