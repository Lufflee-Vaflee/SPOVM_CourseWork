#include "UI.hpp"

namespace YAExplorer
{

UI* UI::instance = nullptr;

std::string UI::status = "";

WINDOW* UI::up_bar = nullptr;
WINDOW* UI::status_bar = nullptr;

dirPanel* UI::left = nullptr;
dirPanel* UI::right = nullptr;

UI::UI()
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

    UI::up_bar = newwin(1, COLS, 0, 0);
    wbkgd(UI::up_bar, COLOR_PAIR(1));
    mvwprintw(UI::up_bar, 0, COLS - strlen(APP_NAME) - 1, "%s", APP_NAME);
    wrefresh(UI::up_bar);

    UI::status_bar = create_newwin(3, COLS, LINES - 3, 0);

    UI::left = new dirPanel(LINES - 3, COLS / 2 + 1, 1, 0);
    UI::right = new dirPanel(LINES - 3, half_width, 1, half_width, ACS_TTEE, ACS_URCORNER, ACS_BTEE, ACS_RTEE);
}


UI* UI::GetInstance()
{
    return instance == nullptr ? instance = new UI() : instance;
}

void UI::CloseInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

WINDOW* UI::create_newwin(int height, int width, int starty, int startx, chtype lu_corner, chtype ru_corner, chtype ll_corner, chtype rl_corner)
{
    WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
    wattron(local_win, COLOR_PAIR(1));
    wbkgd(local_win, COLOR_PAIR(1));
	wborder(local_win, 0, 0, 0, 0, lu_corner, ru_corner, ll_corner, rl_corner);
    wattroff(local_win, COLOR_PAIR(1));
	wrefresh(local_win);

	return local_win;
}

void UI::destroy_win(WINDOW *local_win)
{
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}

void UI::setStatus(std::string message)
{
    status = message;
    mvwprintw(status_bar, 1, 1, "%s", message.c_str());
    wrefresh(status_bar);
}

std::string UI::getStatus()
{
    return UI::status;
}


UI::~UI()
{
    echo();
    curs_set(curs_visible);
    destroy_win(UI::up_bar);
    destroy_win(UI::status_bar);
    delete UI::left;
    delete UI::right;
    endwin();
}

}