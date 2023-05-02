#pragma once

#include <ncurses.h>

#include <exception>
#include <cstring>
#include <string>

#include "panel_manager.hpp"

namespace YAExplorer
{

#define curs_invisible 0
#define curs_normal 1
#define curs_visible 2

#define APP_NAME "Yet Another Explorer"

class UI            //singleton
{
    protected:

    UI();

    static UI* instance;

    static WINDOW* up_bar;             //in-fact all if them are static, its not marked by static cause their initialization should be after UI constructor;
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

    static bool changeSide(); // 0 - left, 1 - right;


    private:

    ~UI();
};




}