#ifndef MY_PAINT_H
#define MY_PAINT_H
#include <vector>
#include <list>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include "event_manager/eventsubscriber.h"
#include "image/image.h"
#include "mypaint_api.h"

namespace mp
{

class CPluginLoader;
class CTool;
class CImageLoader;

/**
 * CMyPaint class stores images, undo history,
 * loads plugins and handles their execution.
 */
class MYPAINT_API CMyPaint : public CEventSubscriber
{
public:
    typedef std::unique_ptr<CTool> toolPtr;
    typedef std::vector<toolPtr> tools_t;
    typedef std::unique_ptr<CImage> imagePtr;
    typedef std::unique_ptr<CImageLoader> loaderPtr;
    typedef std::vector<loaderPtr> loaders_t;
    typedef std::unique_ptr<CPluginLoader> pluginPtr;
    typedef std::vector<pluginPtr> plugins_t;

    CMyPaint(const std::string& executablePath);
    ~CMyPaint();

    void             Undo();
    void             Redo();
    bool             CanUndo() const;
    bool             CanRedo() const;
    void             ClearHistory();

    void             OpenImage(const std::string& path);
    void             SaveImage(const std::string& path) const;

    void             ResizeImage(const glm::uvec2& newSize);

    void             SetCurrentTool(unsigned index);
    CImage*          GetPreviousImage() const; ///useful when painting over old images
    const imagePtr&  GetCurrentImage() const;
    imagePtr&        GetCurrentImage();
    const tools_t&   GetTools() const;
    const loaders_t& GetImageLoaders() const;
    const CTool*     GetCurrentTool() const;
    glm::uvec3       GetBrushColor() const;
    void             SetBrushColor(const glm::uvec3& col);
    unsigned         GetBrushSize() const;
    void             SetBrushSize(unsigned brushSz);

    void             ReceiveEvent(const CEvent &event) override;

private:
    /**
     * @brief SetExecutablePath
     * @param fullpath
     * Sets the 'CMyPaint::pathToExecutable' to absolute path to current workdir
     */
    void             SetExecutablePath(const std::string& fullpath);
    void             PushImageToHistory();
    void             LoadPlugins();
    CImageLoader*    LoaderForFormat(const std::string& format) const;

    static const int     maxUndoSteps = 10;

    glm::uvec3           brushColor = glm::uvec3(0u, 0u, 0u);
    unsigned             brushSize = 2;
    std::list<imagePtr>  undoStack;
    std::list<imagePtr>  redoStack;
    imagePtr             currentImage;
    tools_t              tools;
    loaders_t            imageLoaders;
    CTool*               currentTool = nullptr;
    plugins_t            plugins;
    std::string          pathToExecutable;
};

}
#endif
