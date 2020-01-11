#include "imgui/imgui.h"
#include "gui.hpp"
#include "base.hpp"

#include <GLFW/glfw3.h>

Gui::Gui(GLFWwindow *window, Base *base) : window(window), base(base) {}

void ToolTipHelper(const char *text)
{
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void Gui::HandleLogic()
{
    bool free_mode = base->IsInFreeMode();
    static bool show_window(true);
    ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Begin("##window", &show_window, windowFlags);
    if (ImGui::Button("Reset Camera"))
    {
        base->SetDefaults();
    }
    ToolTipHelper("R");
    if (ImGui::Checkbox("Free Roam", &free_mode))
    {
        base->ToggleFreeMode();
    }
    ToolTipHelper("TAB");
    ImGui::Text("Zoom Factor: %.1f", base->GetZoomFactor());
    ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
    ImGui::End();
}