#include <unordered_map>
#include <map>
#include <string>
#include <functional>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class Menu 
{
    class ImGuiWindow
    {
    public:
        ImGuiWindow() = default;
        ImGuiWindow(const std::string& label, const ImVec2& position, const ImVec2& size = ImVec2(100, 100), int flags = 0)
            :
            m_Title(label), m_Position(position), m_Size(size), m_Flags(flags)
        {}

        ImGuiWindow& AddButton(const std::string& label, const std::function<void()>& callback)
        {
            auto pWidget = m_Widgets.find(label);

            if (pWidget == m_Widgets.end())
                m_SortedLabels.push_back(label);

            m_Widgets[label] = [label, callback] {
                if (ImGui::Button(label.c_str()))
                {
                    callback();
                }
            };
            return *this;
        }
        ImGuiWindow& AddSliderFloat(const std::string& label, float* value, float minValue, float maxValue)
        {
            auto pWidget = m_Widgets.find(label);

            if (pWidget == m_Widgets.end())
                m_SortedLabels.push_back(label);

            m_Widgets[label] = [label, value, minValue, maxValue] {
                ImGui::SliderFloat(LabelPrefix(label).c_str(), value, minValue, maxValue);
            };
            return *this;
        }
        ImGuiWindow& AddCheckbox(const std::string& label, bool* value)
        {
            auto pWidget = m_Widgets.find(label);

            if (pWidget == m_Widgets.end())
                m_SortedLabels.push_back(label);

            m_Widgets[label] = [label, value] {
                ImGui::Checkbox(label.c_str(), value);
            };
            return *this;
        }
        ImGuiWindow& AddText(const std::string& label)
        {
            auto pWidget = m_Widgets.find(label);

            if (pWidget == m_Widgets.end())
                m_SortedLabels.push_back(label);

            m_Widgets[label] = [label] {
                ImGui::Text(label.c_str());
            };
            return *this;
        }
        ImGuiWindow& AddImage(const std::string& label, GLuint imageID, const ImVec2& size) // TODO: migth be a problem
        {
            auto pWidget = m_Widgets.find(label);

            if (pWidget == m_Widgets.end())
                m_SortedLabels.push_back(label);

            m_Widgets[label] = [label, imageID, size] {
                ImGui::Image((void*)(intptr_t)imageID, size);
            };
            return *this;
        }

        void SetSize(const ImVec2& size)
        {
            m_Size = size;
        }
        void SetPosition(const ImVec2& position)
        {
            m_Position = position;
        }    

        void Render()
        {
            ImGui::SetNextWindowPos(m_Position);
            ImGui::SetNextWindowSize(m_Size);
            ImGui::Begin(m_Title.c_str(), (bool*)0, m_Flags);
            Update();
            ImGui::End();
        }

    private:
        void Update()
        {
            for (const auto& label : m_SortedLabels)
            {
                auto widget = m_Widgets.find(label);
                if (widget != m_Widgets.end())
                {
                    widget->second();
                }
            }
        }

    private:
        int m_Flags;
        ImVec2 m_Size;
        ImVec2 m_Position;
        std::string m_Title;

        std::vector<std::string> m_SortedLabels;
        std::map<std::string, std::function<void()>> m_Widgets;
    };
public:
    Menu(GLFWwindow* window);
    ~Menu();

    ImGuiWindow& AddWindow(const std::string& label, const ImVec2& position, const ImVec2& size);
    ImGuiWindow& AlterWindow(const std::string& label);

    void Render();

    // Imgui styles from @janekb04 
    // https://github.com/ocornut/imgui/issues/707#issuecomment-917151020
    static void EmbraceTheDarkness();

private:
    static std::string LabelPrefix(const std::string& label);

private:
    std::unordered_map<std::string, ImGuiWindow> m_ImGuiWindows;
};
