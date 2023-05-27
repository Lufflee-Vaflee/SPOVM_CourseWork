#include "entry_window.hpp"

namespace YAExplorer
{

entryWindow::entryWindow(int x, int y, int width, int height, std::filesystem::directory_entry entry,smartWindow& parent, const list<weak_ptr<smartWindow>>& neighbours)
    : smartWindow(x, y, width, height, parent, neighbours)
{
    this->entry = entry;
    //if(!this->redraw())
        //throw something
}

entryWindow::Creator::Creator(int x, int y, int width, int height, std::filesystem::directory_entry entry, const list<weak_ptr<smartWindow>>& neighbours)
    : smartWindow::Creator::Creator(x, y, width, height, neighbours)
{
    this->entry = entry;
}

std::string entryWindow::readable_size()
{
    std::uintmax_t size = this->entry.file_size();
    std::stringstream result;
    int i = 0;
    double mantissa = size;

    for (; mantissa >= 1024.; mantissa /= 1024., ++i);

    mantissa = std::ceil(mantissa * 10.) / 10.;
    auto temp = result.str();
    result << mantissa;
    result << "BKMGTPE"[i];
    temp = result.str();
    if(i != 0)
        result << "B";
    temp = result.str();
    return result.str();
}

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
}

std::string entryWindow::readable_time()
{
    time_t t = to_time_t(entry.last_write_time());
    tm time = *localtime(&t);

    int year = time.tm_year + 1900;
    int month = time.tm_mon + 1;
    int day = time.tm_mday;

    int hour = time.tm_hour;
    int minute = time.tm_min;

    std::stringstream readable;
    std::stringstream temp;

    // 2 + 1 + 2 + 1 + 4 + 1 + 2 + 1 + 2  + 1 = 17 symb
    readable << std::setfill('0');
    readable << std::setw(2) <<  day << ":";
    readable << std::setw(2) << month << ":" << year << " ";
    readable << std::setw(2) << hour << ":";
    readable << std::setw(2) << minute;
    return readable.str();
}

std::string entryWindow::readable_type()
{
    if (entry.is_directory())
    {
        return "dir";
    }
    else if (entry.is_regular_file())
    {
        return "file";
    }
    else if (entry.is_symlink())
    {
        return "link";
    }
    else
    {
        return "other";
    }
}

bool entryWindow::redraw()
{
    if(!this->entry.exists())
        return false;

    // type - file dir link unknown  5 chracters space
    // size - 1 - 1023 [k/g/t]B - 8 characters
    // change - xx:xx:xx - 9 charachters
    // name - width - (5 + 8 + 9) = width - 22

    if(this->entry.is_regular_file())
        this->print(readable_size(), 0, this->get_width() - SIZE_SPACE - 1);
    this->print(readable_time(), 0, this->get_width() - CHANGE_SPACE - SIZE_SPACE - 1);
    this->print(readable_type(), 0, this->get_width() - CHANGE_SPACE - SIZE_SPACE - TYPE_SPACE - 1);
    this->print(this->entry.path().filename(), 0, 0, this->get_width() - MIN_WIDTH);
    this->refresh();

    return true;
}

std::filesystem::directory_entry entryWindow::get_entry()
{
    return this->entry;
}

weak_ptr<smartWindow> entryWindow::Creator::create(smartWindow& parent)
{
    shared_ptr<smartWindow> win = std::dynamic_pointer_cast<smartWindow>(std::shared_ptr<entryWindow>(new entryWindow(x, y, width, height, entry, parent, neighbours)));
    this->registrate_heir(parent, win);
    return win;
}


}

