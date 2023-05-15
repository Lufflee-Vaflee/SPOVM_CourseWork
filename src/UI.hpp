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

class UI : public smartWindow               //singleton
{
    private:

    UI();

    static UI* instance;

    static WINDOW* up_bar;
    static WINDOW* status_bar;

    static panelManager* left;
    static panelManager* right;

    static std::string status;

    public:

    UI(UI &other) = delete;
    void operator=(const UI&) = delete;

    static UI* GetInstance();
    static void CloseInstance();

    static void setStatus(std::string message);
    static std::string getStatus();

    //static bool changeSide(); // 0 - left, 1 - right;


    private:

    virtual ~UI();
};




}