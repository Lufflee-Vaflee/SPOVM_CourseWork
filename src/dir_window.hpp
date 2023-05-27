#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>

#include "smart_window.hpp"
#include "entry_window.hpp"


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

    std::vector<std::weak_ptr<entryWindow>> view;
    int current_screen = 0;
    int selected_entry = 0;

    void flush_view();

    public:

    void sort(sortOrder order, bool update = true);
    sortOrder get_order();
    void update_model();

    int get_space();
    int count_screens(bool update_model = false);

    void draw_head();
    bool redraw(int screen_num = 0, bool update = false);

    virtual ~dirWindow() = default;

    class Creator : public smartWindow::Creator
    {
        public:

        Creator(int x, int y, int width, int height, std::filesystem::path catalog, const list<weak_ptr<smartWindow>>& neighbours = list<weak_ptr<smartWindow>>());

        protected:

        std::filesystem::path catalog;

        public:

        virtual weak_ptr<smartWindow> create(smartWindow& parent) override;

        virtual ~Creator() = default;
    };

};

};
