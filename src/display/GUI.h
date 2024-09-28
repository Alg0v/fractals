//
// Created by algov on 8/28/24.
//

#ifndef UNTITLED_GUI_H
#define UNTITLED_GUI_H
#include <SDL2/SDL.h>
#include "../math/complex.h"

namespace GUI {
    extern SDL_Window * window;
    extern SDL_Renderer * renderer;
    extern SDL_Texture * frame;
    extern SDL_Event event;
    extern SDL_Rect window_rect;
    extern bool mousedown;
    extern SDL_Point mouse_start;

    extern void init();
    extern void init_window_size();
    extern void destroy();
    extern void run();
    extern void render(SDL_Rect to_render={0, 0, window_rect.w, window_rect.h}, bool multithread=true);
    extern int render_t(void * data);
    extern void zoom(const long& k);

    extern int * pixels_buffer;
    extern double offsetX;
    extern double offsetY;
    extern long _zoom;

    extern std::pair<uint8_t, double> (*algorithm) (const math::Complex&, const uint8_t& iterations);
    extern int (*coloring) (const std::pair<uint8_t, double>& data);

    extern bool isFullScreen;
    extern bool quit;
    extern bool request_update;
    extern int threads_running;
    extern bool is_zooming;
    extern float zoom_factor;
}

#endif //UNTITLED_GUI_H
