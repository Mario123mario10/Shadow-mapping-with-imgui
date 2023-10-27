// Dear ImGui: standalone example application for GLFW + OpenGL2, using legacy fixed pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_glfw_opengl2/ folder**
// See imgui_impl_glfw.cpp for details.

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>



// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

//void ShowExampleAppCustomNodeGraph(bool* opened)
//{
//    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);
//    if (!ImGui::Begin("Example: Custom Node Graph", opened))
//    {
//        ImGui::End();
//        return;
//    }
//
//    // Dummy
//    struct Node
//    {
//        ImVec2 Pos, Size;
//        int ID;
//        char Name[32];
//        ImGuiColorEditFlags ColorEditFlags;
//
//        Node(int id, const ImVec2& pos, const char* name, ImGuiColorEditFlags flags)
//        {
//            ID = id;
//            Pos = pos;
//            Size = ImVec2(100, 50);
//            strncpy(Name, name, 31);
//            ColorEditFlags = flags;
//        }
//
//        void Draw()
//        {
//            ImGui::SetCursorScreenPos(Pos);
//            ImGui::InvisibleButton(Name, Size);
//            if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
//            {
//                Pos.x += ImGui::GetIO().MouseDelta.x;
//                Pos.y += ImGui::GetIO().MouseDelta.y;
//            }
//
//            ImGui::PushID(ID)
//                ;
//            ImGui::BeginGroup();
//            ImGui::Text("Node %d", ID);
//            ImGui::ColorEdit3("Color", (float*)&Color, ColorEditFlags);
//            ImGui::EndGroup();
//            ImGui::PopID();
//        }
//    };
//
//    static ImVector<Node> nodes;
//    if (nodes.Size == 0)
//    {
//        nodes.push_back(Node(0, ImVec2(50, 50), "Node 0", ImGuiColorEditFlags_RGB));
//        nodes.push_back(Node(1, ImVec2(300, 50), "Node 1", ImGuiColorEditFlags_HSV));
//    }
//
//    //Draw a list of nodes on the left side
//    bool open_context_menu = false;
//    int node_hovered_in_list = -1;
//    int node_hovered_in_scene = -1;
//    ImGui::BeginChild("node_list", ImVec2(100, 0));
//    ImGui::Text("Nodes");
//    ImGui::Separator();
//    for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
//    {
//        Node* node = &nodes[node_idx];
//        ImGui::PushID(node->ID);
//        if (ImGui::Selectable(node->Name, node->ID == node_hovered_in_list))
//            node_hovered_in_list = node->ID;
//        if (ImGui::IsItemHovered())
//        {
//            node_hovered_in_list = node->ID;
//            open_context_menu |= ImGui::IsMouseClicked(1);
//        }
//        ImGui::PopID();
//    }
//    ImGui::EndChild();
//
//    ImGui::SameLine();
//    ImGui::BeginGroup();
//
//    const float NODE_SLOT_RADIUS = 4.0f;
//    const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);
//
//    // Create our child canvas
//    ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", ImGui::GetScrollX(), ImGui::GetScrollY());
//    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
//    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
//    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
//    ImGui::PushItemWidth(120.0f);
//
//    ImVec2 offset = ImGui::GetCursorScreenPos() + ImVec2(NODE_WINDOW_PADDING.x, NODE_WINDOW_PADDING.y);
//    ImDrawList* draw_list = ImGui::GetWindowDrawList();
//    draw_list->ChannelsSplit(2);
//
//    // Display grid
//    if (true)
//    {
//        ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
//        float GRID_SZ = 64.0f;
//        ImVec2 win_pos = ImGui::GetCursorScreenPos();
//        ImVec2 canvas_sz = ImGui::GetWindowSize();
//        for (float x = fmodf(ImGui::GetScrollX(), GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
//            draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
//        for (float y = fmodf(ImGui::GetScrollY(), GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
//            draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
//    }
//
//    // Display nodes
//    for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
//    {
//        Node* node = &nodes[node_idx];
//        ImGui::PushID(node->ID);
//        ImVec2 node_rect_min = offset + node->Pos;
//
//        // Display node contents first
//        draw_list->ChannelsSetCurrent(1); // Foreground
//        bool old_any_active = ImGui::IsAnyItemActive();
//        ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
//        ImGui::BeginGroup(); // Lock horizontal position
//        ImGui::Text("Node %d", node->ID);
//        //ImGui::ColorEdit3("Color", (float*)&node->Color, node->ColorEditFlags);
//        ImGui::EndGroup();
//
//        bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
//        node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
//        ImVec2 node_rect_max = node_rect_min + node->Size;
//
//        // Display node box
//        draw_list->ChannelsSetCurrent(0); // Background
//        ImGui::SetCursorScreenPos(node_rect_min);
//        ImGui::InvisibleButton("node", node->Size);
//        if (ImGui::IsItemHovered())
//        {
//            node_hovered_in_scene = node->ID;
//            open_context_menu |= ImGui::IsMouseClicked(1);
//        }
//        bool node_moving_active = ImGui::IsItemActive();
//        if (node_widgets_active || node_moving_active)
//            node_hovered_in_list = node->ID;
//        if (node_moving_active && ImGui::IsMouseDragging(0))
//            node->Pos = node->Pos + ImGui::GetIO().MouseDelta;
//
//        ImVec4 node_rect_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_hovered_in_scene == -1)) ? ImVec4(1.0f, 0.0f, 0.0f, 0.5f) : ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
//        draw_list->AddRectFilled(node_rect_min, node_rect_max, IM_COL32(node_rect_color.x * 255, node_rect_color.y * 255, node_rect_color.z * 255, node_rect_color.w * 255), 4.0f);
//        draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
//
//        ImGui::PopID();
//    }
//    draw_list->ChannelsMerge();
//
//    // Open context menu
//    if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseReleased(1))
//        node_hovered_in_list = node_hovered_in_scene = -1;
//    if (open_context_menu)
//    {
//        ImGui::OpenPopup("context_menu");
//        if (node_hovered_in_list != -1)
//            nodes[node_hovered_in_list].Selected = true;
//    }
//
//    // Draw context menu
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
//    if (ImGui::BeginPopup("context_menu"))
//    {
//        Node* node = node_hovered_in_list != -1 ? &nodes[node_hovered_in_list] : NULL;
//        ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
//        if (node)
//        {
//            ImGui::Text("Node '%s'", node->Name);
//            ImGui::Separator();
//            if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
//            if (ImGui::MenuItem("Delete", NULL, false, false)) {}
//            if (ImGui::MenuItem("Copy", NULL, false, false)) {}
//        }
//        else
//        {
//            if (ImGui::MenuItem("Add")) { nodes.push_back(Node(nodes.Size, scene_pos, "Node", ImGuiColorEditFlags_RGB)); }
//            if (ImGui::MenuItem("Paste", NULL, false, false)) {}
//        }
//        ImGui::EndPopup();
//    }
//    ImGui::PopStyleVar();
//
//    // Scrolling
//    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
//        ImGui::SetScrollX(ImGui::GetScrollX() - ImGui::GetIO().MouseDelta.x);
//    ImGui::SetScrollY(ImGui::GetScrollY() - ImGui::GetIO().MouseDelta.y);
//
//    ImGui::PopItemWidth();
//    ImGui::EndChild();
//    ImGui::PopStyleColor();
//    ImGui::PopStyleVar(2);
//    ImGui::EndGroup();
//    ImGui::End();
//}

static int payload_data = 42;
static bool item_dropped = false;
int main(int, char**)
{
    // Inicjalizacja GLFW
    if (!glfwInit())
        return 1;

    // Utworzenie okna GLFW
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Przyk³ad ImGui + OpenGL 2", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-sync

    // Inicjalizacja ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Inicjalizacja backendów ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // G³ówna pêtla renderowania
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Rozpoczêcie ramki ImGui
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Twój kod ImGui tutaj
        ImGui::Begin("Drag and Drop Example");

        // Element do przeci¹gania
        if (ImGui::Button("Przeci¹gnij mnie!"))
            ImGui::SetDragDropPayload("DND_DEMO", &payload_data, sizeof(int)); // Ustawienie ³adunku do przeci¹gniêcia
            //ImGui::Text("Our text");
        if (ImGui::BeginDragDropSource())
        {
            ImGui::Text("Przeci¹ganie...");
            ImGui::EndDragDropSource();
        }

        // Miejsce docelowe
        ImGui::Button(item_dropped ? "Upuszczono!" : "Upuœæ tutaj!");
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO"))
            {
                IM_ASSERT(payload->DataSize == sizeof(int));
                int payload_n = *(const int*)payload->Data;
                // Teraz mo¿esz u¿ywaæ zmiennej payload_n
                item_dropped = true;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::End();

        // Renderowanie
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Czyszczenie
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
