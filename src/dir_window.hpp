#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>

#include "smart_window.hpp"


namespace YAExplorer
{

class smartWindow;

class dirWindow : public smartWindow
{
    enum sortOrder
    {
        none,
        name,
        type,
        size,
        change
    };

    protected:

    dirWindow(int x, int y, int width, int height, const smartWindow& parent, std::filesystem::path catalog, const list<weak_ptr<smartWindow>>& neighbours);

    std::filesystem::path catalog;
    sortOrder order = sortOrder::none;
    std::vector<std::filesystem::directory_entry> entries;
    int current_screen = 0;

    public:

    void sort(sortOrder order, bool update = true);
    sortOrder get_order();
    void update_model();

    int get_space();
    int count_screens(bool update = false);

    void draw_head();
    void draw_entry(std::filesystem::directory_entry str, int position);
    bool redraw(int screen_num = 0);

    class Creator : public smartWindow::Creator
    {
        public:

        Creator(int x, int y, int width, int height, std::filesystem::path catalog, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

        protected:

        std::filesystem::path catalog;

        public:

        virtual weak_ptr<smartWindow> create(smartWindow& parent) override;

        ~Creator() = default;
    };

    virtual ~dirWindow() = default;
};

};
