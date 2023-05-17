#pragma once

#include <ncurses.h>

#include <exception>
#include <cstring>
#include <string>

#include "panel_manager.hpp"
#include "smart_window.hpp"
#include "stuff.hpp"

namespace YAExplorer
{

#define curs_invisible 0
#define curs_normal 1
#define curs_visible 2

class smartWindow;

#define APP_NAME "Yet Another Explorer"

class UI               //singleton
{
    private:

    UI();

    public:

    UI(UI &other) = delete;
    void operator=(const UI&) = delete;

    static UI& instance()
    {
        static UI inst;
        return inst;
    }

    ~UI();
};




}