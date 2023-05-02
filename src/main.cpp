#include "UI.hpp"

const std::filesystem::path OS_BASE_DIR = std::filesystem::path("/"); 
std::filesystem::path LAUNCH_DIR;

std::string& extract_dir(std::string& path);

int main(int argc, char* argv[])
{
    std::string dir(argv[0]);
    dir = extract_dir(dir);
    LAUNCH_DIR = std::filesystem::path(dir);

    auto UI_ = YAExplorer::UI::GetInstance();

    UI_->setStatus("test message");

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

