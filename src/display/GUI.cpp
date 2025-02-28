//
// Created by algov on 8/28/24.
//

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "./GUI.h"
#include "../math/mandelbrot.h"
#include "../math/utils.h"
#include "../math/julia.h"
#include "./color/classic.h"

#define THREADS 12

namespace GUI {
    SDL_Window * window{nullptr};
    SDL_Renderer * renderer{nullptr};
    SDL_Texture * frame{nullptr};
    SDL_Event event;
    SDL_Rect window_rect;
    SDL_Point mouse_start;
    bool mousedown = false;

    int * pixels_buffer{nullptr};
    double offsetX = -4;
    double offsetY = -4;
    long _zoom = 100;
    uint8_t iterations = 64;

    std::pair<uint8_t, double> (*algorithm) (const math::Complex&, const uint8_t& iterations) = math::julia;
    int (*coloring) (const std::pair<uint8_t, double>& data) = color::classic;


    bool quit{false};
    bool request_update{false};
    bool isFullScreen{false};
    int threads_running = 0;
    bool is_zooming = false;
    float zoom_factor = 0;

    void init() {
        std::cout << "GUI::init() ";
        int exitcode = SDL_Init(SDL_INIT_VIDEO);
        if (exitcode < 0) {
            std::cout << "[ERROR: SDL2 failed to initialize]:\n";
            std::cout << SDL_GetError() << "\n";
            exit(1);
        }

        window = SDL_CreateWindow("Fractals", -1, -1, 800, 800, SDL_WINDOW_RESIZABLE);

        if (window == nullptr) {
            std::cout << "[ERROR: couldn't create a window]:\n";
            std::cout << SDL_GetError() << "\n";
            SDL_Quit();
            exit(2);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

        if (renderer == nullptr) {
            std::cout << "[ERROR: could't create the renderer]:\n";
            std::cout << SDL_GetError() << "\n";
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(3);
        }

        init_window_size();

        render();
        request_update = true;

        std::cout << "[OK]\n";
    }


    int render_t(void * data){
        SDL_Rect * rRect = (SDL_Rect *)data;

        printf("GUI::render(): Thread {%i, %i, %i, %i} started\n", rRect->x, rRect->y, rRect->w, rRect->h);

        for(int y = rRect->y; y < rRect->h + rRect->y; y++)
            for (int x = rRect->x; x < rRect->w + rRect->x; x++) {
                pixels_buffer[x + y * window_rect.w] = coloring(algorithm(math::Complex((double)(((double)x/(double)_zoom) + offsetX),(double)(((double)y/(double)_zoom) + offsetY)), iterations));
            }

        printf("GUI::render(): Thread {%i, %i, %i, %i} finished, requesting update\n", rRect->x, rRect->y, rRect->w, rRect->h);
        request_update = true;
        threads_running--;
        return 0;
    }

    void render(SDL_Rect to_render, bool multithread){
        if(multithread){
            while(threads_running) SDL_Delay(16);
            threads_running = THREADS;
            SDL_Thread * threads[THREADS];
            SDL_Rect * rects = (SDL_Rect *)malloc(sizeof(SDL_Rect) * THREADS);
            for(int i = 0; i < THREADS - 1; i++){
                rects[i] = to_render;
                rects[i].h = to_render.h/THREADS;
                rects[i].y = rects[0].h * i + to_render.y;
                threads[i] = SDL_CreateThread(render_t, nullptr, &rects[i]);
                SDL_DetachThread(threads[i]);
            }

            rects[THREADS - 1] = to_render;
            rects[THREADS - 1].y = rects[THREADS - 2].y + rects[THREADS - 2].h;
            rects[THREADS - 1].h = to_render.h - (THREADS - 1)*rects[0].h;
            threads[THREADS - 1] = SDL_CreateThread(render_t, nullptr, &rects[THREADS - 1]);
            SDL_DetachThread(threads[THREADS - 1]);
            SDL_Rect threads_rect = {0, 0, window_rect.w, 16};
            SDL_SetRenderDrawColor(renderer, 120, 120, 255, 255); // Indicateur de chargement
            SDL_RenderFillRect(renderer, &threads_rect);
            SDL_RenderPresent(renderer);
            while(threads_running) SDL_Delay(16);
            free(rects);
        } else {
            for (int y = to_render.y; y < to_render.h + to_render.y; y++)
                for (int x = to_render.x; x < to_render.w + to_render.x; x++) {
                    pixels_buffer[x + y * window_rect.w] = coloring(algorithm(
                            math::Complex((double) (((double) x / (double) _zoom) + offsetX),
                                          (double) (((double) y / (double) _zoom) + offsetY)), 64));
                }
        }

    }

    void init_window_size(){
        if(pixels_buffer != nullptr) free(pixels_buffer);

        SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);
        pixels_buffer = (int *) malloc(sizeof(int) * window_rect.w * window_rect.h);

        // render the frame
        if(frame != nullptr) SDL_DestroyTexture(frame);
        frame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, 0, window_rect.w, window_rect.h);
    }

    void zoom(const long& k){
        offsetX += ((double)window_rect.w/(double)_zoom-window_rect.w/(double)k)/2;
        offsetY += ((double)window_rect.h/(double)_zoom-window_rect.h/(double)k)/2;
        _zoom = k;
        render();
        request_update = true;
    }

    void run(){
        while(!quit){
            is_zooming = false;

            if(request_update == 0) {
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;
                        case SDL_WINDOWEVENT:
                            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                                printf("GUI::run(): Resized window to %d:%d\n", event.window.data1, event.window.data2);
                                window_rect.w = event.window.data1;
                                window_rect.h = event.window.data2;
                                init_window_size();
                                render();
                                request_update = true;
                            }
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if (!mousedown) {
                                mouse_start = {event.button.x, event.button.y};
                                mousedown = true;
                                //SDL_SetWindowMouseGrab(window, SDL_TRUE); gèle la fenêtre sur gnome wayland
                            }
                            break;
                        case SDL_MOUSEMOTION:
                            if (mousedown) {
                                SDL_Rect frame_rect = {-(mouse_start.x - event.motion.x),
                                                       -(mouse_start.y - event.motion.y), window_rect.w, window_rect.h};
                                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                                SDL_RenderClear(renderer);
                                SDL_RenderCopyEx(renderer, frame, nullptr, &frame_rect, 0, nullptr,
                                                 (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));
                                SDL_RenderPresent(renderer);
                            }
                            break;
                        case SDL_MOUSEBUTTONUP:
                            if (mousedown) {
                                mousedown = false;
                                SDL_SetWindowMouseGrab(window, SDL_FALSE);
                                int deltaX = event.motion.x - mouse_start.x;
                                int deltaY = event.motion.y - mouse_start.y;
                                offsetX += deltaX / (double) _zoom;
                                offsetY += deltaY / (double) _zoom;
                                SDL_Rect to_render_v = {0, 0, abs(deltaX), window_rect.h - abs(deltaY)};
                                SDL_Rect to_render_h = {0, 0, window_rect.w, abs(deltaY)};

                                if (deltaY > 0) {
                                    to_render_h.y = window_rect.h - deltaY;
                                    to_render_v.y = 0;
                                } else {
                                    to_render_v.y = abs(deltaY);
                                    to_render_h.y = 0;
                                }

                                if (deltaX > 0) {
                                    to_render_v.x = window_rect.w - deltaX;
                                    to_render_v.w = deltaX;
                                } else {
                                    to_render_v.x = 0;
                                    to_render_v.w = abs(deltaX);
                                }


                                int *_pixels_buf = (int *) malloc(sizeof(int) * window_rect.w * window_rect.h);
                                for (int y = math::max(deltaY); y < window_rect.h && y - deltaY < window_rect.h; y++)
                                    for (int x = math::max(deltaX);
                                         x < window_rect.w && x - deltaX < window_rect.w; x++) {
                                        _pixels_buf[math::max(x - deltaX) + window_rect.w * math::max(y - deltaY)] =
                                                pixels_buffer[x + window_rect.w * y];
                                    }
                                free(pixels_buffer);
                                pixels_buffer = _pixels_buf;
                                render(to_render_h);
                                render(to_render_v);
                                request_update = true;
                            }
                            break;

                        case SDL_MOUSEWHEEL:
                            is_zooming = true;
                            if (zoom_factor == 0) zoom_factor = 1;
                            if (event.wheel.y > 0) {
                                zoom_factor += .1;
                                SDL_Rect r = {(int) (window_rect.w * (1 - 1 / zoom_factor)) / 2,
                                              (int) (window_rect.h * (1 - 1 / zoom_factor)) / 2,
                                              (int) (window_rect.w / zoom_factor),
                                              (int) (window_rect.h / zoom_factor)};
                                SDL_RenderCopyEx(renderer, frame, &r, NULL, 0, NULL,
                                                 (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));
                            } else {
                                if (zoom_factor >= 0.2) zoom_factor -= .1;
                                SDL_Rect r = {(int) (window_rect.w * (1 - zoom_factor)) / 2,
                                              (int) (window_rect.h * (1 - zoom_factor)) / 2,
                                              (int) (window_rect.w * zoom_factor),
                                              (int) (window_rect.h * zoom_factor)};
                                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                                SDL_RenderClear(renderer);
                                SDL_RenderCopyEx(renderer, frame, NULL, &r, 0, NULL,
                                                 (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));
                            }
                            SDL_RenderPresent(renderer);
                            break;

                        case SDL_KEYDOWN:
                            switch (event.key.keysym.sym) {
                                case SDLK_UP:
                                    zoom(_zoom * 1.5);
                                    std::cout << "GUI::run(): zoomed to *" << _zoom << "\n";
                                    break;
                                case SDLK_DOWN:
                                    if (_zoom > 3) { // avoid the _zoom value to get freezed to 1
                                        zoom(_zoom / 1.5);
                                        std::cout << "GUI::run(): zoomed down to *" << _zoom << "\n";
                                    }
                                    break;
                                case SDLK_u:
                                    render();
                                    request_update = true;
                                    break;
                                case SDLK_e:
                                    if (algorithm == math::mandelbrot) algorithm = math::julia;
                                    else algorithm = math::mandelbrot;
                                    render();
                                    request_update = true;
                                    break;
                                case SDLK_F11:
                                    SDL_SetWindowFullscreen(window, (isFullScreen ? 0 : SDL_WINDOW_FULLSCREEN));
                                    isFullScreen = !isFullScreen;
                                    break;
                                case SDLK_j:
                                    request_update = true;
                                    break;
                            }
                    }
                }
            }
            if(request_update){
                std::cout << "GUI::run(): updating... ";
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                SDL_UpdateTexture(frame, nullptr, (void *)pixels_buffer, (int) (window_rect.w * sizeof(int)));
                SDL_RenderCopyEx(renderer, frame, nullptr, nullptr, 0, nullptr, (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));
                SDL_RenderPresent(renderer);
                request_update = false;
                std::cout << "[OK]\n";
            }
            if(is_zooming){
                SDL_Delay(60);
            }
            else if(zoom_factor != 0){
                zoom(_zoom*zoom_factor);
                zoom_factor = 0;
            }
        }
    }

    void destroy(){
        std::cout << "GUI::destroy() ";
        SDL_DestroyTexture(frame);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::cout << "[OK]\n";
    }
}
