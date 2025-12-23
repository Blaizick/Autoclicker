//
// Created by novik on 12/22/2025.
//

#include "Autoclicker.h"

#include <algorithm>

namespace Autoclicker {
#pragma region InputSystem
    void InputSystem::fillWithClick(INPUT *inputs) {
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }

    void InputSystem::click() {
        INPUT inputs[2] = {0};
        fillWithClick(inputs);
        SendInput(2, inputs, sizeof(INPUT));
    }

    void InputSystem::click(float x, float y) {
        INPUT inputs[3] = {0};

        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        inputs[0].mi.dx = static_cast<long>(x) * 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
        inputs[0].mi.dy = static_cast<long>(y) * 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);

        fillWithClick(inputs + 1);

        SendInput(3, inputs, sizeof(INPUT));
    }

    bool InputSystem::getMousePosition(Vec2 *pos) {
        POINT point;
        if (!GetCursorPos(&point)) {
            pos->x = 0;
            pos->y = 0;
            return false;
        }
        pos->x = static_cast<float>(point.x);
        pos->y = static_cast<float>(point.y);
        return true;
    }

    bool InputSystem::isKeyDown(int key) {
        return GetAsyncKeyState(key) & 0x8000;
    }
#pragma endregion

#pragma region Autoclicker
    void Autoclicker::startClicking() {
        state = AutoclickerState_Clicking;
        m_LastClickTime = std::chrono::high_resolution_clock::now();
    }

    void Autoclicker::stopClicking() {
        state = AutoclickerState_Idle;
    }

    void Autoclicker::startPickingPosition() {
        state = AutoclickerState_PickingPosition;
    }

    void Autoclicker::setClicksCountData(ClicksCountData &data) {
        clicksCountData = data;
        curClicksCount = 0;
    }

    void Autoclicker::init() {

    }

    void Autoclicker::update() {
        framesSinceAutostop++;
        if (state == AutoclickerState_Clicking) {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - m_LastClickTime;
            if (elapsed.count() >= ClickInterval::intervalToSeconds(clickIntervalData)) {
                if (clickPositionData.clickPositionType == ClickPositionType_CursorPosition) {
                    inputSystem->click();
                }
                else {
                    inputSystem->click(clickPositionData.x, clickPositionData.y);
                }

                m_LastClickTime = now;

                if (clicksCountData.clicksType == ClicksCountType_ClicksCount && ++curClicksCount >= clicksCountData.clicksCount) {
                    state = AutoclickerState_Idle;
                    curClicksCount = 0;
                    framesSinceAutostop = 0;
                }
            }
        }

        if (state == AutoclickerState_PickingPosition) {
            if (inputSystem->isKeyDown(VK_LBUTTON)) {
                Vec2 pos;
                if (!inputSystem->getMousePosition(&pos)) {
                    return;
                }
                clickPositionData.x = pos.x;
                clickPositionData.y = pos.y;
                state = AutoclickerState_Idle;
            }
        }
    }
#pragma endregion

#pragma region AutoclickerWindow
    void AutoclickerWindow::draw() {
#ifdef IMGUI_HAS_VIEWPORT
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
#else
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif

        ImGui::Begin("Autoclicker", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

        {
            ImGui::Text("Click interval");

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

        ImGui::Spacing();

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

        ImGui::Spacing();

        {
            ImGui::RadioButton("Click under cursor", &autoclicker->clickPositionData.clickPositionType, ClickPositionType_CursorPosition);

            ImGui::Columns(4, nullptr, false);
            ImGui::RadioButton("Custom click position", &autoclicker->clickPositionData.clickPositionType, ClickPositionType_CustomPosition);
            ImGui::NextColumn();
            ImGui::InputFloat("X", &autoclicker->clickPositionData.x);
            ImGui::NextColumn();
            ImGui::InputFloat("Y", &autoclicker->clickPositionData.y);
            ImGui::NextColumn();
            if (ImGui::Button("Pick position")) {
                autoclicker->startPickingPosition();
            }
            ImGui::NextColumn();
        }

        ImGui::Spacing();

        {
            bool canHandle = autoclicker->framesSinceAutostop > 10;

            bool disabled = autoclicker->state == AutoclickerState_Clicking;
            if (disabled) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
            if (ImGui::Button("Start") && canHandle) {
                autoclicker->startClicking();
            }
            if (disabled) {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }

            ImGui::SameLine();
            disabled = !disabled;
            if (disabled) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
            if (ImGui::Button("Stop") && canHandle) {
                autoclicker->stopClicking();
            }
            if (disabled) {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
        }

        ImGui::End();
    }
#pragma endregion
}
