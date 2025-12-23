//
// Created by novik on 12/22/2025.
//

#ifndef AUTOCLICKER_AUTOCLICKER_H
#define AUTOCLICKER_AUTOCLICKER_H

#include <chrono>
#include <memory>
#include <iostream>
#include <windows.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

typedef int ClicksCountType;
#define ClicksCountType_InfinityClicks 0
#define ClicksCountType_ClicksCount 1

namespace Autoclicker {
    class ClickIntervalData {
    public:
        float milliseconds = 100.0f;
        float seconds = 0.0f;
        float minutes = 0.0f;
        float hours = 0.0f;
    };
    class ClicksCountData {
    public:
        int clicksType = 0;
        int clicksCount = 0;
    };

    class ClickInterval {
    public:
        static long intervalToSeconds(ClickIntervalData& data) {
            return (data.milliseconds * 0.001f) + (data.seconds) + (data.minutes * 60) + (data.hours * 3600);
        }
    };


    class Autoclicker {
    private:
        std::chrono::time_point<std::chrono::system_clock> m_LastClickTime;

    public:
        ClicksCountData clicksCountData;
        int curClicksCount = 0;
        int framesSinceAutostop = 0;

        ClickIntervalData clickIntervalData;
        bool clicking = false;

        void startClicking();
        void stopClicking();

        void setClicksCountData(ClicksCountData& data);

        void init();
        void update();
    };

    class AutoclickerWindow {
    public:
        std::shared_ptr<Autoclicker> autoclicker;

        AutoclickerWindow(std::shared_ptr<Autoclicker> autoclicker) : autoclicker(autoclicker) {};

        void draw();
    };
}


#endif //AUTOCLICKER_AUTOCLICKER_H