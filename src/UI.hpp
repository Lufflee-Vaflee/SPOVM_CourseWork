#pragma once

#include <ncurses.h>
#include <exception>
#include <cstring>
#include "tab.hpp"

namespace YAExplorer
{

#define curs_invisible 0
#define curs_normal 1
#define curs_visible 2

#define APP_NAME "Yet Another Explorer"

class UI
{
    protected:

    UI()
    {
        //if (has_colors() == false)
        //    throw new std::exception();

        initscr();
        clear();
        refresh();
        noecho();
        curs_set(curs_invisible);
        start_color();

        init_pair(1, COLOR_CYAN, COLOR_BLACK);

        int half_width = COLS / 2;

        up_bar = newwin(1, COLS, 0, 0);
        wbkgd(up_bar, COLOR_PAIR(1));
        mvwprintw(up_bar, 0, COLS - strlen(APP_NAME) - 1, "%s", APP_NAME);
        wrefresh(up_bar);
    
        status_bar = create_newwin(3, COLS, LINES - 3, 0);

        left = new dirPanel(LINES - 3, COLS / 2 + 1, 1, 0);
        right = new dirPanel(LINES - 3, half_width, 1, half_width, ACS_TTEE, ACS_URCORNER, ACS_BTEE, ACS_RTEE);
    }

    static UI* instance;

    WINDOW* up_bar;
    WINDOW* status_bar;

    dirPanel* left;
    dirPanel* right;

    public:

    UI(UI &other) = delete;
    void operator=(const UI&) = delete;

    static UI* GetInstance();
    static void CloseInstance();

    static WINDOW* create_newwin(int height, int width, int starty, int startx, chtype lu_corner = ACS_ULCORNER, chtype ru_corner = ACS_URCORNER, chtype ll_corner = ACS_LLCORNER, chtype rl_corner = ACS_LRCORNER);
    static void destroy_win(WINDOW *local_win);


    private:

    ~UI()
    {
        echo();
        curs_set(curs_visible);
        destroy_win(up_bar);
        destroy_win(status_bar);
        delete left;
        delete right;
        endwin();
    }
};




}