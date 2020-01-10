#include "imgui/imgui.h"
#include "gui.hpp"
#include "renderer.hpp"

#include <GLFW/glfw3.h>

Gui::Gui(GLFWwindow *window, Renderer *renderer) : window(window), renderer(renderer) {}

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
    bool free_mode = renderer->IsInFreeMode();
    static bool show_window(true);
    ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Begin("##window", &show_window, windowFlags);
    if (ImGui::Button("Reset Camera"))
    {
        renderer->SetDefaults();
    }
    ToolTipHelper("R");
    if (ImGui::Checkbox("Free Roam", &free_mode))
    {
        renderer->ToggleFreeMode();
    }
    ToolTipHelper("TAB");
    ImGui::Text("Zoom Factor: %.1f", renderer->GetZoomFactor());
    ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
    ImGui::End();
}