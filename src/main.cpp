#include "UI.hpp"
#include "smart_window.hpp"
#include "filesystem"

std::string& extract_dir(std::string& path);

namespace YAExplorer
{
    std::filesystem::path LAUNCH_DIR;
    std::filesystem::path ROOT_DIR;
}

int main(int argc, char* argv[])
{
    std::string dir(argv[0]);
    dir = extract_dir(dir);
    YAExplorer::LAUNCH_DIR = std::filesystem::path(dir);
    YAExplorer::ROOT_DIR = YAExplorer::LAUNCH_DIR.root_directory();

    auto UI_ = YAExplorer::UI::GetInstance();

    //auto win = UI_->create_heir(10, 10, 30, 30);
    //win.lock()->draw_borders();

    getch();

    UI_->CloseInstance();
    return 0;
}

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

