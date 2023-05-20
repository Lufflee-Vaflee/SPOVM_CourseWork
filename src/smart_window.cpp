#include "smart_window.hpp"

namespace YAExplorer
{

    smartWindow smartWindow::main = smartWindow();

    smartWindow::smartWindow()
    {
        UI::instance(); // guarantee UI initialization
        this->raw = stdscr;
        this->flush();
        this->refresh();
    }

    smartWindow::smartWindow(int x, int y, int width, int height, const smartWindow &parent, const list<weak_ptr<smartWindow>> &neighbours)
    {
        this->raw = derwin(parent.raw, height, width, y, x);
        this->neighbours = neighbours;
    }

    smartWindow::smartWindow(smartWindow &&other)
    {
        this->raw = other.raw;
        other.raw = nullptr;
        this->neighbours = other.neighbours;
        other.neighbours.clear();
        this->heirs = other.heirs;
        other.heirs.clear();
        this->auto_refresh = other.auto_refresh;
    }

    int smartWindow::getX()
    {
        return getbegx(this->raw);
    }

    int smartWindow::getY()
    {
        return getbegy(this->raw);
    }

    int smartWindow::get_width() // https://linux.die.net/man/3/getbegyx
    {
        return getmaxx(this->raw);
    }

    int smartWindow::get_height()
    {
        return getmaxy(this->raw);
    }

    int smartWindow::get_maxX()
    {
        return this->getX() + this->get_width() - 1;
    }

    int smartWindow::get_maxY()
    {
        return this->getY() + this->get_height() - 1;
    }

    XY smartWindow::getXY()
    {
        return XY(this->getX(), this->getY());
    }

    XY smartWindow::getWH()
    {
        return XY(this->get_width(), this->get_height());
    }

    XY smartWindow::getmaxXY()
    {
        return XY(this->get_maxX(), this->get_maxY());
    }

    chtype smartWindow::getcorner(int x, int y, bool U, bool B, bool L, bool R)
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
        case 0b1100:
            result = ACS_VLINE;
            break;
        case 0b0011:
            result = ACS_HLINE;
            break;
        default:
            return 0;
            break;
        }

        return result;
    }

    void smartWindow::draw_borders()
    {
        chtype ul = getcorner(this->getX(), this->getY(), false, true, false, true);
        chtype ur = getcorner(this->get_maxX(), this->getY(), false, true, true, false);
        chtype bl = getcorner(this->getX(), this->get_maxY(), true, false, false, true);
        chtype br = getcorner(this->get_maxX(), this->get_maxY(), true, false, true, false);

        wattron(this->raw, COLOR_PAIR(1)); // defenitly need to do something with this hardcoded colors
        wborder(this->raw, 0, 0, 0, 0, ul, ur, bl, br);
        wattroff(this->raw, COLOR_PAIR(1));

        if (this->auto_refresh)
            wrefresh(this->raw);
    }

    void smartWindow::draw_borders_no_bottom()
    {
        wattron(this->raw, COLOR_PAIR(1));
        wborder(this->raw, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wattroff(this->raw, COLOR_PAIR(1));

        chtype ul = getcorner(this->getX(), this->getY(), false, true, false, true);
        chtype ur = getcorner(this->get_maxX(), this->getY(), false, true, true, false);
        chtype bl = getcorner(this->getX(), this->get_maxY(), true, false, false, false);
        chtype br = getcorner(this->get_maxX(), this->get_maxY(), true, false, false, false);

        wattron(this->raw, COLOR_PAIR(1));
        wborder(this->raw, 0, 0, 0, ' ', ul, ur, bl, br);
        wattroff(this->raw, COLOR_PAIR(1));

        if (this->auto_refresh)
            wrefresh(this->raw);
    }

    void smartWindow::delete_borders(bool smart)
    {
        chtype ul = getcorner(this->getX(), this->getY(), false, true, false, true);
        chtype ur = getcorner(this->get_maxX(), this->getY(), false, true, true, false);
        chtype bl = getcorner(this->getX(), this->get_maxY(), true, false, false, true);
        chtype br = getcorner(this->get_maxX(), this->get_maxY(), true, false, true, false);

        wattron(this->raw, COLOR_PAIR(1));
        wborder(this->raw, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wattroff(this->raw, COLOR_PAIR(1));

        if (smart)
        {
            for (auto i = this->neighbours.begin(); i != this->neighbours.end(); i++)
            {
                auto temp = (*i).lock();

                if (temp)
                {
                    temp->draw_borders();
                    temp->refresh();
                }
                else
                {
                    auto del = i;
                    i--;
                    this->neighbours.erase(del);
                }
            }
        }

        if (this->auto_refresh)
            wrefresh(this->raw);
    }

    void smartWindow::flush()
    {
        wclear(this->raw);
        wbkgd(this->raw, COLOR_PAIR(1));

        if (this->auto_refresh)
            wrefresh(this->raw);
    }

    void smartWindow::set_auto_refresh(bool flag)
    {
        this->auto_refresh = flag;
    }

    string smartWindow::supress(string str, unsigned int to)
    {
        if (str.size() <= to)
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

    void smartWindow::print(const std::string &mes)
    {
        wattron(this->raw, COLOR_PAIR(1));
        mvwprintw(this->raw, 1, 1, "%s", this->supress(mes, this->get_width() - 2).c_str());
        wattroff(this->raw, COLOR_PAIR(1));

        if (auto_refresh)
            this->refresh();
    }

    void smartWindow::refresh()
    {
        wrefresh(this->raw); // case all heirs have common memory space there is no need in recursive refresh call (i guess);
    }

    void smartWindow::registrate_neighbour(weak_ptr<smartWindow> neighbour)
    {
        this->neighbours.emplace_back(neighbour);
    }

    weak_ptr<smartWindow> smartWindow::create(smartWindow::Creator& builder)
    {
        return builder.create(*this);
    }

    smartWindow::~smartWindow()
    {
        while (heirs.size())
            heirs.pop_back();
        this->flush();
        this->delete_borders(true);
        if (this->auto_refresh)
            this->refresh();
        delwin(this->raw);
    }

    smartWindow::Creator::Creator(int x, int y, int width, int height, const list<weak_ptr<smartWindow>> &neighbours)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->neighbours = neighbours;
    }

    weak_ptr<smartWindow> smartWindow::Creator::create(smartWindow &parent)
    {
        auto win = shared_ptr<smartWindow>(new smartWindow(x, y, width, height, parent));
        this->registrate_heir(parent, win);
        return win;
    }

    void smartWindow::Creator::registrate_heir(smartWindow &parent, shared_ptr<smartWindow> &heir)
    {
        parent.heirs.push_back(heir);
    }

    bool smartWindow::delete_(std::weak_ptr<smartWindow> win)
    {
        for (auto i = heirs.begin(); i != heirs.end(); i++)
        {
            if (win.lock() == *i)
            {
                heirs.erase(i);
                return true;
            }
        }

        return false;
    }

    std::optional<weak_ptr<smartWindow>> smartWindow::get(int num)
    {
        auto temp = heirs.begin();
        for (int i = 0; i < num; i++)
        {
            if (temp == heirs.end())
                return std::nullopt;
            temp++;
        }

        return std::optional<weak_ptr<smartWindow>>(*temp);
    }

}