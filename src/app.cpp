#include "app.h"
#include <fmt/format.h>

using namespace std::chrono;

SDL2OpenGL3ImGuiApplication::SDL2OpenGL3ImGuiApplication(std::string title, int x, int y, int w, int h, uint32_t flags,
                                                         double fps)
: title{std::move(title)},
  windowW{w},
  windowH{h},
  vsync{fps <= 0},
  timePerFrame{vsync ? duration<double>{0} : duration<double>{1} / fps},
  addedEventHandler{nullptr},
  addedDrawingHandler{nullptr} {
    // Init SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        throw(std::runtime_error(fmt::format("Error: {}/n", SDL_GetError())));
    }
    // GL 3.0 + GLSL 130
    const auto* glslVersion = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);  // NOLINT
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // NOLINT
    window = SDL_CreateWindow("New C++ Project", x, y, windowW, windowH, flags);
    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    if (vsync) SDL_GL_SetSwapInterval(1);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

auto SDL2OpenGL3ImGuiApplication::loadTTFFont(const std::filesystem::path& fontPath, float fontSize, // NOLINT
                                              const ImFontConfig* fontCfg, const ImWchar* glyphRanges) -> ImFont* {
    auto fontPathStr = fontPath.generic_string();
    return ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPathStr.data(), fontSize, fontCfg, glyphRanges);
}

SDL2OpenGL3ImGuiApplication::~SDL2OpenGL3ImGuiApplication() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

struct FrameTimeGuard {
    const bool vsync;
    const std::chrono::duration<double> timePerFrame;
    steady_clock::time_point beginFrameTime;
    FrameTimeGuard(bool vsync, std::chrono::duration<double> timePerFrame) : vsync{vsync}, timePerFrame{timePerFrame} {
        if (!vsync) beginFrameTime = steady_clock::now();
    }
    ~FrameTimeGuard() {
        if (!vsync) {
            auto elapsed = steady_clock::now() - beginFrameTime;
            auto sleepTime = duration_cast<milliseconds>(timePerFrame - elapsed);
            if (sleepTime.count() > 0) SDL_Delay((Uint32)sleepTime.count());
        }
    }
    FrameTimeGuard(const FrameTimeGuard&) = delete;
    auto operator=(const FrameTimeGuard&) = delete;
    FrameTimeGuard(FrameTimeGuard&&) = delete;
    auto operator=(FrameTimeGuard&&) = delete;
};

auto SDL2OpenGL3ImGuiApplication::run() const -> int {
    for (bool quit = false; !quit;) {
        FrameTimeGuard ftg{vsync, timePerFrame};
        for (SDL_Event event; static_cast<bool>(SDL_PollEvent(&event));) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) quit = true;
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                    quit = true;
            }
            // User process event function
            if (addedEventHandler) addedEventHandler(event, quit);
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // User draw function
        if (addedDrawingHandler) addedDrawingHandler(windowW, windowH);

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    return 0;
}