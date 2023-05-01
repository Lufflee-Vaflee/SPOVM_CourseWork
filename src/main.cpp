#include "UI.hpp"



int main()
{
    auto UI_ = YAExplorer::UI::GetInstance();

    UI_->setStatus("test message");

    getch();

    UI_->CloseInstance();
    return 0;
}

