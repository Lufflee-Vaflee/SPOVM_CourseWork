#include "UI.hpp"

namespace YAExplorer
{

extern std::filesystem::path ROOT_DIR;

UI* UI::instance = nullptr;

std::string UI::status = "";

WINDOW* UI::up_bar = nullptr;
WINDOW* UI::status_bar = nullptr;

panelManager* UI::left = nullptr;
panelManager* UI::right = nullptr;

UI::UI() : smartWindow::smartWindow()
{
    initscr();
    clear();
    refresh();
    noecho();
    curs_set(curs_invisible);
    start_color();

    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    int half_width = COLS / 2;

    wbkgd(UI::up_bar, COLOR_PAIR(1));
    wrefresh(UI::up_bar);



    //UI::up_bar = subwin(stdscr, 1, COLS, 0, 0);
    //mvwprintw(UI::up_bar, 0, COLS - strlen(APP_NAME) - 1, "%s", APP_NAME);

    //UI::status_bar = create_newwin(3, COLS, LINES - 3, 0);

    //UI::left = new panelManager(stdscr, LINES - 3, COLS / 2 + 1, 1, 0);
    //UI::right = new panelManager(stdscr, LINES - 3, half_width, 1, half_width, ROOT_DIR);
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
    //destroy_win(UI::up_bar);
    //destroy_win(UI::status_bar);
    //delete UI::left;
    //delete UI::right;
    endwin();

}

}