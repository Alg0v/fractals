#include "../../lib_imgui/imgui.h"
#include "../../lib_imgui/backends/imgui_impl_sdl2.h"
#include "../../lib_imgui/backends/imgui_impl_sdlrenderer2.h"
#include "../math/julia.h"
#include "../math/mandelbrot.h"
#include "GUI.h"
#include "color/algov.h"
#include "color/algov_alt.h"
#include "color/classic.h"
#include <SDL2/SDL.h>
#include <iostream>

int launcher(){
    std::cout << "GUI::init() ";
    int exitcode = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if (exitcode < 0) {
        std::cout << "[ERROR: SDL2 failed to initialize]:\n";
        std::cout << SDL_GetError() << "\n";
        exit(1);
    }

    SDL_Window* window = SDL_CreateWindow("Explorateur de fractales", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, NULL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    int display_mandelbrot = 0;
    int color_alg = 2;
    float re_c = 0;
    float im_c = .75;
    int _iterations = 64;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();


        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, 0);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Paramètres du programme", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Veuillez choisir la fractale à afficher");
        ImGui::BeginTable("algo_table", 2);
        ImGui::TableNextColumn();
        ImGui::RadioButton("Mandelbrot", &display_mandelbrot, 1);
        ImGui::TableNextColumn();
        ImGui::RadioButton("Julia", &display_mandelbrot, 0);
        ImGui::EndTable();
        if(display_mandelbrot == 0) {
            ImGui::Text("Veuillez choisir la constante à utiliser");
            ImGui::SliderFloat("Partie réelle", &re_c, -1.0f, 1.0f);
            ImGui::SliderFloat("Partie imaginaire", &im_c, -1.0f, 1.0f);
        }
        ImGui::SliderInt("Itérations de l'algorithme", &_iterations, 16, 255);
        ImGui::Text("Veuillez choisir l'algorithme de coloration à utiliser");
        ImGui::BeginTable("color_table", 3);
        ImGui::TableNextColumn();
        ImGui::RadioButton("Classique", &color_alg, 2);
        ImGui::TableNextColumn();
        ImGui::RadioButton("Algov I", &color_alg, 0);
        ImGui::TableNextColumn();
        ImGui::RadioButton("Algov II", &color_alg, 1);
        ImGui::EndTable();
        if (ImGui::Button("Lancer l'application")) done = true;

        ImGui::End();

        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if(display_mandelbrot == 1) GUI::algorithm = math::mandelbrot;
    else GUI::algorithm = math::julia;

    math::julia_c = math::Complex(re_c, im_c);

    if(color_alg == 0) GUI::coloring = color::algov;
    else if (color_alg == 1) GUI::coloring = color::algov_alt;
    else GUI::coloring = color::classic;

    GUI::iterations = _iterations;

    return 0;
}