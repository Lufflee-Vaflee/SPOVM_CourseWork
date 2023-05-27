#include "UI.hpp"
#include "smart_window.hpp"
#include "panel_manager.hpp"
#include "filesystem"
#include <iostream>
#include "dir_window.hpp"

std::string& extract_dir(std::string& path)
{
    auto temp = --path.end();
    while((*temp) != '/')
    {
        path.erase(temp);
        temp--;
    }
    path.erase(temp);

    return path;
}

namespace YAExplorer
{
    std::filesystem::path LAUNCH_DIR;
    std::filesystem::path ROOT_DIR;
}

using namespace YAExplorer;

int main(int argc, char* argv[])
{
    std::string dir(argv[0]);
    dir = extract_dir(dir);
    YAExplorer::LAUNCH_DIR = std::filesystem::path(dir);
    YAExplorer::ROOT_DIR = YAExplorer::LAUNCH_DIR.root_directory();
    



    auto &UI_ = YAExplorer::UI::instance();

    auto& MAIN = smartWindow::main;

    int half_width = MAIN.get_width() / 2;

    auto temp = panelManager::Creator(0, 1, half_width, MAIN.get_height() - 1);
    auto left = MAIN.create(temp);
    temp = panelManager::Creator(half_width - 1, 1, MAIN.get_width() - half_width + 1, smartWindow::main.get_height() - 1);
    auto right = MAIN.create(temp);



    //left.lock()->draw_borders();
    //left.lock()->refresh();

    //right.lock()->draw_borders();
    //right.lock()->refresh();

    getch();

    return 0;
}


