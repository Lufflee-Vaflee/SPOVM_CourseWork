#pragma once

#include <ncurses.h>
#include <panel.h>
#include <exception>
#include <string>
#include <list>
#include <filesystem>
#include <cstring>

#define TAB_SIZE_DEFAULT 20
#define TAB_SIZE_MAX 80
#define NEW_TAB  "NEW TAB"

namespace YAExplorer
{

class tab
{
    public:
    tab(std::string name, WINDOW* win)
    {
        this->name = name;
        this->win = win;
    }

    std::string name;
    WINDOW* win;

    ~tab()
    {
        delete win;
    }
};

class panelManager
{
    public:

    panelManager(WINDOW* parent, int height, int width, int starty, int startx, chtype ul = ACS_ULCORNER, chtype ur = ACS_TTEE, chtype ll = ACS_LTEE, chtype lr = ACS_BTEE)
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

        registrate_tab("asdfsdihfvo123dfn");
        registrate_tab("avafsad");
        registrate_tab("||||");
        registrate_tab("bwnjnihwfowjer");
        registrate_tab("2");
        registrate_tab("fdkmw");
        registrate_tab("wnjfvnoijef");
        registrate_tab("avnwjf");
        registrate_tab("wkjnvefpwe");
        registrate_tab("vwjnfj wojei weoijpoknvw wevjpf");

        redraw_tabs();


    }

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

    void registrate_tab(std::string name)
    {
        tabs.push_back(name);
        tabs_common_size = tabs.size() * tabs_size;
        if (tabs_common_size > width - 1)
        {
            //tabs_size = (width - (cur_tab->size() > TAB_SIZE_MAX ? TAB_SIZE_MAX : cur_tab->size())) / tabs.size();
            tabs_size = width / tabs.size();
        }
    }

    void redraw_tabs()
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

    public:

    ~panelManager()
    {
        wborder(current, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	    wrefresh(current);
	    delwin(current);
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
        default:
            return 0;
            break;
        }

        return result;
    }
};

};