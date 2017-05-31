#include "mypaintgui.h"
#include <mypaint.h>
#include <event_manager/eventmanager.h>
#include <image/image.h>
#include <tools/tool.h>
#include <image_formats/loader.h>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <tinyfiledialogs.h>
#include <stdexcept>
#include <string>

namespace mp
{

glm::vec2 CMyPaintGUI::cameraPosition = glm::vec2(40.0f, -60.0f);

CMyPaintGUI::CMyPaintGUI(CMyPaint* mdl)
{
    if(!mdl)
    {
        throw std::logic_error("GUI has no model");
    }
    model = mdl;

    imageSize = model->GetCurrentImage()->GetSize();
    brushSize = model->GetBrushSize();
    brushColor = glm::vec3(model->GetBrushColor()) / 255.0f;

    if(!glfwInit())
    {
        throw std::logic_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(800, 600, "My Paint", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::logic_error("Failed to create window");
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    ImGui_ImplGlfw_Init(window, true);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glfwSetCursorPosCallback(window, CMyPaintGUI::MousePosCallback);
    glfwSetMouseButtonCallback(window, CMyPaintGUI::MouseCallback);
    glfwSetFramebufferSizeCallback(window, CMyPaintGUI::ResizeCallback);
    ResizeCallback(nullptr, 800, 600);

    CEventManager::GetInstance().SubscribeFor("ImageUpdated", this);

    FillLoadedTools();
    FillLoadedImgLoaders();

    CreateTexture();
    UpdateTexture();
}

CMyPaintGUI::~CMyPaintGUI()
{
    DestroyTexture();

    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    for(char* toolName : loadedToolsNames)
    {
        delete[] toolName;
    }

    for(char* loader : loadedImageLoaders)
    {
        delete[] loader;
    }
}

void CMyPaintGUI::ResizeCallback(GLFWwindow *window, int width, int height)
{
    (void*)window;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)width, -(double)height, 0.0, 1.0, 100.0);
}

void CMyPaintGUI::MouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    (void*)window;
    (void*)mods;
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse)
    { //event is handled by gui
        return;
    }

    CEvent mouseBtnEvent("MouseButton");

    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseBtnEvent.button = CEvent::MB_L;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseBtnEvent.button = CEvent::MB_R;
            break;
        default: return;
    }

    switch (action)
    {
        case GLFW_PRESS:
            mouseBtnEvent.type = CEvent::ET_MOUSE_PRESS;
            break;
        case GLFW_RELEASE:
            mouseBtnEvent.type = CEvent::ET_MOUSE_RELEASE;
            break;
        default: return;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glm::vec2 worldCoords = MousePosToWorldCoords(glm::vec2(xpos, ypos));
    mouseBtnEvent.mouseX = worldCoords.x;
    mouseBtnEvent.mouseY = worldCoords.y;

    CEventManager::GetInstance().SendEvent(mouseBtnEvent);
}

void CMyPaintGUI::MousePosCallback(GLFWwindow *window, double x, double y)
{
    (void*)window;
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse)
    { //event is handled by gui
        return;
    }

    CEvent mousePosEvent("MousePosition");
    mousePosEvent.type = CEvent::ET_MOUSE_MOVE;
    glm::vec2 worldCoords = MousePosToWorldCoords(glm::vec2(x, y));
    mousePosEvent.mouseX = worldCoords.x;
    mousePosEvent.mouseY = worldCoords.y;

    CEventManager::GetInstance().SendEvent(mousePosEvent);
}

void CMyPaintGUI::DrawGUI()
{
    static bool showHelp = false;
    static bool showToolbar = true;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open"))
            {
                OpenImage();
                imageSize = model->GetCurrentImage()->GetSize();
            }
            if (ImGui::MenuItem("Save As..."))
            {
                SaveImage();
            }
            if (ImGui::MenuItem("Exit"))
            {
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "", nullptr, model->CanUndo()))
            {
                model->Undo();
                imageSize = model->GetCurrentImage()->GetSize();
            }
            if (ImGui::MenuItem("Redo", "", nullptr, model->CanRedo()))
            {
                model->Redo();
                imageSize = model->GetCurrentImage()->GetSize();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Toolbar", "", &showToolbar, true);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            showHelp = ImGui::MenuItem("About...");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if(showToolbar)
    {
        showToolbar = DrawToolbar();
    }

    if(showHelp)
    {
        showHelp = DrawHelpDialog();
    }
}

bool CMyPaintGUI::DrawToolbar()
{
    bool result = true;
    ImGui::SetNextWindowPos(ImVec2(600, 100), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_Once);
    ImGui::Begin("Toolbar", &result);

    ImGui::PushItemWidth(-80);

    const char** items = const_cast<const char**>(loadedToolsNames.data());
    if (ImGui::ListBox("Tools", &selectedToolIndex, items, loadedToolsNames.size(), loadedToolsNames.size()))
    {
        model->SetCurrentTool(selectedToolIndex);
    }

    if (ImGui::SliderInt("Brush Size", &brushSize, 1, 20))
    {
        model->SetBrushSize(brushSize);
    }

    if (ImGui::ColorEdit3("Brush Color", &brushColor.x))
    {
        model->SetBrushColor(glm::uvec3(brushColor * 255.0f));
    }

    ImGui::DragInt2("", &imageSize.x, 1.0f, 1, 1024);
    if (ImGui::Button("Resize"))
    {
        imageSize.x = glm::clamp(imageSize.x, 1, 1024);
        imageSize.y = glm::clamp(imageSize.y, 1, 1024);
        model->ResizeImage(glm::uvec2(imageSize));
    }

	if (ImGui::Button("Undo"))
	{
		if (model->CanUndo())
		{
			model->Undo();
			imageSize = model->GetCurrentImage()->GetSize();
		}
	}
	if (ImGui::Button("Redo"))
	{
		if (model->CanRedo())
		{
			model->Redo();
			imageSize = model->GetCurrentImage()->GetSize();
		}
	}

    ImGui::End();
    return result;
}

bool CMyPaintGUI::DrawHelpDialog()
{
    bool result = true;
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
    ImGui::Begin("MyPaint", &result);

    ImGui::Text("(c) Serov Alexey");

    ImGui::End();
    return result;
}

void CMyPaintGUI::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();

        DrawGUI();

        DrawScene();

        ImGui::Render();
        glfwSwapBuffers(window);
    }
}

void CMyPaintGUI::FillLoadedTools()
{
    const CMyPaint::tools_t& loadedTools = model->GetTools();

    for(const CMyPaint::toolPtr& tool : loadedTools)
    {
        loadedToolsNames.push_back(new char[tool->GetName().size() + 1]);
        std::strcpy(loadedToolsNames.back(), tool->GetName().c_str());
    }
}

void CMyPaintGUI::FillLoadedImgLoaders()
{
    const CMyPaint::loaders_t& loadedImgLoaders = model->GetImageLoaders();

    for(const CMyPaint::loaderPtr& loader : loadedImgLoaders)
    {
        loadedImageLoaders.push_back(new char[loader->GetFormat().size() + 3]);
        loadedImageLoaders.back()[0] = '*';
        loadedImageLoaders.back()[1] = '.';
        std::strcpy(loadedImageLoaders.back() + 2, loader->GetFormat().c_str());
    }
}

glm::vec2 CMyPaintGUI::MousePosToWorldCoords(const glm::vec2 &posWndCoords)
{
    return glm::vec2(posWndCoords.x - cameraPosition.x,
                     posWndCoords.y + cameraPosition.y);
}

void CMyPaintGUI::DrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(cameraPosition.x, cameraPosition.y, 0.0f);

    const glm::uvec2 imgSize = model->GetCurrentImage()->GetSize();

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, -10.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.0f, -(float)imgSize.y, -10.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f((float)imgSize.x, -(float)imgSize.y, -10.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f((float)imgSize.x, 0.0f, -10.0f);
    glEnd();
}

void CMyPaintGUI::CreateTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void CMyPaintGUI::DestroyTexture()
{
    glDeleteTextures(1, &texture);
}

void CMyPaintGUI::UpdateTexture()
{
    const auto& image = model->GetCurrentImage();
    const glm::uvec2 imgSize = image->GetSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgSize.x, imgSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetRawPixels().get());
}

void CMyPaintGUI::OpenImage()
{
    const char* result = tinyfd_openFileDialog("Open image" ,"./", loadedImageLoaders.size(), loadedImageLoaders.data(), nullptr, 0);
    if(result)
    {
        model->OpenImage(result);
    }
}

void CMyPaintGUI::SaveImage()
{
    const char *result = tinyfd_saveFileDialog("Save image", "./", loadedImageLoaders.size(), loadedImageLoaders.data(), nullptr);
    if (result)
    {
        model->SaveImage(result);
    }
}

void CMyPaintGUI::ReceiveEvent(const CEvent &event)
{
    switch(event.type)
    {
    case CEvent::ET_USER :
        if(event.name == "ImageUpdated")
        {
            UpdateTexture();
        }
        break;
    default : break;
    }
}

}
