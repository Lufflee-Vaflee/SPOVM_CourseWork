#pragma once

#include "stuff.hpp"
#include <ncurses.h>
#include <list>
#include <functional>
#include <iostream>
#include <memory>

#define MIN_WINDOW_HEIGHT 3
#define MIN_WINDOW_WIDTH 3

namespace YAExplorer
{

class smartWindow;

typedef smartWindow SWindow;

typedef std::pair<int, int> XY;

using namespace std;

class smartWindow
{
    public:

    static const weak_ptr<smartWindow> main_ptr;

    private:

    static const shared_ptr<smartWindow> main;

    smartWindow();      //special constructor for main window

    smartWindow(int x, int y, int width, int height, weak_ptr<smartWindow> parent = main, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

    smartWindow(smartWindow &other) = delete;
    void operator=(const smartWindow&) = delete;

    smartWindow(smartWindow&& other);

    bool auto_refresh = true;

    list<weak_ptr<smartWindow>> neighbours;
    list<shared_ptr<smartWindow>> heirs = list<shared_ptr<smartWindow>>();

    weak_ptr<smartWindow> selfref;

    WINDOW* raw; //raw window pointer to interract with ncurses lib

    chtype getcorner(int x, int y, bool U, bool B, bool L, bool R);
    std::string supress(std::string str, unsigned int to);

    public:

    int getX();
    int getY();
    int get_width();
    int get_height();
    int get_maxX(); 
    int get_maxY();
    XY  getXY();
    XY  getWH();
    XY  getmaxXY();

    void draw_borders();
    void draw_borders_no_bottom();
    void delete_borders(bool smart = true);
    void flush();

    void refresh();
    void print(const std::string& mes); // add more complex parameters

    void set_auto_refresh(bool refresh);

    void registrate_neighbour(weak_ptr<smartWindow> neighbour);
    bool delete_neighbour(weak_ptr<smartWindow> neighbour);

    weak_ptr<smartWindow> create_heir(int x, int y, int width, int height, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

    ~smartWindow();
};



}