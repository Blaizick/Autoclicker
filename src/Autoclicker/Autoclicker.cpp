//
// Created by novik on 12/22/2025.
//

#include "Autoclicker.h"

namespace Autoclicker {
    void Autoclicker::startClicking() {
        clicking = true;
        m_LastClickTime = std::chrono::high_resolution_clock::now();
    }

    void Autoclicker::stopClicking() {
        clicking = false;
    }

    void Autoclicker::setClicksCountData(ClicksCountData &data) {
        clicksCountData = data;
        curClicksCount = 0;
    }

    void Autoclicker::init() {

    }

    void Autoclicker::update() {
        framesSinceAutostop++;
        if (clicking) {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - m_LastClickTime;
            if (elapsed.count() >= ClickInterval::intervalToSeconds(clickIntervalData)) {
                INPUT Inputs[2] = {0};

                Inputs[0].type = INPUT_MOUSE;
                Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

                Inputs[1].type = INPUT_MOUSE;
                Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

                SendInput(2, Inputs, sizeof(INPUT));

                m_LastClickTime = now;

                if (clicksCountData.clicksType == ClicksCountType_ClicksCount && ++curClicksCount >= clicksCountData.clicksCount) {
                    clicking = false;
                    curClicksCount = 0;
                    framesSinceAutostop = 0;
                }
            }
        }
    }

    void AutoclickerWindow::draw() {
#ifdef IMGUI_HAS_VIEWPORT
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        auto size = viewport->GetWorkSize();
        size.x += 1.0f;
        size.y += 1.0f;
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowViewport(viewport->ID);
#else
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        auto size = ImGui::GetIO().DisplaySize;
        size.x += 1.0f;
        size.y += 1.0f;
        ImGui::SetNextWindowSize(size);
#endif

        ImGui::Begin("Autoclicker", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

        {
            ImGui::Columns(4, nullptr, false);

            ImGui::InputFloat("hrs.", &autoclicker->clickIntervalData.hours);
            ImGui::NextColumn();
            ImGui::InputFloat("min.", &autoclicker->clickIntervalData.minutes);
            ImGui::NextColumn();
            ImGui::InputFloat("sec.", &autoclicker->clickIntervalData.seconds);
            ImGui::NextColumn();
            ImGui::InputFloat("ms.", &autoclicker->clickIntervalData.milliseconds);
            ImGui::NextColumn();
        }

        {
            ImGui::RadioButton("Infinity clicks", &autoclicker->clicksCountData.clicksType, ClicksCountType_InfinityClicks);

            ImGui::Columns(2, nullptr, false);

            ImGui::PushID("ClicksCountButton");
            ImGui::RadioButton("Clicks count", &autoclicker->clicksCountData.clicksType, ClicksCountType_ClicksCount);
            ImGui::PopID();
            ImGui::NextColumn();

            int count = autoclicker->clicksCountData.clicksCount;
            ImGui::PushID("ClicksCountInputInt");
            ImGui::InputInt("Clicks count", &count);
            autoclicker->clicksCountData.clicksCount = std::max(0, count);
            ImGui::PopID();
            ImGui::NextColumn();
        }

        {
            const int framesDelaySinceAutostop = 10;

            ImGui::SetNextItemWidth(300);
            if (autoclicker->clicking) {
                if (ImGui::Button("Stop") && autoclicker->framesSinceAutostop > framesDelaySinceAutostop) {
                    autoclicker->stopClicking();
                }
            }
            else {
                if (ImGui::Button("Start") && autoclicker->framesSinceAutostop > framesDelaySinceAutostop) {
                    autoclicker->startClicking();
                }
            }
        }

        ImGui::End();
    }
}
