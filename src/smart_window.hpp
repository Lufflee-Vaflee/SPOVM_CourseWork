#pragma once

#include <ncurses.h>
#include <list>
#include <iostream>
#include <memory>

#include "UI.hpp"

#define MIN_WINDOW_HEIGHT 3
#define MIN_WINDOW_WIDTH 3

namespace YAExplorer
{

class mainWin;

class smartWindow;

typedef smartWindow SWindow;

typedef std::pair<int, int> XY;

using namespace std;

class smartWindow
{
    private:

    smartWindow();  //special constructor for main window

    protected:

    smartWindow(int x, int y, int width, int height, const smartWindow& parent = main, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

    smartWindow(smartWindow &other) = delete;
    void operator=(const smartWindow&) = delete;

    public:

    static smartWindow main;

    smartWindow(smartWindow&& other);

    private:

    bool auto_refresh = true;

    list<weak_ptr<smartWindow>> neighbours;

    list<shared_ptr<smartWindow>> heirs = list<shared_ptr<smartWindow>>();

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

    virtual void draw_borders();
    void draw_borders_no_bottom();
    void delete_borders(bool smart = true);
    void flush();

    void refresh();
    void print(const std::string& mes); // add more complex parameters

    void set_auto_refresh(bool refresh);

    void registrate_neighbour(weak_ptr<smartWindow> neighbour);
    bool delete_neighbour(weak_ptr<smartWindow> neighbour);

    virtual ~smartWindow();
    class Creator              //fabrick
    {
        public:

        Creator(int x, int y, int width, int height, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

        protected:

        int x, y, width, height;
        list<weak_ptr<smartWindow>> neighbours;

        void registrate_heir(smartWindow& parent, shared_ptr<smartWindow>& heir);

        public:

        virtual weak_ptr<smartWindow> create(smartWindow& parent);

        ~Creator() = default;
    };

    weak_ptr<smartWindow> create(smartWindow::Creator builder);
};




// this is just an example of normal way to inherite smartWindow class
/*
class anotherWin : public smartWindow
{
    protected:
    anotherWin(int another_value, int x, int y, int width, int height, const smartWindow& parent = smartWindow::main, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>())
        : smartWindow(x, y, width, height, parent, neighbours)
    {

    }

    public:
    class Creator : public smartWindow::Creator              //fabrick
    {
        int another_value;

        public:

        Creator(int another_value, int x, int y, int width, int height, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>())
            : smartWindow::Creator(x, y, width, height, neighbours)
        {
        }

        virtual weak_ptr<smartWindow> create(smartWindow& parent) override
        {
            shared_ptr<smartWindow> win = std::dynamic_pointer_cast<smartWindow>(std::shared_ptr<anotherWin>(new anotherWin(0, x, y, width, height, parent, neighbours)));
            this->registrate_heir(parent, win);
            return win;
        }

        ~Creator()
        {

        }
    };
};
*/


}