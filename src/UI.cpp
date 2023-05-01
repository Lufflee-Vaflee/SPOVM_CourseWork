#include "UI.hpp"

namespace YAExplorer
{

UI* UI::instance = nullptr;

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

}