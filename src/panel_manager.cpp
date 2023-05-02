#include "panel_manager.hpp"

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

panelManager::panelManager(WINDOW* parent, int height, int width, int starty, int startx, chtype ul, chtype ur, chtype ll, chtype lr)
{
    if (height < 1 || width < 1 || starty < 0 || startx < 0)
        throw new std::exception();

    this->height = height;
    this->width = width;
    current = subwin(parent, height, width, starty, startx);
    wbkgd(current, COLOR_PAIR(1));
    smart_wborder(current);
    wrefresh(current);

    tabbar = subwin(parent, 3, width, starty, startx);
    smart_wborder(tabbar);
    
    wrefresh(tabbar);


    //WINDOW* default_tab = newwin(3, TAB_SIZE_DEFAULT, tabbar->_begy, tabbar->_begx);
    //wattron(default_tab, COLOR_PAIR(1));
    //wborder(default_tab, 0, 0, 0, ' ', ul, ur, ACS_VLINE, lr);
    //mvwprintw(default_tab, 1, 1, "%s", NEW_TAB);
    //wattroff(default_tab, COLOR_PAIR(1));
    //wrefresh(default_tab);

    registrate_tab("");
    registrate_tab("avafsad");

    redraw_tabs();
}

void panelManager::registrate_tab(std::string name) 
{
    tabs.push_back(name);
    tabs_common_size = tabs.size() * (tabs_size - 1) + TAB_SIZE_MAX;  //there is always selected tab with maximized size
    if (tabs_common_size > width - 1)
    {
        //tabs_size = (width - (cur_tab->size() > TAB_SIZE_MAX ? TAB_SIZE_MAX : cur_tab->size())) / tabs.size();
        tabs_size = width / tabs.size();
    }
}

void panelManager::redraw_tabs()
{
    for (auto i = bars.begin(); i != bars.end(); i++)
        delete *i;

    int correction = (width - 1) % tabs_size;
    int countx = 0;
    for (auto i = tabs.begin(); i != tabs.end(); i++)
    {
        int real_tab_size = tabs_size;
        if (correction)
        {
            real_tab_size = tabs_size + 1;
            correction--;
        }

        bars.emplace_back(derwin(tabbar, 3, real_tab_size + 1, 0, countx));
        smart_wborder((*bars.rbegin()));
        wattron((*bars.rbegin()), COLOR_PAIR(1));
        mvwprintw((*bars.rbegin()), 1, 1, "%s", supress((*i), real_tab_size - 1).c_str());
        wattroff((*bars.rbegin()), COLOR_PAIR(1));
        wrefresh((*bars.rbegin()));
        countx += real_tab_size;
    }
    
    wrefresh(tabbar);
}




}