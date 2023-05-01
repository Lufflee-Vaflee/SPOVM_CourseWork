#pragma once

#include <ncurses.h>
#include <panel.h>
#include <exception>
#include <cstring>

namespace YAExplorer
{

#define curs_invesible 0
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
        curs_set(curs_invesible);
        start_color();

        init_pair(1, COLOR_CYAN, COLOR_BLACK);

        int half_width = COLS / 2;

        up_bar = newwin(1, COLS, 0, 0);
        wbkgd(up_bar, COLOR_PAIR(1));

        mvwprintw(up_bar, 0, COLS - strlen(APP_NAME) - 1, "%s", APP_NAME);
        wrefresh(up_bar);
    
        status_bar = create_newwin(3, COLS, LINES - 3, 0);
        left = create_newwin(LINES - 3, half_width + 1, 1, 0, ACS_ULCORNER, ACS_RTEE, ACS_LTEE, ACS_RTEE);
        right = create_newwin(LINES - 3, half_width, 1, half_width, ACS_TTEE, ACS_URCORNER, ACS_BTEE, ACS_RTEE);

        //wattron(left, COLOR_PAIR(1));
        mvwprintw(left, 0, 2, "%s", " Some dir ");
        wrefresh(left);

        mvwprintw(right, 0, 2, "%s", " Some another dir ");
        wrefresh(right);
        //wattroff(left, COLOR_PAIR(1));

    }

    static UI* instance;

    WINDOW* up_bar;
    WINDOW* left;
    WINDOW* right;
    WINDOW* status_bar;


    WINDOW* create_newwin(int height, int width, int starty, int startx, chtype lu_corner = ACS_ULCORNER, chtype ru_corner = ACS_URCORNER, chtype ll_corner = ACS_LLCORNER, chtype rl_corner = ACS_LRCORNER);
    void destroy_win(WINDOW *local_win);

    public:

    UI(UI &other) = delete;

    void operator=(const UI&) = delete;

    static UI* GetInstance();
    static void CloseInstance();


    private:

    ~UI()
    {
        destroy_win(left);
        destroy_win(right);
        echo();
        curs_set(curs_visible);
        endwin();
    }
};




}