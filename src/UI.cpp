#include "UI.hpp"
#include "panel_manager.hpp"

namespace YAExplorer
{



UI::UI()
{
    initscr();
    clear();
    refresh();
    noecho();
    curs_set(curs_invisible);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
}


UI::~UI()
{
    echo();
    curs_set(curs_visible);
    endwin();
}

}