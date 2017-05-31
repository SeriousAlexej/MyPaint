#include "mypaint.h"
#include "event_manager/eventmanager.h"
#include "tools/tool.h"
#include "image_formats/loader.h"
#include "dirtools/dirtools.h"
#include "plugin_loaders/pluginLoader.h"
#include <glm/common.hpp>
#include <iostream>

namespace mp
{

CMyPaint::CMyPaint(const std::string& executablePath) :
    currentImage(new CImage(glm::uvec2(500, 500)))
{
    SetExecutablePath(executablePath);
    LoadPlugins();
    CEventManager::GetInstance().SubscribeFor("MouseButton", this);
    CEventManager::GetInstance().SubscribeFor("MousePosition", this);
}

CMyPaint::~CMyPaint()
{
    tools.clear();
    imageLoaders.clear();
    plugins.clear();
}

void CMyPaint::SetExecutablePath(const std::string &fullpath)
{
    pathToExecutable = fullpath;

    auto foundPos = pathToExecutable.find_last_of(dir::GetDirDelimiter());
    if(foundPos != std::string::npos)
    {
        pathToExecutable = pathToExecutable.substr(0, foundPos+1);
    }
}

CImageLoader* CMyPaint::LoaderForFormat(const std::string &format) const
{
    for(const loaderPtr& loader : imageLoaders)
    {
        if(loader->GetFormat() == format)
        {
            return loader.get();
        }
    }
    std::cerr << "Error! Cannot find proper loader for image format!\n\tHint: Write filename extension manually!\n";
    return nullptr;
}

void CMyPaint::OpenImage(const std::string &path)
{
    if(path.length() < 3)
    {
        return;
    }
    CImageLoader* loader = LoaderForFormat(path.substr(path.length() - 3));
    if(!loader)
    {
        return;
    }
    CImage* newImage = loader->Read(path);
    if(newImage)
    {
        ClearHistory();
        currentImage.reset(newImage);
        CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));
    }
}

void CMyPaint::SaveImage(const std::string &path) const
{
    if(!currentImage || path.length() < 3)
    {
        return;
    }

    CImageLoader* loader = LoaderForFormat(path.substr(path.length() - 3));
    if(!loader)
    {
        return;
    }
    loader->Write(path, currentImage.get());
}

bool CMyPaint::CanUndo() const
{
    return !undoStack.empty();
}

bool CMyPaint::CanRedo() const
{
    return !redoStack.empty();
}

void CMyPaint::Undo()
{
    if(!CanUndo())
    {
        return;
    }

    redoStack.emplace_back(std::move(currentImage));
    imagePtr& undoImg = undoStack.front();
    currentImage = std::move(undoImg);
    undoStack.pop_front();

    CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));
}

void CMyPaint::Redo()
{
    if(!CanRedo())
    {
        return;
    }

    undoStack.emplace_front(std::move(currentImage));
    imagePtr& redoImg = redoStack.back();
    currentImage = std::move(redoImg);
    redoStack.pop_back();

    CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));
}

void CMyPaint::PushImageToHistory()
{
    if(undoStack.size() == maxUndoSteps)
    {
        undoStack.pop_back();
    }

    undoStack.emplace_front(imagePtr(new CImage(*currentImage)));

    redoStack.clear();
}

void CMyPaint::ClearHistory()
{
    undoStack.clear();
    redoStack.clear();
}

void CMyPaint::ResizeImage(const glm::uvec2 &newSize)
{
    if(newSize == currentImage->GetSize())
    {
        return;
    }

    PushImageToHistory();

    currentImage.reset(new CImage(newSize));

    currentImage->CopyImageDataFrom(*undoStack.front());

    CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));
}

void CMyPaint::LoadPlugins()
{
    typedef CTool*(*toolFactoryPtr)(CMyPaint*);
    typedef CImageLoader*(*loaderFactoryPtr)();

    //search for libraries in 'plugins' subdirectory
    auto libraries = dir::GetLibrariesInDir(pathToExecutable + "plugins" + dir::GetDirDelimiter());
    for(const std::string& str : libraries)
    {
        try
        {
            pluginPtr plugin(new CPluginLoader(pathToExecutable + "plugins" + dir::GetDirDelimiter() + str));

            toolFactoryPtr*   toolFactories   = (toolFactoryPtr*)plugin->FindSymbol("plugin_ToolFactories");
            loaderFactoryPtr* loaderFactories = (loaderFactoryPtr*)plugin->FindSymbol("plugin_LoaderFactories");
            int* numTools   = (int*)plugin->FindSymbol("toolFactoryNumElements");
            int* numLoaders = (int*)plugin->FindSymbol("loaderFactoryNumElements");

            plugins.emplace_back(std::move(plugin));

            std::cout << "Loading from module " << str << "\n";

            for(int i = 0; i < *numTools; i++)
            {
                tools.push_back(std::unique_ptr<CTool>(toolFactories[i](this)));
                std::cout << "\tTool: " << tools.back()->GetName() << "\n";
            }
            for(int i = 0; i < *numLoaders; i++)
            {
                imageLoaders.push_back(std::unique_ptr<CImageLoader>(loaderFactories[i]()));
                std::cout << "\tImage loader: " << imageLoaders.back()->GetFormat() << "\n";
            }

        } catch(std::logic_error& e)
        {
            std::cerr << "Cannot load from module " << str << ": " << e.what() << std::endl;
        }
    }
    if(tools.size() > 0)
    {
        currentTool = tools[0].get();
    }
}

const CMyPaint::imagePtr& CMyPaint::GetCurrentImage() const
{
    return currentImage;
}

CImage* CMyPaint::GetPreviousImage() const
{
    if(undoStack.empty())
    {
        return nullptr;
    }
    return undoStack.front().get();
}

CMyPaint::imagePtr& CMyPaint::GetCurrentImage()
{
    return currentImage;
}

const CMyPaint::tools_t& CMyPaint::GetTools() const
{
    return tools;
}

const CMyPaint::loaders_t& CMyPaint::GetImageLoaders() const
{
    return imageLoaders;
}

const CTool* CMyPaint::GetCurrentTool() const
{
    return currentTool;
}

glm::uvec3 CMyPaint::GetBrushColor() const
{
    return brushColor;
}

void CMyPaint::SetBrushColor(const glm::uvec3 &col)
{
    brushColor = col;
}

unsigned CMyPaint::GetBrushSize() const
{
    return brushSize;
}

void CMyPaint::SetBrushSize(unsigned brushSz)
{
    brushSz = glm::max(1u, brushSz);
    brushSize = brushSz;
}

void CMyPaint::SetCurrentTool(unsigned index)
{
    if(index >= tools.size())
    {
        return;
    }

    if(currentTool)
    {
        currentTool->SetInactive();
    }

    currentTool = tools[index].get();
    currentTool->SetActive();
}

void CMyPaint::ReceiveEvent(const CEvent &event)
{
    switch(event.type)
    {
    case CEvent::ET_MOUSE_PRESS :
        if(currentTool)
        {
            PushImageToHistory();
            currentTool->Begin(glm::ivec2(event.mouseX, event.mouseY));
        }
        break;
    case CEvent::ET_MOUSE_MOVE :
        if(currentTool)
        {
            currentTool->Update(glm::ivec2(event.mouseX, event.mouseY));
        }
        break;
    case CEvent::ET_MOUSE_RELEASE :
        if(currentTool)
        {
            currentTool->End(glm::ivec2(event.mouseX, event.mouseY));

            if(!undoStack.empty())
            {
                if(*undoStack.front() == *currentImage)
                { //image is the same, revert changes...
                    Undo();
                    redoStack.pop_back();
                    break;
                }
            }

            CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));
        }
        break;
    default : break;
    }
}

}
