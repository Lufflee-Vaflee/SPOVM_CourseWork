#pragma once

#include <ncurses.h>
#include <panel.h>

namespace YAExplorer
{

#define curs_invesible 0
#define curs_normal 1
#define curs_visible 2

class UI
{
    protected:

    UI()
    {
        initscr();
        clear();
        refresh();
        noecho();
        curs_set(curs_invesible);
        //start_color();

        int half_width = COLS / 2;

        up_bar = create_newwin(3, COLS, 0, 0);
        status_bar = create_newwin(3, COLS, LINES - 3, 0);
        left = create_newwin(LINES - 4, half_width + 1, 2, 0, ACS_LTEE, ACS_RTEE, ACS_LTEE, ACS_RTEE);
        right = create_newwin(LINES - 4, half_width, 2, half_width, ACS_TTEE, ACS_RTEE, ACS_BTEE, ACS_RTEE);


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