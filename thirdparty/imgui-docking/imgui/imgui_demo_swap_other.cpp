
#include "imgui.h"
#include <string>
#include <iostream>

// Demonstrate most Dear ImGui features (this is big function!)
// You may execute this function to experiment with the UI and understand what it does.
// You may then search for keywords in the code when you are interested by a specific feature.
void ImGui::ShowDemoWindow(bool* p_open, char* name)
{
    // Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
    // Most functions would normally just assert/crash if the context is missing.
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    

    ImGuiWindowFlags window_flags = 0;
   
    // Main body of the Demo window starts here.
    if (!ImGui::Begin(name, p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    enum Mode
    {
        Mode_Copy,
        Mode_Move,
        Mode_Swap
    };
    static int mode = 0;
    
    if (ImGui::RadioButton((std::string("Copy (") + name + ")").c_str(), mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
    if (ImGui::RadioButton((std::string("Move (") + name + ")").c_str(), mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
    if (ImGui::RadioButton((std::string("Swap (") + name + ")").c_str(), mode == Mode_Swap)) { mode = Mode_Swap; }

    const int namesSize = 9;
    static const char* namesA[9] =
    {
        "Bobby", "Beatrice", "Betty",
        "Brianna", "Barry", "Bernard",
        "Bibi", "Blaine", "Bryn"
    };

    static const char* namesB[9] =
    {
        "Bobby", "Beatrice", "Betty",
        "Brianna", "Barry", "Bernard",
        "Bibi", "Blaine", "Bryn"
    };

    const char* nameOther = !strcmp(name, "b") ? "a" : "b";
    const char** names = !strcmp(name, "b") ? namesB : namesA;
    const char** namesOther = !strcmp(name, "b") ? namesA : namesB;

    for (int n = 0; n < namesSize; n++)
    {
        int m = n;
        

        if (!strcmp(name, "b")) {
            m += namesSize;
            
        }

        ImGui::PushID(m);
        if ((n % 3) != 0)
            ImGui::SameLine();
        ImGui::Button(names[n], ImVec2(60, 60));

        // Our buttons are both drag sources and drag targets here!
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload(name, &m, sizeof(int));

            // Display preview (could be anything, e.g. when dragging an image we could decide to display
            // the filename and a small preview of the image, etc.)
            if (mode == Mode_Copy) { ImGui::Text("Copy %s", names[n]); }
            if (mode == Mode_Move) { ImGui::Text("Move %s", names[n]); }
            if (mode == Mode_Swap) { ImGui::Text("Swap %s", names[n]); }
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(nameOther);

            if (payload == NULL)
            {
                payload = ImGui::AcceptDragDropPayload(name);
            }
            //if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(name))
            if (payload != NULL)
            {
                IM_ASSERT(payload->DataSize == sizeof(int));
                int payload_n = *(const int*)payload->Data;
                std::cout << (payload_n) << std::endl;
                //const char* namesO = names;
                if ((payload_n >= namesSize) != (!strcmp(name, "b")))
                {

                    std::cout << (payload_n) << std::endl;
                    payload_n %= namesSize;
                    std::cout << (payload_n) << std::endl;

                    if (mode == Mode_Copy)
                    {
                        names[n] = names[payload_n];
                    }
                    if (mode == Mode_Move)
                    {
                        names[n] = names[payload_n];
                        names[payload_n] = "";
                    }
                    if (mode == Mode_Swap)
                    {
                        
                        const char* tmp = names[n];
                        names[n] = namesOther[payload_n];
                        namesOther[payload_n] = tmp;
                    }
                }
                
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::PopID();
    }


    // End of ShowDemoWindow()
    ImGui::End();
}




