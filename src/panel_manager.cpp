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
    current = derwin(parent, height, width, starty, startx);
    wbkgd(current, COLOR_PAIR(1));
    smart_wborder(current);
    wrefresh(current);

    tabbar = derwin(current, 3, width, 0, 0);
    smart_wborder(tabbar);
    
    wrefresh(tabbar);

    this->tabs.push_back(tab(NEW_TAB, nullptr));
    this->cur_tab = tabs.begin();

    registrate_tab(dir.c_str());
    set_current(tabs.begin());

    redraw_tabs();
}

void panelManager::registrate_tab(std::string name) 
{
    tabs.emplace(--this->tabs.end(), name, nullptr);
    int tabs_common_size = tabs.size() * TAB_SIZE_MAX;  //there is always selected tab with maximized size
    if (tabs_common_size > width - 1)
    {
        tabs_size = (width - TAB_SIZE_MAX - 1) / (tabs.size() - 1);
    }
}

void panelManager::set_current(std::list<tab>::iterator tab)
{
    if (tab != tabs.end())
    {
        this->cur_tab = tab; 
    }
}

void panelManager::remove_tab(std::list<tab>::iterator remove)
{
    if (remove == tabs.end() || remove == --tabs.end())
        return;
    else if (remove == cur_tab)
        cur_tab = cur_tab++;

    delwin(remove->second);
    this->tabs.erase(remove);

    int tabs_common_size = tabs.size() * TAB_SIZE_MAX;  //there is always selected tab with maximized size
    if (tabs_common_size > width - 1)
        tabs_size = (width - TAB_SIZE_MAX - 1) / (tabs.size() - 1);

    return;
}

void panelManager::flush_tabs()
{
    for(auto i = tabs.begin(); i != tabs.end(); i++)
    {
        i->second = nullptr;
    }

    destroy_win(tabbar);
    tabbar = derwin(current, 3, width, 0, 0);
    smart_wborder(tabbar);
    wrefresh(tabbar);
}

void panelManager::redraw_tabs()
{
    flush_tabs();

    int correction = (width - 1 - TAB_SIZE_MAX) % tabs_size;
    int countx = 0;
    for (auto i = tabs.begin(); i != tabs.end(); i++)
    {
        int real_tab_size = tabs_size;

        if (i == this->cur_tab)
            real_tab_size = TAB_SIZE_MAX;
        else if (correction)
        {
            real_tab_size = tabs_size + 1;
            correction--;
        }

        i->second = derwin(tabbar, 3, real_tab_size + 1, 0, countx);

        if (i == this->cur_tab)
            smart_wborder_no_bottom(i->second);
        else
            smart_wborder(i->second);

        wattron(i->second, COLOR_PAIR(1));
        mvwprintw(i->second, 1, 1, "%s", supress(i->first, real_tab_size - 1).c_str());
        wattroff(i->second, COLOR_PAIR(1));
        wrefresh(i->second);
        countx += real_tab_size;
    }


    wrefresh(tabbar);
}

panelManager::~panelManager()
{
    flush_tabs();

    destroy_win(current);
}



}