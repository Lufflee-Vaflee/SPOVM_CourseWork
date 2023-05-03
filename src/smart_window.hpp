#pragma once

#include "stuff.hpp"
#include <ncurses.h>
#include <vector>
#include <functional>
#include <iostream>
#include <memory>

#define MIN_WINDOW_HEIGHT 3;
#define MIN_WINDOW_WIDTH 3;

namespace YAExplorer
{

class smartWindow;
typedef smartWindow SWindow;

using namespace std;

class smartWindow
{
    private:

    smartWindow()       //special constructor for main window
    {

    }

    public:

    smartWindow(int x, int y, int width, int height, weak_ptr<smartWindow> parent = main_ptr, const vector<weak_ptr<smartWindow>>& neighbours = std::vector<weak_ptr<smartWindow>>())
    {
        
    }

    static const shared_ptr<smartWindow> main_ptr;

    private:

    vector<weak_ptr<smartWindow>> neighbours;
    shared_ptr<smartWindow> current;

    public:

    int getX();
    int getY();
    int get_width();
    int get_height();
    int get_maxX();
    int get_maxY();

    void draw_borders();
    void delete_borders();


    ~smartWindow()
    {

    }

};



}