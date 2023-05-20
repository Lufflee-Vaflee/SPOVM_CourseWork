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

panelManager::panelManager(int x, int y, int width, int height, const smartWindow& parent, std::filesystem::path _default, const list<weak_ptr<smartWindow>>& neighbours)
    : smartWindow(x, y, width, height, parent, neighbours)
{
    this->draw_borders();
    this->refresh();
    this->tabs.push_back(NEW_TAB);
    this->registrate_tab(_default.c_str());
    this->cur_tab = tabs.begin();
    this->redraw_tabs();



    /*
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
    */
}

void panelManager::registrate_tab(std::string name) 
{
    tabs.emplace(--this->tabs.end(), name);
    int tabs_common_size = tabs.size() * TAB_SIZE_MAX;  //there is always selected tab with maximized size
    if (tabs_common_size > this->get_width() - 1)
    {
        tabs_size = (this->get_width() - TAB_SIZE_MAX - 1) / (tabs.size() - 1);
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

    int tabs_common_size = tabs.size() * TAB_SIZE_MAX;  //there is always selected tab with maximized size
    if (tabs_common_size > this->get_width() - 1)
        tabs_size = (this->get_width() - TAB_SIZE_MAX - 1) / (tabs.size() - 1);

    return;
}

void panelManager::flush_tabs()
{
    for(auto i = view.begin(); i != view.end(); i++)
    {
        this->delete_(*i); // add check if not exist
    }

    this->view.clear();

    this->body.lock()->draw_borders();
    this->body.lock()->refresh();
    this->draw_borders();
    this->refresh();
}

void panelManager::redraw_tabs()
{
    flush_tabs();

    int correction = (this->get_width() - 1 - TAB_SIZE_MAX) % tabs_size;
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

        view.push_back(this->create(smartWindow::Creator(countx, 0, real_tab_size + 1, 3)));

        if (i == this->cur_tab)
            (*view.rend()).lock()->draw_borders_no_bottom();
        else
            (*view.rend()).lock()->draw_borders();

        (*view.rend()).lock()->print(*i);

        countx += real_tab_size;
    }


    //wrefresh(tabbar);
}



}