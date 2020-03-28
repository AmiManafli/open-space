#include "RenderWindow.h"

RenderWindow::RenderWindow(std::string title, uint16_t width, uint16_t height)
        : title(title), width(width), height(height) {
}

bool RenderWindow::isOpen() {
    return false;
}
