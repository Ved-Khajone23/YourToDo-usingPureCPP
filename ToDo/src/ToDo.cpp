#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "imgui.h"
#include <algorithm> // for std::sort
#include "Walnut/Image.h" //for image loading

class ToDoApp : public Walnut::Layer {
public:
    struct Task {
        std::string description;
        bool completed = false;
        int priority = 0; // 0 = low, 1 = medium, 2 = high
    };

    virtual void OnUIRender() override {

        ImVec2 windowPos(100, 100);
        ImVec2 windowSize(950, 500); // Fixed size for 150 mm x 60 mm

        // Set window position and size
        ImGui::SetNextWindowPos(windowPos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

        // Begin window with custom flags
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("##ToDoApp", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBackground); // No background to show custom rounded shape

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 pMin = ImGui::GetWindowPos();
        ImVec2 pMax = ImVec2(pMin.x + ImGui::GetWindowWidth(), pMin.y + ImGui::GetWindowHeight());

        float roundingRadius = 25.0f; // Set desired rounding radius
        ImU32 backgroundColor = IM_COL32(20, 20, 20, 255); // Dark background color

        // Draw filled rounded background for the entire window
        drawList->AddRectFilled(pMin, pMax, backgroundColor, roundingRadius);

        // Top margin background (for title bar area)
        ImVec2 pTopMax = ImVec2(pMin.x + ImGui::GetWindowWidth(), pMin.y + 45);
        drawList->AddRectFilled(pMin, pTopMax, IM_COL32(0, 0, 0, 255), roundingRadius, ImDrawCornerFlags_Top); // Dark black margin with rounded top corners

                                                                                // Draw macOS-style traffic light buttons

          // Draw macOS-style traffic light buttons
        ImVec2 buttonPos = ImVec2(pMin.x + 10, pMin.y + 10);
        float buttonRadius = 10.0f;

        // Close button (red)
        bool isCloseHovered = ImGui::IsMouseHoveringRect(ImVec2(buttonPos.x, buttonPos.y), ImVec2(buttonPos.x + 25, buttonPos.y + 25));
        ImU32 closeButtonColor = isCloseHovered ? IM_COL32(255, 120, 110, 255) : IM_COL32(255, 96, 92, 200);
        drawList->AddCircleFilled(ImVec2(buttonPos.x + 15, buttonPos.y + 12), buttonRadius, closeButtonColor);

        if (isCloseHovered) {
            ImGui::BeginTooltip();
            ImGui::Text("Close");
            ImGui::EndTooltip();
            ImVec2 textPos(buttonPos.x + 10, buttonPos.y - 0.7); // Position of the "X"
            ImU32 textColor = IM_COL32(10, 10, 10, 255);  // White color for the "X"
            ImGui::GetWindowDrawList()->AddText(textPos, textColor, "x");
        }

        if (isCloseHovered && ImGui::IsMouseClicked(0)) {
            Walnut::Application::Get().Close();
        }

        // Maximize button (yellow)
        ImVec2 maxButtonPos = ImVec2(buttonPos.x + 25, buttonPos.y);
        bool isMaxHovered = ImGui::IsMouseHoveringRect(ImVec2(maxButtonPos.x, maxButtonPos.y), ImVec2(maxButtonPos.x + 25, maxButtonPos.y + 25));
        ImU32 maxButtonColor = isMaxHovered ? IM_COL32(255, 220, 70, 255) : IM_COL32(255, 189, 68, 200);
        drawList->AddCircleFilled(ImVec2(maxButtonPos.x + 15, maxButtonPos.y + 12), buttonRadius, maxButtonColor);

        if (isMaxHovered) {
            ImGui::BeginTooltip();
            ImGui::Text("Maximize");
            ImGui::EndTooltip();
            ImVec2 textPos(maxButtonPos.x + 10, maxButtonPos.y - 0.7); // Position of the "+"
            ImU32 textColor = IM_COL32(10, 10, 10, 255);  // White color for the "+"
            ImGui::GetWindowDrawList()->AddText(textPos, textColor, "+");
        }

        if (isMaxHovered && ImGui::IsMouseClicked(0)) {
            // Add maximize functionality here (e.g., toggle window size)
            ImVec2 maximizedSize = ImVec2(1600, 900); // Example maximized size
            ImGui::SetNextWindowSize(maximizedSize);
        }

        // Minimize button (green)
        ImVec2 minButtonPos = ImVec2(buttonPos.x + 50, buttonPos.y);
        bool isMinHovered = ImGui::IsMouseHoveringRect(ImVec2(minButtonPos.x, minButtonPos.y), ImVec2(minButtonPos.x + 25, minButtonPos.y + 25));
        ImU32 minButtonColor = isMinHovered ? IM_COL32(0, 220, 88, 255) : IM_COL32(0, 202, 78, 200);
        drawList->AddCircleFilled(ImVec2(minButtonPos.x + 15, minButtonPos.y + 12), buttonRadius, minButtonColor);

        if (isMinHovered) {
            ImGui::BeginTooltip();
            ImGui::Text("Minimize");
            ImGui::EndTooltip();
            ImVec2 textPos(minButtonPos.x + 10, minButtonPos.y - 0.7); // Position of the "-"
            ImU32 textColor = IM_COL32(10, 10, 10, 255);  // White color for the "-"
            ImGui::GetWindowDrawList()->AddText(textPos, textColor, "-");
        }

        if (isMinHovered && ImGui::IsMouseClicked(0)) {
            // Add minimize functionality here (e.g., hide or reduce window size)
            ImVec2 minimizedSize = ImVec2(300, 200); // Example minimized size
            ImGui::SetNextWindowSize(minimizedSize);
        }
        ImGui::Dummy(ImVec2(10, 45)); // Space for header height
        ImGui::SetWindowFontScale(2.0f);//changes font size
        float indent = 10.0f; // Set your desired indentation
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);
        ImGui::Text("Your To Do"); // Title text
        ImGui::SetWindowFontScale(1.0f);//changes font size back to normal

                                                                                                    // New Task Button
// New Task Button
        ImVec2 buttonSize(120, 30); // Set your desired button size (width, height)
        ImGui::SetWindowFontScale(1.2f);
        ImGui::SameLine(ImGui::GetWindowWidth() - buttonSize.x + -70); // Align button to the right

        if (ImGui::Button("New Task")) {
            showAddTaskSection = !showAddTaskSection; // Toggle visibility of add task section
        }

        // Show Add New Task Section
        if (showAddTaskSection) {
            static char newTaskDescription[128] = "";
            static int newTaskPriority = 0;

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);//Indent by 10 pixels
            ImGui::Text("Enter Task Details:");

            // Input field for task description
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);
            ImGui::InputText(" ", newTaskDescription, IM_ARRAYSIZE(newTaskDescription));

            // Dropdown for priority selection
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);
            ImGui::Text("Priority:");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);
            ImGui::RadioButton(" Low", &newTaskPriority, 0); ImGui::SameLine();
            ImGui::RadioButton(" Medium", &newTaskPriority, 1); ImGui::SameLine();
            ImGui::RadioButton(" High", &newTaskPriority, 2);



            // Add Task button
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);
            if (ImGui::Button("Add Task")) {
                if (newTaskDescription[0] != '\0') {
                    tasks.push_back({ newTaskDescription, false, newTaskPriority });
                    SortTasksByPriority();
                    std::fill(std::begin(newTaskDescription), std::end(newTaskDescription), 0); // Clear description input
                    newTaskPriority = 0; // Reset priority
                    showAddTaskSection = false; // Hide the add task section after adding
                }
            }

            // Cancel button to hide the add task section
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                showAddTaskSection = false; // Hide the add task section without adding
            }
        }
        ImGui::NewLine();
        ImGui::SameLine(ImGui::GetWindowWidth() - 430);
        if (ImGui::RadioButton("ALL", &currentFilter, 0)) {}
        ImGui::SameLine();
        if (ImGui::RadioButton("IN PROGRESS", &currentFilter, 1)) {}
        ImGui::SameLine();
        if (ImGui::RadioButton("COMPLETED", &currentFilter, 2)) {}

        // Task list
        for (int i = 0; i < tasks.size(); ++i) {
            Task& task = tasks[i];

            if (currentFilter == 1 && task.completed) continue;
            if (currentFilter == 2 && !task.completed) continue;

            ImGui::PushID(i);

                                                                             // Set color based on priority level
            ImVec4 color;
            switch (task.priority) {
            case 0: color = ImVec4(0, 1, 0, 1); break;
            case 1: color = ImVec4(1, 1, 0, 1); break;
            case 2: color = ImVec4(1, 0, 0, 1); break;
            }

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);// Indent for the priority circle

            // Draw circular priority indicator
            ImVec2 pos = ImGui::GetCursorScreenPos();
            float radius = 10.0f;
            ImU32 circleColor = ImGui::ColorConvertFloat4ToU32(color);
            drawList->AddCircleFilled(ImVec2(pos.x + radius, pos.y + radius), radius, circleColor);

            // Cycle priority
            if (ImGui::InvisibleButton(("##priority" + std::to_string(i)).c_str(), ImVec2(radius * 2, radius * 2))) {
                task.priority = (task.priority + 1) % 3;
                SortTasksByPriority();
            }


            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f); // Indent by 10 pixels

            // Custom rounded checkbox
            float checkboxSize = 20.0f;  // Size of the checkbox
            ImVec2 checkboxPos = ImGui::GetCursorScreenPos();
            ImU32 checkboxColor = task.completed ? IM_COL32(135, 206, 235, 200) : IM_COL32(100, 100, 100, 200);  // Green if completed, gray otherwise
            float roundingRadius = 5.0f; // Radius for rounded corners

            // Draw checkbox background with rounded corners
            drawList->AddRectFilled(checkboxPos, ImVec2(checkboxPos.x + checkboxSize, checkboxPos.y + checkboxSize), checkboxColor, roundingRadius);

            // Check if clicked and toggle completion status
            if (ImGui::InvisibleButton(("##roundedCheckbox" + std::to_string(i)).c_str(), ImVec2(checkboxSize, checkboxSize))) {
                task.completed = !task.completed;
            }

            // Draw a checkmark if the task is completed
            if (task.completed) {
                float padding = 4.0f; // Padding for the checkmark
                // Adjusted first line of the tick mark
                drawList->AddLine(
                    ImVec2(checkboxPos.x + padding, checkboxPos.y + checkboxSize * 0.5f),     // Start near left-middle
                    ImVec2(checkboxPos.x + checkboxSize * 0.45f, checkboxPos.y + checkboxSize - padding), // Go to center-bottom
                    IM_COL32(10, 10, 10, 255), 2.0f); // Thicker line width for visibility

                // Adjusted second line of the tick mark
                drawList->AddLine(
                    ImVec2(checkboxPos.x + checkboxSize * 0.45f, checkboxPos.y + checkboxSize - padding), // Start from middle-bottom
                    ImVec2(checkboxPos.x + checkboxSize - padding, checkboxPos.y + checkboxSize * 0.3f), // End near top-right
                    IM_COL32(10, 10, 10, 255), 2.0f); //above 2 lines are just to make that tick icon

            }
            ImGui::SameLine();//Keeps the line same
            ImGui::Text(task.description.c_str());
            ImGui::SameLine();
            if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) {
                tasks.erase(tasks.begin() + i);
                --i;
            }

            ImGui::PopID();
        }

        ImGui::End();
    }

private:
    std::shared_ptr<Walnut::Image> M_Image;
    bool showAddTaskSection = false;
    std::vector<Task> tasks = {
        {"Get my homework done", false, 2},
        {"Get food for the cat", false, 1},
        {"Buy a new sweater", true, 1},
        {"Get the dishes done", true, 1},
        {"Touch some grass", false, 0}
    }; 
    int currentFilter = 0;
    //Sorting of tasks
    void SortTasksByPriority() {
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.priority > b.priority;
            });
    }
};

class ToDoAppExample : public Walnut::Application {
public:
    ToDoAppExample() {
        PushLayer<ToDoApp>();
    }
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
    return new ToDoAppExample();
}
