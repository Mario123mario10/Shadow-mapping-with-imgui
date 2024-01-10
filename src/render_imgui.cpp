
#include <memory>
#include <functional>
#include <texture.h>
#include <shader.h>
#include <light.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void updateShader(const Shader& shader, const PerspectiveLight& pointLight, const PerspectiveLight& standingLight, const SpotLight& spotLight);
void addSlider(const char* id, const char* text, float& sliderVariable, Light& light, void(Light::* setter)(float, float, float));
void addSlider(const char* id, const char* text, float& sliderVariable, Light& light, void(Light::* setter)(float), std::function<float(float)> lambda = [](float x) { return x; });


void renderImgui(int screenWidth, int screenHeight, std::shared_ptr<Texture2D> windowTexture, const Shader& shader, PerspectiveLight& pointLight, PerspectiveLight& standingLight, SpotLight& spotLight) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool showSettingsWindow = false;
    static bool showPointLightSettings = false;
    static bool showSpotLightSettings = false;
    static bool showStandingLightSettings = false;

    static bool alpha_preview = false;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = false;
    static bool options_menu = false;
    static bool hdr = false;

    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);


    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("The Game");

    if (ImGui::Button("Options")) {
        showSettingsWindow = !showSettingsWindow;
    }

    ImGui::Image((void*)(intptr_t)windowTexture->getId(), ImVec2(screenWidth, screenHeight), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    if (showSettingsWindow) {
        ImGui::SetNextWindowSize(ImVec2(325, 940), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(0, 50));

        ImGui::Begin("OpenGL Settings");
        shader.use();

        static glm::vec3 _colorPointLight = pointLight.getColor();
        static glm::vec3 _colorSpotLight = spotLight.getColor();
        static glm::vec3 _colorStandingLight = standingLight.getColor();

        static ImVec4 colorPointLight = ImVec4(_colorPointLight.x, _colorPointLight.y, _colorPointLight.z, 0.8f);
        static ImVec4 colorSpotLight = ImVec4(_colorSpotLight.x, _colorSpotLight.y, _colorSpotLight.z, 0.8f);
        static ImVec4 colorStandingLight = ImVec4(_colorStandingLight.x, _colorStandingLight.y, _colorStandingLight.z, 0.8f);

        static float specularPointLight = pointLight.getSpecular().x;
        static float diffusePointLight = pointLight.getDiffuse().x;
        static glm::vec3 attenuationPointLight = pointLight.getAttenuation();

        static float specularStandingLight = standingLight.getSpecular().x;
        static float diffuseStandingLight = standingLight.getDiffuse().x;
        static glm::vec3 attenuationStandingLight = standingLight.getAttenuation();

        static float specularSpotLight = spotLight.getSpecular().x;
        static float diffuseSpotLight = spotLight.getDiffuse().x;
        static glm::vec3 attenuationSpotLight = spotLight.getAttenuation();
        static float spotLightInnerCutOff = spotLight.getInnerCutOff();
        static float spotLightOuterCutOff = spotLight.getOuterCutOff();
        


        if (ImGui::Button("Point Light Settings")) {
            showPointLightSettings = !showPointLightSettings;
        }

        if (showPointLightSettings) {
            ImGui::Text("PointLight color");
            ImGui::PushItemWidth(-1);
            if (ImGui::ColorPicker4("##PointLight_color_picker", (float*)&colorPointLight, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs)) {
                pointLight.setColor(colorPointLight.x, colorPointLight.y, colorPointLight.z);
            }

            addSlider("##PointLight_specular_slider", "PointLight specular", specularPointLight, pointLight, &PerspectiveLight::setSpecular);
            addSlider("##PointLight_diffuse_slider", "PointLight diffuse", diffusePointLight, pointLight, &PerspectiveLight::setDiffuse);
            addSlider("##PointLight_attenuation_constant_slider", "PointLight attenuation constant", attenuationPointLight.x, pointLight, &PerspectiveLight::setAttenuationConstantFactor);
            addSlider("##PointLight_attenuation_linear_slider", "PointLight attenuation linear", attenuationPointLight.y, pointLight, &PerspectiveLight::setAttenuationLinearFactor);
            addSlider("##PointLight_attenuation_quadratic_slider", "PointLight attenuation quadratic", attenuationPointLight.z, pointLight, &PerspectiveLight::setAttenuationQuadraticFactor);

            ImGui::PopItemWidth();
        }

        if (ImGui::Button("Standing Light Settings")) {
            showStandingLightSettings = !showStandingLightSettings;
        }

        if (showStandingLightSettings) {
            ImGui::Text("StandingLight color");
            ImGui::PushItemWidth(-1);
            if (ImGui::ColorPicker4("##StandingLight_color_picker", (float*)&colorStandingLight, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs)) {
                standingLight.setColor(colorStandingLight.x, colorStandingLight.y, colorStandingLight.z);
            }

            addSlider("##StandingLight_specular_slider", "StandingLight specular", specularStandingLight, standingLight, &PerspectiveLight::setSpecular);
            addSlider("##StandingLight_diffuse_slider", "StandingLight diffuse", diffuseStandingLight, standingLight, &PerspectiveLight::setDiffuse);
            addSlider("##StandingLight_attenuation_constant_slider", "StandingLight attenuation constant", attenuationStandingLight.x, standingLight, &PerspectiveLight::setAttenuationConstantFactor);
            addSlider("##StandingLight_attenuation_linear_slider", "StandingLight attenuation linear", attenuationStandingLight.y, standingLight, &PerspectiveLight::setAttenuationLinearFactor);
            addSlider("##StandingLight_attenuation_quadratic_slider", "StandingLight attenuation quadratic", attenuationStandingLight.z, standingLight, &PerspectiveLight::setAttenuationQuadraticFactor);

            ImGui::PopItemWidth();
        }

        if (ImGui::Button("Spot Light Settings")) {
            showSpotLightSettings = !showSpotLightSettings;
        }

        if (showSpotLightSettings) {
            ImGui::Text("SpotLight color");
            ImGui::PushItemWidth(-1);
            if (ImGui::ColorPicker4("##SpotLight_color_picker", (float*)&colorSpotLight, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs)) {
                spotLight.setColor(colorSpotLight.x, colorSpotLight.y, colorSpotLight.z);
            }

            auto lambdaCos = [](float x) {return glm::cos(glm::radians(x)); };

            addSlider("##SpotLight_specular_slider", "SpotLight specular", specularSpotLight, spotLight, &SpotLight::setSpecular);
            addSlider("##SpotLight_diffuse_slider", "SpotLight diffuse", diffuseSpotLight, spotLight, &SpotLight::setDiffuse);
            addSlider("##SpotLight_attenuation_constant_slider", "SpotLight attenuation constant", attenuationSpotLight.x, spotLight, &SpotLight::setAttenuationConstantFactor);
            addSlider("##SpotLight_attenuation_linear_slider", "SpotLight attenuation linear", attenuationSpotLight.y, spotLight, &SpotLight::setAttenuationLinearFactor);
            addSlider("##SpotLight_attenuation_quadratic_slider", "SpotLight attenuation quadratic", attenuationSpotLight.z, spotLight, &SpotLight::setAttenuationQuadraticFactor);
            addSlider("##SpotLight_inner_cutOff_slider", "SpotLight Inner cutOff", spotLightInnerCutOff, spotLight, &Light::setInnerCutOff, lambdaCos);
            addSlider("##SpotLight_outer_cutOff_slider", "SpotLight Outer cutOff", spotLightOuterCutOff, spotLight, &Light::setOuterCutOff, lambdaCos);

            ImGui::PopItemWidth();
        }


        if (ImGui::Button("Close"))
            showSettingsWindow = false;

        ImGui::End();
    }

    updateShader(shader, pointLight, standingLight, spotLight);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    
}

void updateShader(const Shader& shader, const PerspectiveLight& pointLight, const PerspectiveLight& standingLight, const SpotLight& spotLight) {
    shader.modifyUniform<glm::vec3>("pointLight[0].color", pointLight.getColor());
    shader.modifyUniform<glm::vec3>("pointLight[0].specular", pointLight.getSpecular());
    shader.modifyUniform<glm::vec3>("pointLight[0].diffuse", pointLight.getDiffuse());
    shader.modifyUniform<float>("pointLight[0].constant", pointLight.getAttenuationConstantFactor());
    shader.modifyUniform<float>("pointLight[0].linear", pointLight.getAttenuationLinearFactor());
    shader.modifyUniform<float>("pointLight[0].quadratic", pointLight.getAttenuationQuadraticFactor());

    shader.modifyUniform<glm::vec3>("pointLight[1].color", standingLight.getColor());
    shader.modifyUniform<glm::vec3>("pointLight[1].specular", standingLight.getSpecular());
    shader.modifyUniform<glm::vec3>("pointLight[1].diffuse", standingLight.getDiffuse());
    shader.modifyUniform<float>("pointLight[1].constant", standingLight.getAttenuationConstantFactor());
    shader.modifyUniform<float>("pointLight[1].linear", standingLight.getAttenuationLinearFactor());
    shader.modifyUniform<float>("pointLight[1].quadratic", standingLight.getAttenuationQuadraticFactor());

    shader.modifyUniform<glm::vec3>("spotlight.color", spotLight.getColor());
    shader.modifyUniform<glm::vec3>("spotlight.specular", spotLight.getSpecular());
    shader.modifyUniform<glm::vec3>("spotlight.diffuse", spotLight.getDiffuse());
    shader.modifyUniform<float>("spotlight.constant", spotLight.getAttenuationConstantFactor());
    shader.modifyUniform<float>("spotlight.linear", spotLight.getAttenuationLinearFactor());
    shader.modifyUniform<float>("spotlight.quadratic", spotLight.getAttenuationQuadraticFactor());
    shader.modifyUniform<float>("spotlight.innerCutOff", spotLight.getInnerCutOff());
    shader.modifyUniform<float>("spotlight.outerCutOff", spotLight.getOuterCutOff());
}

void addSlider(const char* id, const char* text, float& sliderVariable, Light& light, void(Light::*setter)(float, float, float) ) {

    ImGui::Text(text);
    if (ImGui::SliderFloat(id, &sliderVariable, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
        (light.*setter)(sliderVariable, sliderVariable, sliderVariable);
    }
}

void addSlider(const char* id, const char* text, float& sliderVariable, Light& light, void(Light::* setter)(float), std::function<float(float)> lambda) {

    ImGui::Text(text);
    if (ImGui::SliderFloat(id, &sliderVariable, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
        (light.*setter)(lambda(sliderVariable));
    }
}
