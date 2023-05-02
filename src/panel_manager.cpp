#include "panel_manager.hpp"

#include <ncurses.h>

#include <exception>
#include <string>
#include <list>
#include <filesystem>
#include <cstring>

#define TAB_SIZE_DEFAULT 20
#define TAB_SIZE_MAX 30
#define NEW_TAB  "NEW TAB"


namespace YAExplorer
{

panelManager::panelManager(WINDOW* parent, int height, int width, int starty, int startx, std::filesystem::path dir)
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

    this->tabs.push_back(NEW_TAB);
    this->cur_tab = tabs.begin();

    registrate_tab(dir.c_str());
    set_current(tabs.begin());

    redraw_tabs();
}

void panelManager::registrate_tab(std::string name) 
{
    tabs.emplace(--this->tabs.end(), name);
    int tabs_common_size = tabs.size() * TAB_SIZE_MAX;  //there is always selected tab with maximized size
    if (tabs_common_size > width - 1)
    {
        tabs_size = (width - TAB_SIZE_MAX - 1) / (tabs.size() - 1);
    }
}

void panelManager::set_current(std::list<std::string>::iterator tab)
{
    if (tab != tabs.end())
    {
        this->cur_tab = tab; 
    }
}

void panelManager::remove_tab(std::list<std::string>::iterator remove)
{
    if (remove == tabs.end() || remove == --tabs.end())
        return;
    else if (remove == cur_tab)
        cur_tab = cur_tab++;

    this->tabs.erase(remove);

    return;
}

void panelManager::redraw_tabs()
{
    for (auto i = bars.begin(); i != bars.end(); i++)
        delete *i;

    int correction = (width - 1 - TAB_SIZE_MAX) % tabs_size;
    int countx = 0;
    for (auto i = tabs.begin(); i != tabs.end(); i++)
    {
        int real_tab_size = tabs_size;

        if (i == this->cur_tab)
        {
            real_tab_size = TAB_SIZE_MAX;
            bars.emplace_back(derwin(tabbar, 3, real_tab_size + 1, 0, countx));
            smart_wborder_no_bottom((*bars.rbegin()));
            wattron((*bars.rbegin()), COLOR_PAIR(1));
            mvwprintw((*bars.rbegin()), 1, 1, "%s", supress((*i), real_tab_size - 1).c_str());
            wattroff((*bars.rbegin()), COLOR_PAIR(1));
            wrefresh((*bars.rbegin()));
            countx += real_tab_size;

            continue;
        }
        else if (correction)
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