#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <imgui.h>
#include <memory>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include <Windows.h>

#include "Autoclicker.h"

//TODO Crossplatform
//TODO Click duration
//TODO Show current state

HHOOK gKeyboardHook;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION) {
        KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;

        bool pressed = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
        bool released = (wParam == WM_KEYUP   || wParam == WM_SYSKEYUP);

        if (pressed) {
            Autoclicker::inputSystem->setKeyDown(kb->vkCode);
        }
    }
    return CallNextHookEx(gKeyboardHook, code, wParam, lParam);
}

int main() {
    gKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_TRANSPARENT;
    SDL_Window* window = SDL_CreateWindow("Autoclicker", (int)(600 * mainScale), (int)(400 * mainScale), windowFlags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

#pragma region Style

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Catppuccin Mocha Palette
    // --------------------------------------------------------
    const ImVec4 base       = ImVec4(0.117f, 0.117f, 0.172f, 1.0f); // #1e1e2e
    const ImVec4 mantle     = ImVec4(0.109f, 0.109f, 0.156f, 1.0f); // #181825
    const ImVec4 surface0   = ImVec4(0.200f, 0.207f, 0.286f, 1.0f); // #313244
    const ImVec4 surface1   = ImVec4(0.247f, 0.254f, 0.337f, 1.0f); // #3f4056
    const ImVec4 surface2   = ImVec4(0.290f, 0.301f, 0.388f, 1.0f); // #4a4d63
    const ImVec4 overlay0   = ImVec4(0.396f, 0.403f, 0.486f, 1.0f); // #65677c
    const ImVec4 overlay2   = ImVec4(0.576f, 0.584f, 0.654f, 1.0f); // #9399b2
    const ImVec4 text       = ImVec4(0.803f, 0.815f, 0.878f, 1.0f); // #cdd6f4
    const ImVec4 subtext0   = ImVec4(0.639f, 0.658f, 0.764f, 1.0f); // #a3a8c3
    const ImVec4 mauve      = ImVec4(0.796f, 0.698f, 0.972f, 1.0f); // #cba6f7
    const ImVec4 peach      = ImVec4(0.980f, 0.709f, 0.572f, 1.0f); // #fab387
    const ImVec4 yellow     = ImVec4(0.980f, 0.913f, 0.596f, 1.0f); // #f9e2af
    const ImVec4 green      = ImVec4(0.650f, 0.890f, 0.631f, 1.0f); // #a6e3a1
    const ImVec4 teal       = ImVec4(0.580f, 0.886f, 0.819f, 1.0f); // #94e2d5
    const ImVec4 sapphire   = ImVec4(0.458f, 0.784f, 0.878f, 1.0f); // #74c7ec
    const ImVec4 blue       = ImVec4(0.533f, 0.698f, 0.976f, 1.0f); // #89b4fa
    const ImVec4 lavender   = ImVec4(0.709f, 0.764f, 0.980f, 1.0f); // #b4befe

    // Main window and backgrounds
    colors[ImGuiCol_WindowBg]             = base;
    colors[ImGuiCol_ChildBg]              = base;
    colors[ImGuiCol_PopupBg]              = surface0;
    colors[ImGuiCol_Border]               = surface1;
    colors[ImGuiCol_BorderShadow]         = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_FrameBg]              = surface0;
    colors[ImGuiCol_FrameBgHovered]       = surface1;
    colors[ImGuiCol_FrameBgActive]        = surface2;
    colors[ImGuiCol_TitleBg]              = mantle;
    colors[ImGuiCol_TitleBgActive]        = surface0;
    colors[ImGuiCol_TitleBgCollapsed]     = mantle;
    colors[ImGuiCol_MenuBarBg]            = mantle;
    colors[ImGuiCol_ScrollbarBg]          = surface0;
    colors[ImGuiCol_ScrollbarGrab]        = surface2;
    colors[ImGuiCol_ScrollbarGrabHovered] = overlay0;
    colors[ImGuiCol_ScrollbarGrabActive]  = overlay2;
    colors[ImGuiCol_CheckMark]            = green;
    colors[ImGuiCol_SliderGrab]           = sapphire;
    colors[ImGuiCol_SliderGrabActive]     = blue;
    colors[ImGuiCol_Button]               = surface0;
    colors[ImGuiCol_ButtonHovered]        = surface1;
    colors[ImGuiCol_ButtonActive]         = surface2;
    colors[ImGuiCol_Header]               = surface0;
    colors[ImGuiCol_HeaderHovered]        = surface1;
    colors[ImGuiCol_HeaderActive]         = surface2;
    colors[ImGuiCol_Separator]            = surface1;
    colors[ImGuiCol_SeparatorHovered]     = mauve;
    colors[ImGuiCol_SeparatorActive]      = mauve;
    colors[ImGuiCol_ResizeGrip]           = surface2;
    colors[ImGuiCol_ResizeGripHovered]    = mauve;
    colors[ImGuiCol_ResizeGripActive]     = mauve;
    colors[ImGuiCol_Tab]                  = surface0;
    colors[ImGuiCol_TabHovered]           = surface2;
    colors[ImGuiCol_TabActive]            = surface1;
    colors[ImGuiCol_TabUnfocused]         = surface0;
    colors[ImGuiCol_TabUnfocusedActive]   = surface1;
    colors[ImGuiCol_PlotLines]            = blue;
    colors[ImGuiCol_PlotLinesHovered]     = peach;
    colors[ImGuiCol_PlotHistogram]        = teal;
    colors[ImGuiCol_PlotHistogramHovered] = green;
    colors[ImGuiCol_TableHeaderBg]        = surface0;
    colors[ImGuiCol_TableBorderStrong]    = surface1;
    colors[ImGuiCol_TableBorderLight]     = surface0;
    colors[ImGuiCol_TableRowBg]           = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_TableRowBgAlt]        = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]       = surface2;
    colors[ImGuiCol_DragDropTarget]       = yellow;
    colors[ImGuiCol_NavHighlight]         = lavender;
    colors[ImGuiCol_NavWindowingHighlight]= ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
    colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
    colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.0f, 0.0f, 0.0f, 0.35f);
    colors[ImGuiCol_Text]                 = text;
    colors[ImGuiCol_TextDisabled]         = subtext0;

    // Rounded corners
    style.WindowRounding    = 6.0f;
    style.ChildRounding     = 6.0f;
    style.FrameRounding     = 4.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding      = 4.0f;
    style.TabRounding       = 4.0f;

    // Padding and spacing
    style.WindowPadding     = ImVec2(8.0f, 8.0f);
    style.FramePadding      = ImVec2(5.0f, 3.0f);
    style.ItemSpacing       = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing  = ImVec2(4.0f, 4.0f);
    style.IndentSpacing     = 21.0f;
    style.ScrollbarSize     = 14.0f;
    style.GrabMinSize       = 10.0f;

    // Borders
    style.WindowBorderSize  = 1.0f;
    style.ChildBorderSize   = 1.0f;
    style.PopupBorderSize   = 1.0f;
    style.FrameBorderSize   = 0.0f;
    style.TabBorderSize     = 0.0f;

    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    //--------------------------------------------------------

#pragma endregion

    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    auto inputSystem = std::make_shared<Autoclicker::InputSystem>();
    auto configSystem = std::make_shared<Autoclicker::ConfigSystem>(inputSystem);
    configSystem->init();
    auto autoclicker = std::make_shared<Autoclicker::Autoclicker>(inputSystem);
    Autoclicker::inputSystem = inputSystem;
    autoclicker->init();
    auto desktopInput = std::make_shared<Autoclicker::DesktopInput>(inputSystem, autoclicker, configSystem);
    auto settingsWindow = std::make_shared<Autoclicker::SettingsWindow>(configSystem);
    auto autoclickerWindow = std::make_shared<Autoclicker::AutoclickerWindow>(autoclicker, inputSystem, settingsWindow);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();

        ImGui::NewFrame();

        desktopInput->update();
        autoclickerWindow->draw();
        settingsWindow->draw();
        configSystem->update();
        autoclicker->update();
        inputSystem->update();

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}