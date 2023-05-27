#pragma once

#include <ncurses.h>

#include <exception>
#include <string>
#include <list>
#include <filesystem>
#include <cstring>
#include <tuple>

#include "smart_window.hpp"
#include "dir_window.hpp"

#define TAB_SIZE_MAX 30
#define NEW_TAB  "NEW TAB"

namespace YAExplorer
{

class smartWindow;

//i guess i need better name for this class
//this thing should manage all tabs of left or ride panels of app
//it needs to:
//adding new tabs
//removing tabs
//changing tabs positions
//display correctly upbar representing all tabs with acsent on current chosed tab

//rules:
//its always display one special tab representing new tab button at the end of tab bar
//current tab always have TAB_SIZE_MAX while other tabs must shrink corresponding to panel width
//add check for allowoble minimal panel size - ? 

extern std::filesystem::path LAUNCH_DIR;

class panelManager : public smartWindow
{
    protected:

    panelManager(int x, int y, int width, int height, const smartWindow& parent = main, std::filesystem::path _default = LAUNCH_DIR, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

    panelManager(panelManager &other) = delete;
    void operator=(const panelManager&) = delete;

    private:

    std::list<std::string> tabs;
    std::list<weak_ptr<smartWindow>> view;
    std::list<std::string>::iterator cur_tab;

    weak_ptr<dirWindow> body; 

    int tabs_size = TAB_SIZE_MAX;

    void registrate_tab(std::string name);                      //that thing only REGISTER, NOT DRAWING new tab, only for internal class use
    void remove_tab(std::list<std::string>::iterator removed);          //most likely deleting tab will be current chosen, os iterator will be good idea
    void set_current(std::list<std::string>::iterator current);
    void redraw_tabs();                                         //as every possible change will defenitly cause all tabs structure is easiear to redraw all tabs, only for internal use
    void flush_tabs();
    void switch_positions(std::list<std::string>::iterator from, std::list<std::string>::iterator to);

    public:

    //here should be interaction interface

    virtual ~panelManager() = default;

    class Creator : public smartWindow::Creator              //fabrick
    {
        public:

        Creator(int x, int y, int width, int height, std::filesystem::path _default = LAUNCH_DIR, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

        protected:

        std::filesystem::path _default;

        public:

        virtual weak_ptr<smartWindow> create(smartWindow& parent) override;

        ~Creator() = default;
    };
};

};