#ifndef APP_H
#define APP_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <chrono>
#include <filesystem>
#include <functional>
#include <cstdint>

class SDL2OpenGL3ImGuiApplication {
public:
    SDL2OpenGL3ImGuiApplication(std::string title, int x, int y, int w, int h, uint32_t flags = 0, double fps = 0);
    ~SDL2OpenGL3ImGuiApplication();
    SDL2OpenGL3ImGuiApplication(const SDL2OpenGL3ImGuiApplication&) = delete;
    SDL2OpenGL3ImGuiApplication(SDL2OpenGL3ImGuiApplication&&) = delete;
    auto operator=(const SDL2OpenGL3ImGuiApplication&) = delete;
    auto operator=(SDL2OpenGL3ImGuiApplication&&) = delete;
    auto loadTTFFont(const std::filesystem::path& fontPath, float fontSize, const ImFontConfig* fontCfg = nullptr,
                     const ImWchar* glyphRanges = nullptr) -> ImFont*;
    template <class EventHandler>
    void setEventHandler(EventHandler&& handler) {
        addedEventHandler = handler;
    }
    template <class DrawingHandler>
    void setDrawHandler(DrawingHandler&& handler) {
        addedDrawingHandler = handler;
    }
    auto run() const -> int;

private:
    std::string title;
    int windowW;
    int windowH;
    bool vsync;
    std::chrono::duration<double> timePerFrame;
    SDL_GLContext glContext;
    SDL_Window* window;
    std::function<void(SDL_Event&, bool&)> addedEventHandler;
    std::function<void(int, int)> addedDrawingHandler;
};

#endif