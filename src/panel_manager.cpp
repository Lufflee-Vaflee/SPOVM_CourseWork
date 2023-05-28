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
    auto builder = dirWindow::Creator(0, 2, this->get_width(), this->get_height() - 2, _default);
    this->body = std::dynamic_pointer_cast<dirWindow>(this->create(builder).lock()); 
    this->draw_borders();
    this->refresh();
    this->tabs.push_back(NEW_TAB);
    this->registrate_tab(_default.c_str());
    this->cur_tab = tabs.begin();
    this->redraw_tabs();
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

    this->draw_borders();
    this->refresh();
    this->body.lock()->draw_borders();
    this->body.lock()->refresh();
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

        auto builder = smartWindow::Creator(countx, 0, real_tab_size + 1, 3);

        weak_ptr<smartWindow> t = this->create(builder);

        view.push_back(t);

        if (i == this->cur_tab)
            (*view.rbegin()).lock()->draw_borders_no_bottom();
        else
            (*view.rbegin()).lock()->draw_borders();

        (*view.rbegin()).lock()->print(*i);

        countx += real_tab_size;
    }

    //wrefresh(tabbar);
}

panelManager::Creator::Creator(int x, int y, int width, int height, std::filesystem::path _default, const list<weak_ptr<smartWindow>>& neighbours)
    : smartWindow::Creator(x, y, width, height, neighbours)
{
    this->_default = _default;
}

weak_ptr<smartWindow> panelManager::Creator::create(smartWindow& parent)
{
    shared_ptr<smartWindow> win = std::dynamic_pointer_cast<smartWindow>(std::shared_ptr<panelManager>(new panelManager(x, y, width, height, parent, _default,neighbours)));
    this->registrate_heir(parent, win);
    return win;
}

}