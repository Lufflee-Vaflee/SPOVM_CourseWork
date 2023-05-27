#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <cmath>

#include "smart_window.hpp"

#define TYPE_SPACE 6
#define CHANGE_SPACE 17
#define SIZE_SPACE 8
#define MIN_WIDTH TYPE_SPACE + CHANGE_SPACE + SIZE_SPACE + 3

namespace YAExplorer
{

class entryWindow : public smartWindow
{
    protected:

    entryWindow(int x, int y, int width, int height, std::filesystem::directory_entry entry, smartWindow& parent = main, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

    std::filesystem::directory_entry entry;

    private:

    std::string readable_size();
    std::string readable_time();
    std::string readable_type();

    public:

    bool redraw();
    std::filesystem::directory_entry get_entry();

    //void mark_as_chosen(); // navigation feachurel

    class Creator : public smartWindow::Creator
    {
        public:

        Creator(int x, int y, int width, int height, std::filesystem::directory_entry entry, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

        protected:

        std::filesystem::directory_entry entry;

        public:

        virtual weak_ptr<smartWindow> create(smartWindow& parent) override;

        ~Creator() = default;
    };

    virtual ~entryWindow() = default;
};

}