//
// Created by novik on 12/22/2025.
//

#ifndef AUTOCLICKER_AUTOCLICKER_H
#define AUTOCLICKER_AUTOCLICKER_H

#include <chrono>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>

#include <windows.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <imgui_internal.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include <yaml-cpp/yaml.h>

typedef int ClicksCountType;
#define ClicksCountType_InfinityClicks 0
#define ClicksCountType_ClicksCount 1

typedef int ClickPositionType;
#define ClickPositionType_CursorPosition 0
#define ClickPositionType_CustomPosition 1

typedef int AutoclickerState;
#define AutoclickerState_Idle 0
#define AutoclickerState_Clicking 1
#define AutoclickerState_PickingPosition 2

typedef int ConfigSystemState;
#define ConfigSystemState_Idle 0
#define ConfigSystemState_Rebinding 1

namespace Autoclicker {
    class Vec2 {
    public:
        float x = 0.0f;
        float y = 0.0f;

        Vec2() = default;
        Vec2(float x, float y) : x(x), y(y) {};
    };

    class ClickIntervalData {
    public:
        float milliseconds = 100.0f;
        float seconds = 0.0f;
        float minutes = 0.0f;
        float hours = 0.0f;
    };
    class ClicksCountData {
    public:
        int clicksType = ClicksCountType_InfinityClicks;
        int clicksCount = 0;
    };
    class ClickPositionData {
    public:
        float x = 0.0f;
        float y = 0.0f;
        int clickPositionType = ClickPositionType_CursorPosition;
    };

    class ClickInterval {
    public:
        static float intervalToSeconds(ClickIntervalData& data) {
            return (data.milliseconds * 0.001f) + (data.seconds) + (data.minutes * 60) + (data.hours * 3600);
        }
    };

    class ConfigData {
    public:
        int enableKey = VK_F6;
        int overrideEnableKey = VK_F6;
    };

    class InputSystem {
    private:
        void fillWithClick(INPUT* inputs);
        bool m_IsKeyDown = false;
        int m_KeyDown = 0;
    public:
        void click();
        void click(float x, float y);
        bool getMousePosition(Vec2* pos);
        bool isKeyDown(int key);

        bool getKeyDown(int& key);
        void setKeyDown(int key);
        void update();
    };
    static std::shared_ptr<InputSystem> inputSystem;

    class ConfigSystem {
    public:
        static constexpr std::string FileName = "Config.yaml";

        std::filesystem::path configPath;

        int state = ConfigSystemState_Idle;
        ConfigData configData;

        std::shared_ptr<InputSystem> inputSystem;

        int framesSinceRebind = 0;

        ConfigSystem() = default;
        explicit ConfigSystem(const std::shared_ptr<InputSystem>& inputSystem) : inputSystem(inputSystem) {}

        void init();
        void update();

        void save();
        void load();

        void startRebinding();
        void stopRebinding();
        void resetKeybind();
    };

    class Autoclicker {
    private:
        std::chrono::time_point<std::chrono::system_clock> m_LastClickTime;

    public:
        std::shared_ptr<InputSystem> inputSystem;

        ClicksCountData clicksCountData;
        int curClicksCount = 0;
        int framesSinceAutostop = 0;

        ClickIntervalData clickIntervalData;
        ClickPositionData clickPositionData;

        int state = AutoclickerState_Idle;

        Autoclicker() = default;
        explicit Autoclicker(const std::shared_ptr<InputSystem>& inputSystem) : inputSystem(inputSystem) {}

        void startClicking();
        void stopClicking();
        void startPickingPosition();

        void setClicksCountData(ClicksCountData& data);

        void init();
        void update();
    };

    class DesktopInput {
    private:
        bool m_enablePressed = false;
    public:
        std::shared_ptr<InputSystem> inputSystem;
        std::shared_ptr<Autoclicker> autoclicker;
        std::shared_ptr<ConfigSystem> configSystem;

        DesktopInput(const std::shared_ptr<InputSystem>& inputSystem, const std::shared_ptr<Autoclicker>& autoclicker, const std::shared_ptr<ConfigSystem>& configSystem) :
            inputSystem(inputSystem), autoclicker(autoclicker), configSystem(configSystem) {}

        void update();
    };

    class SettingsWindow {
    public:
        bool active = false;

        std::shared_ptr<ConfigSystem> configSystem;

        explicit SettingsWindow(const std::shared_ptr<ConfigSystem>& configSystem) : configSystem(configSystem) {}

        void draw();
    };

    class AutoclickerWindow {
    public:
        std::shared_ptr<InputSystem> inputSystem;
        std::shared_ptr<Autoclicker> autoclicker;
        std::shared_ptr<SettingsWindow> settingsWindow;

        AutoclickerWindow() = default;
        AutoclickerWindow(const std::shared_ptr<Autoclicker>& autoclicker, const std::shared_ptr<InputSystem>& inputSystem, const std::shared_ptr<SettingsWindow>& settingsWindow) :
            autoclicker(autoclicker), inputSystem(inputSystem), settingsWindow(settingsWindow) {}

        void draw();
    };
}


#endif //AUTOCLICKER_AUTOCLICKER_H