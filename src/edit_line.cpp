#include "edit_line.hpp"

namespace YAExplorer
{


editLine::editLine(int x, int y, int width, std::string _default, const smartWindow& parent, const list<weak_ptr<smartWindow>>& neighbours)
    : smartWindow(x, y, width, 1, parent, neighbours)
{
    this->model = _default;
    wmove(this->raw, 0, 1);
    curs_set(curs_visible);
    this->refresh();
}

int editLine::move_cursor_left()
{
    if(this->cursor_position > 0)
        this->cursor_position--;
    else if(this->model_display_start > 0)
    {
        this->model_display_start--;
        this->redraw();
    }

    wmove(this->raw, 0, this->cursor_position + 1);
    this->refresh();
    return this->cursor_position;
}

int editLine::move_cursor_right()
{
    if((this->get_width() - 2) > model.size())
    {
        if(this->cursor_position < model.size() - 1)
            this->cursor_position++;
    }
    else
    {
        if(this->cursor_position < this->get_width() - 3)
            this->cursor_position++;
        else if (this->model_display_start <= this->model.size() - (this->get_width() - 2))
            model_display_start++;
    }

    wmove(this->raw, 0, this->cursor_position + 1);
    this->refresh();
    return this->cursor_position;
}

editLine::Creator::Creator(int x, int y, int width, std::string _default, const list<weak_ptr<smartWindow>>& neighbours)
    : smartWindow::Creator(x, y, width, 1, neighbours)
{
    this->_default = _default;
}

weak_ptr<smartWindow> editLine::Creator::create(smartWindow& parent)
{
    shared_ptr<smartWindow> win = std::dynamic_pointer_cast<smartWindow>(std::shared_ptr<editLine>(new editLine(x, y, width, _default, parent, neighbours)));
    this->registrate_heir(parent, win);
    return win;
}

int editLine::insert(char c)
{
    this->model = this->model.insert(this->model_display_start + this->cursor_position, 1, c);

    if(this->cursor_position == this->get_width() - 1)
        this->model_display_start++;

    return this->model_display_start + this->cursor_position;
}

int editLine::erase()
{
    this->model = this->model.erase(this->model_display_start + this->cursor_position, 1);

    if(this->cursor_position == 0 && this->model_display_start != 0)
        this->model_display_start--;

    return this->model_display_start + this->cursor_position;
}


void editLine::redraw()
{
    this->print(std::string(this->get_width() - 2, ' '), 0, 1);
    this->print(this->model.substr(this->model_display_start, this->get_width() - 2), 0, 1);
    wmove(this->raw, 0, this->cursor_position + 1);
    this->refresh();
}

editLine::~editLine()
{
    curs_set(curs_invisible);
}




}