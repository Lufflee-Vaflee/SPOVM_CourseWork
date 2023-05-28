#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <cmath>

#include "smart_window.hpp"

namespace YAExplorer
{

class editLine : public smartWindow
{
    protected:

    editLine(int x, int y, int width, std::string _default = "", const smartWindow& parent = main, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

    std::string model;
    int model_display_start = 0;
    int cursor_position = 0;

    public:

    int move_cursor_left();
    int move_cursor_right();
    int erase();
    int insert(char c);
    void redraw();

    std::string operator()();

    class Creator : public smartWindow::Creator              //fabrick
    {
        public:

        Creator(int x, int y, int width, std::string _default = "", const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

        protected:
        std::string _default;

        public:

        virtual weak_ptr<smartWindow> create(smartWindow& parent) override;

        virtual ~Creator() = default;
    };

    virtual ~editLine();
};

}