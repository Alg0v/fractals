#include "display/GUI.h"
#include "display/launcher.cpp"

int main() {
    launcher();
    GUI::init();
    GUI::run();
    GUI::destroy();
    return 0;
}
