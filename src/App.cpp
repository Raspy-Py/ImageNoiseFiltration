#include "App.h"
#include "GaussianNoiser.h"
#include <random>

App::App()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

m_Window = std::make_unique<Window>(1280, 720, "ImageNoiseFiltering");
m_OriginalImage = std::make_unique<Texture>("./data/images/dog.jpg");
m_NoisyImage = std::make_unique<Texture>(*m_OriginalImage.get());
m_DenoisedImage = std::make_unique<Texture>(*m_OriginalImage.get());

IMGUI_CHECKVERSION();
ImGui::CreateContext();
m_ImGuiIO = &ImGui::GetIO();
m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
ImGui::StyleColorsDark();
ImGui_ImplGlfw_InitForOpenGL(m_Window->Get(), true);
ImGui_ImplOpenGL3_Init("#version 430");


m_Mean = 0.0;
m_StandartDeviation = 15.0;
/*
* Implement and than instantiate here image filters
* like this:
*
* m_DefaultDenoiser = std::make_unique<FancyDenoiserName>(args...);
*/
m_DefaultNoiser = std::make_unique<GaussianNoiser>(m_Mean, m_StandartDeviation);

/*
* Transform our images here
*
* m_DefaultDenoiser->TransformFrom(m_DenoisedImage.get(), m_NoisyImage.get());
*
* m_DenoisedImage->Update();
*/
m_DefaultNoiser->Transform(m_NoisyImage.get());
m_NoisyImage->Update();
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
    ImVec2 display = ImGui::GetIO().DisplaySize;
    ImVec2 imageSize(display.x * 0.65, display.x * 0.65 / (float)m_OriginalImage->width * (float)m_OriginalImage->height);

    // Content window
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(display.x * 0.7, display.y));
    ImGui::Begin("Content", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    //ImGui::Text("Original image");
    //ImGui::Image((void*)(intptr_t)m_OriginalImage->GetID(), imageSize);
    ImGui::Text("Noisy image");
    ImGui::Image((void*)(intptr_t)m_NoisyImage->GetID(), imageSize);
    //ImGui::Text("Denoised image");
    //ImGui::Image((void*)(intptr_t)m_DenoisedImage->GetID(), imageSize);
    ImGui::End();

    // Settings window
    ImGui::SetNextWindowPos(ImVec2(display.x * 0.7, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(display.x * 0.3, display.y));
    ImGui::Begin("Settings", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::SliderFloat("Standart deviation", &m_StandartDeviation, 0.0f, 50.0f);
    if (ImGui::Button("Regenerate noise"))
    {
        m_DefaultNoiser->SetParam("standartDeviation", m_StandartDeviation);
        m_DefaultNoiser->TransformFrom(m_NoisyImage.get(), m_OriginalImage.get());
        m_NoisyImage->Update();
    }
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
