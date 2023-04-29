#include "UI.hpp"



int main()
{
    auto UI_ = YAExplorer::UI::GetInstance();

    char* str;

    getch();

    UI_->CloseInstance();
    return 0;
}

