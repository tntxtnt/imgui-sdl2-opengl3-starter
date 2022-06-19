#include "app.h"
#include <filesystem>

static constexpr int kWindowW = 900;
static constexpr int kWindowH = 600;
static constexpr double kFps = 60.0;
static constexpr float kFontSize = 18.0F;
static const std::filesystem::path kFontDirPath{"res/fonts"};
static const char* const kWideosevkaExtended = "wideosevka-extendedmedium.ttf";

auto main(int /*argc*/, char** /*argv*/) -> int {
    SDL2OpenGL3ImGuiApplication app("New C++ project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kWindowW,
                                    kWindowH, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI, kFps);
    // Load fonts
    const ImWchar bmpRanges[] = {0x0001, 0xFFFF, 0};
    app.loadTTFFont(kFontDirPath / kWideosevkaExtended, kFontSize, nullptr, static_cast<const ImWchar*>(bmpRanges));
    // Draw
    app.setDrawHandler([&](int windowW, int windowH) {
        ImGui::Begin("Gui", nullptr, 0);
        ImGui::End();
    });
    return app.run();
}