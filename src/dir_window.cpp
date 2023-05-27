#include "dir_window.hpp"
#include <cmath>
#include <sstream>
#include <algorithm>
#include <time.h>

#define TYPE_SPACE 5
#define CHANGE_SPACE 9
#define SIZE_SPACE 8
#define MIN_WIDTH TYPE_SPACE + CHANGE_SPACE + SIZE_SPACE + 8

namespace YAExplorer
{

dirWindow::dirWindow(int x, int y, int width, int height, const smartWindow& parent, std::filesystem::path catalog, const list<weak_ptr<smartWindow>>& neighbours)
    : smartWindow(x, y, width, height, parent, neighbours)
{
    //if(!std::filesystem::is_directory(catalog))
        //throw something

    this->catalog = catalog;
    draw_head();

}

void dirWindow::draw_head()
{
    smartWindow::Creator builder(0, 0, this->get_width(), 2);
    auto head = this->create(builder);
                                        // type - file dir link unknown  5 chracters space
                                        // size - 1 - 1023 [k/g/t]B - 8 characters
                                        // change - xx:xx:xx - 9 charachters
                                        // name - width - (5 + 8 + 9) = width - 22
    head.lock()->print("size", 1, this->get_width() - TYPE_SPACE - 1);
    head.lock()->print("change", 1, this->get_width() - CHANGE_SPACE - TYPE_SPACE - 1);
    head.lock()->print("type", 1, this->get_width() - CHANGE_SPACE - TYPE_SPACE - SIZE_SPACE - 1);
    head.lock()->print("name", 1, 1);

    head.lock()->refresh();
}

dirWindow::Creator::Creator(int x, int y, int width, int height, std::filesystem::path catalog, const list<weak_ptr<smartWindow>>& neighbours)
    : smartWindow::Creator(x, y, width, height, neighbours)
{
    this->catalog = catalog;
}

weak_ptr<smartWindow> dirWindow::Creator::create(smartWindow& parent)
{
    shared_ptr<smartWindow> win = std::dynamic_pointer_cast<smartWindow>(std::shared_ptr<dirWindow>(new dirWindow(x, y, width, height, parent, catalog, neighbours)));
    this->registrate_heir(parent, win);
    return win;
}

int countType(const std::filesystem::directory_entry& a)
{
    if (std::filesystem::is_directory(a.path()))
    {
        return 0;
    }
    else if (std::filesystem::is_regular_file(a.path()))
    {
        return 1;
    }
    else if (std::filesystem::is_symlink(a.path()))
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

void dirWindow::update_model()
{
    for (const auto& entry : std::filesystem::directory_iterator(this->catalog))
        entries.push_back(entry);
}

void dirWindow::sort(sortOrder order, bool update)
{
    this->order = order;
    if(update)
        update_model();

    switch (order)
    {
    case sortOrder::none :
        return;
    case sortOrder::name :
        std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) 
        {
            return a.path().filename().string() < b.path().filename().string();
        });
        return;
    case sortOrder::type : 
        std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) 
        {
            return countType(a) < countType(b);
        });
        return;
    case sortOrder::change :
        std::sort(entries.begin(), entries.end(), [](const auto & lhs, const auto & rhs) 
        {
            return std::filesystem::last_write_time(lhs) > std::filesystem::last_write_time(rhs);
        });
    case sortOrder::size :
        std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) 
        {
            int A = 0, B = 0;
            if (std::filesystem::is_regular_file(a.path()))
                A = std::filesystem::file_size(a.path());

            if (std::filesystem::is_regular_file(b.path()))
                B = std::filesystem::file_size(b.path());

            return A < B;
        });
        return;
    default:
        // throw unknown sortOrder
        return;
    }
}

dirWindow::sortOrder dirWindow::get_order()
{
    return this->order;
}

int dirWindow::get_space()
{
    return this->get_height() - 2 - 1; // - borders - head; 
}

int dirWindow::count_screens(bool update)
{
    if(update)
    {
        this->update_model();
        this->sort(this->order);
    }

    return this->entries.size() / this->get_space() + 1;
}

struct HumanReadable 
{
    std::uintmax_t size{};
    private: friend
    std::ostream& operator<<(std::ostream& os, HumanReadable hr) 
    {
        int i{};
        double mantissa = hr.size;
        for (; mantissa >= 1024.; mantissa /= 1024., ++i) { }
        mantissa = std::ceil(mantissa * 10.) / 10.;
        os << mantissa << "BKMGTPE"[i];
        return i == 0 ? os : os << "B (" << hr.size << ')';
    }
};

std::string readable_size(std::filesystem::directory_entry entry)
{
    std::uintmax_t size = entry.file_size();
    std::stringstream result;
    int i = 0;
    double mantissa = size;

    for (; mantissa >= 1024.; mantissa /= 1024., ++i);

    mantissa = std::ceil(mantissa * 10.) / 10.;
    result << mantissa;
    std::string cut = result.str().substr(SIZE_SPACE - 1);
    result.flush();
    result << cut << "BKMGTPE"[i];
    if(i != 0)
        result << "B";
    return result.str();
}

std::string readable_time(std::filesystem::directory_entry entry)
{
    
}

void dirWindow::draw_entry(std::filesystem::directory_entry entry, int position)
{
    smartWindow::Creator builder(0, 0, this->get_width(), 2);
    auto head = this->create(builder);
                                        // type - file dir link unknown  5 chracters space
                                        // size - 1 - 1023 [k/g/t]B - 8 characters
                                        // change - xx:xx:xx - 9 charachters
                                        // name - width - (5 + 8 + 9) = width - 22

    head.lock()->print(readable_size(entry), 1, this->get_width() - TYPE_SPACE - 1);
    head.lock()->print("change", 1, this->get_width() - CHANGE_SPACE - TYPE_SPACE - 1);
    head.lock()->print("type", 1, this->get_width() - CHANGE_SPACE - TYPE_SPACE - SIZE_SPACE - 1);
    head.lock()->print("name", 1, 1);

    head.lock()->refresh();
}



















/*
int dirWindow::get_space()
{
    return (this->get_height() - 2 - 1); // height - borders - head
}

int dirWindow::count_screens()
{
    int count = 0;
    auto i = this->catalog.begin();
    i++; // skip [.];
    for(auto i = this->catalog.begin(); i != this->catalog.end(); i++)
        count++;

    return count / get_space() + 1;
}
*/


/*void dirWindow::draw_string(std::filesystem::directory_iterator str, int position)
{
    //if(position < 0 || position >= this->get_width() - 2)
        //throw something;
    position += 2;

    smartWindow::Creator builder(0, position, this->get_width(), 1);
    auto win = this->create(builder);
                                                        // type - file dir link unknown  5 chracters space
                                                        // size - 1 - 1023 [k/g/t]B - 8 characters
                                                        // change - xx:xx:xx - 9 charachters
                                                        // name - width - (5 + 8 + 9) = width - 22

    win.lock()->print("size", 0, this->get_width() - TYPE_SPACE - 1);
    win.lock()->print("change", 0, this->get_width() - CHANGE_SPACE - TYPE_SPACE - 1);
    win.lock()->print("type", 0, this->get_width() - CHANGE_SPACE - TYPE_SPACE - SIZE_SPACE - 1);
    win.lock()->print((*str).path().filename().c_str(), 0, 1);

    win.lock()->refresh();
}

bool dirWindow::redraw_dir(int screen_num)
{
    if(screen_num < 0 || screen_num > this->count_screens())
        return false;

    int count = 0;
    auto temp = std::filesystem::directory_iterator(this->catalog);
    for(; (*temp). && count <= get_space() * screen_num; temp++) 
    {
        count++;
    }

    int position = 0;
    for(int i = 0; i < get_space() && temp != catalog.end(); i++)
    {
        this->draw_string(temp, position);
        position++;
        temp++;
    }

    this->refresh();

    return true;
}*/





};