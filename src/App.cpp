#include "App.h"
#include <random>

App::App()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = std::make_unique<Window>(1280, 720, "ImageNoiseFiltering");
    m_OriginalImage = std::make_unique<Texture>("./data/images/cat.jpg");
    m_NoisyImage = std::make_unique<Texture>(*m_OriginalImage.get());
    m_DenoisedImage = std::make_unique<Texture>(*m_OriginalImage.get());
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_ImGuiIO = &ImGui::GetIO();
    m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_Window->Get(), true);
    ImGui_ImplOpenGL3_Init("#version 430");

    /*
    * Implement and than instantiate here image filters
    * like this:
    * 
    * m_DefaultNoiser = std::make_unique<FancyNoiserName>(args...);
    * m_DefaultDenoiser = std::make_unique<FancyDenoiserName>(args...);
    */

    /*
    * Transform our images here
    *
    * m_DefaultNoiser->Transform(m_NoisyImage.get());
    * m_DefaultDenoiser->Transform(m_DenoisedImage.get());
    *
    * m_NoisyImage->Update();
    * m_DenoisedImage->Update();
    */
}

App::~App()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
    m_Window->Clear(0.3f, 0.5f, 0.9f);

    /*
    * Draw OpenGL stuff here
    */

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    /*
    * Draw ImGui stuff here
    */
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Window with cat", (bool*)0, ImGuiWindowFlags_NoDecoration);
    ImGui::Text("Original image");
    ImGui::Image((void*)(intptr_t)m_OriginalImage->GetID(), ImVec2(640, 480));
    ImGui::Text("Noisy image");
    ImGui::Image((void*)(intptr_t)m_NoisyImage->GetID(), ImVec2(640, 480));
    ImGui::Text("Denoised image");
    ImGui::Image((void*)(intptr_t)m_DenoisedImage->GetID(), ImVec2(640, 480));
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
