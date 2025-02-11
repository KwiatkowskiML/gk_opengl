#include "includes/Renderer.h"

extern int LibMain()
{
    try {
        Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
        renderer.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
