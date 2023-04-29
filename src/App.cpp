#include "App.h"
#include "GaussianNoiser.h"
#include "Utils.h"
#include <random>

#include "imgui/imgui.h"

App::App()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Most of the magic numbers gethered here
    m_Settings = { 
        .noiseMean = 0.0f,
        .noiseStadDev = 10.0f,
        .noiseStadDevMax = 30.0f,
        .relImageWidth = 0.675f,
        .relContentWindowWidth = 0.7f,
        .relSettingsWindowWidth = 0.3f,
    };

    m_Window = std::make_unique<Window>(1280, 720, "ImageNoiseFiltering");
    m_OriginalImage = std::make_unique<Texture>("./data/images/dog.jpg");
    m_NoisyImage = std::make_unique<Texture>(*m_OriginalImage.get());

    /*
    * Implement and than instantiate here image filters
    * like this:
    *
    * m_DefaultDenoiser = std::make_unique<FancyDenoiserName>(args...);
    */
    m_DefaultNoiser = std::make_unique<GaussianNoiser>(m_Settings.noiseMean, m_Settings.noiseStadDev);


    /*
    * Transform our images here
    *
    * m_DefaultDenoiser->TransformFrom(m_DenoisedImage.get(), m_NoisyImage.get());
    *
    * m_DenoisedImage->Update();
    */
    m_DefaultNoiser->Transform(m_NoisyImage.get());
    m_NoisyImage->Update();

    m_DenoisedImage = std::make_unique<Texture>(*m_NoisyImage.get());

    m_Menu = std::make_unique<Menu>(m_Window->Get());
    // Some hacky-packy
    m_Menu->Render();
    m_ImGuiIO = &ImGui::GetIO();
    ImVec2 display = m_ImGuiIO->DisplaySize;
    float imageAR = (float)m_OriginalImage->width / (float)m_OriginalImage->height;
    ImVec2 imageSize(display.x * m_Settings.relImageWidth, 
                     display.x * m_Settings.relImageWidth / imageAR);

    m_Menu->AddWindow("Content", ImVec2(0, 0), ImVec2(display.x * m_Settings.relContentWindowWidth, display.y))
        .AddText("Original image")
        .AddImage("original_image", m_OriginalImage->GetID(), imageSize)
        .AddText("Noisy image")
        .AddImage("noisy_image", m_NoisyImage->GetID(), imageSize)
        .AddText("Denoised image")
        .AddImage("denoised_image", m_DenoisedImage->GetID(), imageSize);

    m_Menu->AddWindow("Settings", 
            ImVec2(display.x * m_Settings.relContentWindowWidth, 0), 
            ImVec2(display.x * m_Settings.relSettingsWindowWidth, display.y))
        .AddSliderFloat("Noise level", &m_Settings.noiseStadDev, 0.0f, m_Settings.noiseStadDevMax)
        .AddButton("Regenerate noise", [this] {
            m_DefaultNoiser->SetParam("standartDeviation", m_Settings.noiseStadDev);
            m_DefaultNoiser->TransformFrom(m_NoisyImage.get(), m_OriginalImage.get());
            m_NoisyImage->Update();
            m_OriginalImage->Update();
        });

    Window::SetDropCallBack([this](GLFWwindow* window, int count, const char** paths) {
        if (auto dropedFile = m_Window->TryPopDropedFile())
        {
            m_OriginalImage.reset(new Texture(dropedFile.value().c_str()));

            m_NoisyImage.reset(new Texture(*m_OriginalImage.get()));
            m_DefaultNoiser->TransformFrom(m_NoisyImage.get(), m_OriginalImage.get());

            m_DenoisedImage.reset(new Texture(*m_NoisyImage.get()));
            // TODO: apply filtering to m_DenoisedImage

            ImVec2 display = m_ImGuiIO->DisplaySize;
            float imageAR = (float)m_OriginalImage->width / (float)m_OriginalImage->height;
            ImVec2 imageSize(display.x * m_Settings.relImageWidth, display.x * m_Settings.relImageWidth / imageAR);

            m_Menu->AlterWindow("Content")
                .AddImage("original_image", m_OriginalImage->GetID(), imageSize)
                .AddImage("noisy_image",    m_NoisyImage->GetID(),    imageSize)
                .AddImage("denoised_image", m_DenoisedImage->GetID(), imageSize);

            m_OriginalImage->Update();
            m_NoisyImage->Update();
            m_DenoisedImage->Update();
        }});
    Window::SetFramebufferSizeCallBack([this](GLFWwindow* window, int width, int height) {
        float imageAR = (float)m_OriginalImage->width / (float)m_OriginalImage->height;
        ImVec2 imageSize(width * m_Settings.relImageWidth, width * m_Settings.relImageWidth / imageAR);

        m_Menu->AlterWindow("Content")
            .SetSize(ImVec2(width * m_Settings.relContentWindowWidth, height));
        m_Menu->AlterWindow("Content")
            .SetPosition(ImVec2(0, 0));
        m_Menu->AlterWindow("Content")
            .AddImage("original_image", m_OriginalImage->GetID(), imageSize)
            .AddImage("noisy_image",    m_NoisyImage->GetID(),    imageSize)
            .AddImage("denoised_image", m_DenoisedImage->GetID(), imageSize);

        m_Menu->AlterWindow("Settings")
            .SetSize(ImVec2(width * m_Settings.relSettingsWindowWidth, height));
        m_Menu->AlterWindow("Settings")
            .SetPosition(ImVec2(width * m_Settings.relContentWindowWidth, 0));
        });

    // ImGui style
    Menu::EmbraceTheDarkness();
}

App::~App()
{
    glfwTerminate();
}

void App::Run()
{
    m_Clock.Restart();
    while (m_Window->Open())
    {
        float deltaTime = m_Clock.Stamp();

        // Processing user input
        ProcessEvents(deltaTime);

        // Rendering stuff goes here
        DoFrame(deltaTime);

        PollEvents(deltaTime);
        m_Window->SwapBuffers();
    }
}

void App::DoFrame(float dt)
{
    m_Window->Clear(0.105f, 0.105f, 0.105f);

    // Draw OpenGL stuff here
    

    // Draw ImGui stuff here
    m_Menu->Render();

    m_FrameCounter++;
}

void App::ProcessEvents(float dt)
{   
    // Keyboard input
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        m_Window->Close();

    // Ignore mouse input when it's captured by ImGui
    if (m_ImGuiIO->WantCaptureMouse)
        return;

    // Mouse input
}

void App::PollEvents(float dt)
{
    glfwPollEvents();
}
