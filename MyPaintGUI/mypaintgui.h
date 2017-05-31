#ifndef MY_PAINT_GUI_H
#define MY_PAINT_GUI_H
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <event_manager/eventsubscriber.h>
#include <mypaint_api.h>

struct GLFWwindow;

namespace mp
{

class CMyPaint;

/**
 * CMyPaintGUI class is a GUI for CMyPaint, it
 * renders interface and provides controls.
 */
class CMyPaintGUI : public CEventSubscriber
{
public:
    explicit CMyPaintGUI(CMyPaint* mdl);
    ~CMyPaintGUI();

    void Run();

    void ReceiveEvent(const CEvent &event) override;

private:
    static void ResizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void MousePosCallback(GLFWwindow* window, double x, double y);

    void CreateTexture();
    void DestroyTexture();
    void UpdateTexture();

    void OpenImage();
    void SaveImage();

    void DrawScene();
    void DrawGUI();
    bool DrawHelpDialog();
    bool DrawToolbar();

    void FillLoadedTools();
    void FillLoadedImgLoaders();

    static glm::vec2 MousePosToWorldCoords(const glm::vec2& posWndCoords);

    static glm::vec2   cameraPosition;
    glm::ivec2         imageSize;
    int                brushSize;
    glm::vec3          brushColor;
    int                selectedToolIndex = 0;
    std::vector<char*> loadedToolsNames;
    std::vector<char*> loadedImageLoaders;
    GLFWwindow*        window = nullptr;
    CMyPaint*          model = nullptr;
    unsigned int       texture;
};

}

#endif
