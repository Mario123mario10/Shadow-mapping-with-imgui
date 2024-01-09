
#include <memory>
#include <texture.h>
#include <shader.h>
#include <light.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void renderImgui(int screenWidth, int screenHeight, std::shared_ptr<Texture2D> windowTexture, const Shader& shader, PerspectiveLight pointLight, PerspectiveLight standingLight, SpotLight spotLight) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool showSettingsWindow = false;
    static bool showPointLightSettings = false;
    static bool showSpotLightSettings = false;

    static bool alpha_preview = false;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = false;
    static bool options_menu = false;
    static bool hdr = false;

    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);


    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("OpenGL Texture Window");

    if (ImGui::Button("Opcje")) {
        showSettingsWindow = !showSettingsWindow;
    }

    ImGui::Image((void*)(intptr_t)windowTexture->getId(), ImVec2(screenWidth, screenHeight), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    if (showSettingsWindow) {
        ImGui::SetNextWindowSize(ImVec2(325, 1080), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(0, 50));

        ImGui::Begin("OpenGL Settings");
        shader.use();

        ImVec4 colorPointLight = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
        ImVec4 colorSpotLight = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

        float specularPointLight = 0.0f;
        float diffusePointLight = 0.5f;

        float specularSpotLight = 0.0f;
        float diffuseSpotLight = 0.5f;

        float spotLightInnerCutOff = 12.5f;


        if (ImGui::Button("Point Light Settings")) {
            showPointLightSettings = !showPointLightSettings;
        }

        if (showPointLightSettings) {
            ImGui::Text("PointLight_color_picker");
            ImGui::PushItemWidth(-1);
            if (ImGui::ColorPicker4("##PointLight_color_picker", (float*)&colorPointLight, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs)) {
                pointLight.setColor(colorPointLight.x, colorPointLight.y, colorPointLight.z);
                standingLight.setColor(colorPointLight.x, colorPointLight.y, colorPointLight.z);

                shader.modifyUniform<glm::vec3>("pointLight[0].color", pointLight.getColor());
                shader.modifyUniform<glm::vec3>("pointLight[1].color", standingLight.getColor());
            }

            ImGui::Text("PointLight_specular_slider");
            if (ImGui::SliderFloat("##PointLight_specular_slider", &specularPointLight, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
                pointLight.setSpecular(specularPointLight, specularPointLight, specularPointLight);
                standingLight.setSpecular(specularPointLight, specularPointLight, specularPointLight);

                shader.modifyUniform<glm::vec3>("pointLight[0].specular", pointLight.getSpecular());
                shader.modifyUniform<glm::vec3>("pointLight[1].specular", standingLight.getSpecular());
            }


            ImGui::Text("PointLight_diffuse_slider");
            if (ImGui::SliderFloat("##PointLight_diffuse_slider", &diffusePointLight, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
                pointLight.setDiffuse(diffusePointLight, diffusePointLight, diffusePointLight);
                standingLight.setDiffuse(diffusePointLight, diffusePointLight, diffusePointLight);

                shader.modifyUniform<glm::vec3>("pointLight[0].diffuse", pointLight.getDiffuse());
                shader.modifyUniform<glm::vec3>("pointLight[1].diffuse", standingLight.getDiffuse());
            }
            ImGui::PopItemWidth();
        }

        if (ImGui::Button("Spot Light Settings")) {
            showSpotLightSettings = !showSpotLightSettings;
        }

        if (showSpotLightSettings) {
            ImGui::Text("SpotLight_color_picker");
            ImGui::PushItemWidth(-1);
            if (ImGui::ColorPicker4("##SpotLight_color_picker", (float*)&colorSpotLight, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs)) {
                spotLight.setColor(colorSpotLight.x, colorSpotLight.y, colorSpotLight.z);
                shader.modifyUniform<glm::vec3>("spotlight.color", spotLight.getColor());
            }

            ImGui::Text("SpotLight_specular_slider");
            if (ImGui::SliderFloat("##SpotLight_specular_slider", &specularSpotLight, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
                spotLight.setSpecular(specularSpotLight, specularSpotLight, specularSpotLight);
                shader.modifyUniform<glm::vec3>("spotlight.specular", spotLight.getSpecular());
            }

            ImGui::Text("SpotLight_diffuse_slider");
            if (ImGui::SliderFloat("##SpotLight_diffuse_slider", &diffuseSpotLight, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
                spotLight.setDiffuse(diffuseSpotLight, diffuseSpotLight, diffuseSpotLight);
                shader.modifyUniform<glm::vec3>("spotlight.diffuse", spotLight.getDiffuse());
            }

            ImGui::Text("SpotLight_cutOff_slider");
            if (ImGui::SliderFloat("##SpotLight_cutOff_slider", &spotLightInnerCutOff, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
                spotLight.setInnerCutOff(glm::cos(glm::radians(spotLightInnerCutOff)));
                spotLight.setOuterCutOff(glm::cos(glm::radians(spotLightInnerCutOff * 1.4)));
                shader.modifyUniform<float>("spotlight.innerCutOff", spotLight.getInnerCutOff());
                shader.modifyUniform<float>("spotlight.outerCutOff", spotLight.getOuterCutOff());

            }
            ImGui::PopItemWidth();
        }


        if (ImGui::Button("Close"))
            showSettingsWindow = false;

        ImGui::End();
    }


    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}