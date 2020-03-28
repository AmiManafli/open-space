#ifndef CG1_PROJECT_RENDERWINDOW_H
#define CG1_PROJECT_RENDERWINDOW_H

#include <GLHeader.h>

class RenderWindow {
public:
    RenderWindow(std::string title, uint16_t width, uint16_t height);

    bool isOpen();

private:
    const std::string title;
    uint16_t width;
    uint16_t height;
};


#endif //CG1_PROJECT_RENDERWINDOW_H
