#include "toolEllipse.h"
#include <event_manager/eventmanager.h>
#include <mypaint.h>
#include <image/image.h>
#include <glm/common.hpp>

namespace mp
{

CEllipseTool::CEllipseTool(CMyPaint *ow) :
    CTool(ow)
{
    name = "Ellipse";
}

void CEllipseTool::Begin(const glm::ivec2 &mousePos)
{
    started = true;
    startPosition = mousePos;
    prevMousePosition = mousePos;
}

void CEllipseTool::GetDrawBBox(const glm::ivec2 &newPos, int &xStart, int &xEnd, int &yStart, int &yEnd) const
{
    xStart = glm::min(startPosition.x, newPos.x);
    xEnd   = glm::max(startPosition.x, newPos.x);
    yStart = glm::min(startPosition.y, newPos.y);
    yEnd   = glm::max(startPosition.y, newPos.y);
}

bool CEllipseTool::PointInsideEllipse(float x, float y, float eX, float eY, float rX, float rY)
{
    return (x - eX)*(x - eX)/(rX * rX) + (y - eY)*(y - eY)/(rY * rY) <= 1.0f;
}

void CEllipseTool::Update(const glm::ivec2 &mousePos)
{
    if(!started)
    {
        return;
    }

    unsigned brushSize = owner->GetBrushSize();
    int xStart, xEnd, yStart, yEnd;
    GetDrawBBox(prevMousePosition, xStart, xEnd, yStart, yEnd);
    xStart -= brushSize;
    xEnd   += brushSize;
    yStart -= brushSize;
    yEnd   += brushSize;

    CImage& img = *(owner->GetCurrentImage());
    CImage* prevImg = owner->GetPreviousImage();

    if(!prevImg)
    {
        return;
    }

    img.CopyRectFrom(*prevImg, glm::ivec2(xStart, yStart), glm::ivec2(xEnd, yEnd));

    DrawEllipseTo(mousePos);

    CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));

    prevMousePosition = mousePos;
}

void CEllipseTool::End(const glm::ivec2 &mousePos)
{
    started = false;
}

void CEllipseTool::DrawEllipseTo(const glm::ivec2 &mousePos)
{
    CImage& img = *(owner->GetCurrentImage());

    float brushSize = owner->GetBrushSize() * 0.5f;
    glm::uvec3 brushColor = owner->GetBrushColor();

    int xStart, xEnd, yStart, yEnd;
    GetDrawBBox(mousePos, xStart, xEnd, yStart, yEnd);

    float ellipseRadiusX = 0.5f * (xEnd - xStart);
    float ellipseRadiusY = 0.5f * (yEnd - yStart);
    float ellipseCenterX = 0.5f * (xEnd + xStart);
    float ellipseCenterY = 0.5f * (yEnd + yStart);

    xStart += -brushSize;
    xEnd   +=  brushSize;
    yStart += -brushSize;
    yEnd   +=  brushSize;

    for(int x = xStart; x <= xEnd; x++)
    for(int y = yStart; y <= yEnd; y++)
    {
        //point should be inside outer ellipse, but outside inner ellipse
        if( PointInsideEllipse(x, y, ellipseCenterX, ellipseCenterY, ellipseRadiusX + brushSize, ellipseRadiusY + brushSize) &&
           !PointInsideEllipse(x, y, ellipseCenterX, ellipseCenterY, ellipseRadiusX - brushSize, ellipseRadiusY - brushSize))
        {
            img.SetPixelColor(glm::uvec2(x, y), brushColor);
        }
    }
}

}
