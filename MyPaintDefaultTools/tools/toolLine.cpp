#include "toolLine.h"
#include <event_manager/eventmanager.h>
#include <mypaint.h>
#include <image/image.h>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat2x2.hpp>

namespace mp
{

CLineTool::CLineTool(CMyPaint* ow) :
    CTool(ow)
{
    name = "Line";
}

void CLineTool::Begin(const glm::ivec2 &mousePos)
{
    started = true;
    startPosition = mousePos;
    prevMousePosition = mousePos;
}

bool CLineTool::SConvexRectangle::PointInside(const glm::vec2& point)
{
    for(int i=0; i<4; ++i)
    {
        glm::vec2 vB = point - vertex[i];
        glm::vec2 vA = vertex[(i+1)%4] - vertex[i];
        if(vA[0]*vB[1] - vB[0]*vA[1] < 0.0f)
            return false;
    }
    return true;
}

void CLineTool::End(const glm::ivec2 &mousePos)
{
    started = false;
}

void CLineTool::GetDrawBBox(const glm::ivec2 &newPos, int &xStart, int &xEnd, int &yStart, int &yEnd) const
{
    unsigned brushSize = owner->GetBrushSize();
    xStart = glm::min(startPosition.x, newPos.x) - brushSize;
    xEnd   = glm::max(startPosition.x, newPos.x) + brushSize;
    yStart = glm::min(startPosition.y, newPos.y) - brushSize;
    yEnd   = glm::max(startPosition.y, newPos.y) + brushSize;
}

void CLineTool::Update(const glm::ivec2 &mousePos)
{
    if(!started)
    {
        return;
    }

    int xStart, xEnd, yStart, yEnd;
    GetDrawBBox(prevMousePosition, xStart, xEnd, yStart, yEnd);

    CImage& img = *(owner->GetCurrentImage());
    CImage* prevImg = owner->GetPreviousImage();

    if(!prevImg)
    {
        return;
    }

    img.CopyRectFrom(*prevImg, glm::ivec2(xStart, yStart), glm::ivec2(xEnd, yEnd));

    DrawLineTo(mousePos);

    CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));

    prevMousePosition = mousePos;
}

void CLineTool::DrawLineTo(const glm::ivec2 &mousePos)
{
    if(mousePos == startPosition)
    {
        return;
    }

    CImage& img = *(owner->GetCurrentImage());

    float brushSize = owner->GetBrushSize() * 0.5f;
    glm::uvec3 brushColor = owner->GetBrushColor();

    int xStart, xEnd, yStart, yEnd;
    GetDrawBBox(mousePos, xStart, xEnd, yStart, yEnd);

    static const glm::mat2 rotation90degCW = glm::mat2(glm::vec2(0.0f, -1.0f),
                                                       glm::vec2(1.0f,  0.0f));

    glm::vec2 toSide = glm::normalize(glm::vec2(mousePos - startPosition)) * brushSize;
    toSide = rotation90degCW * toSide;

    SConvexRectangle rect;
    rect.vertex[0] = glm::vec2(startPosition) + toSide;
    rect.vertex[1] = glm::vec2(mousePos) + toSide;
    rect.vertex[2] = glm::vec2(mousePos) - toSide;
    rect.vertex[3] = glm::vec2(startPosition) - toSide;

    for(int x = xStart; x <= xEnd; x++)
    for(int y = yStart; y <= yEnd; y++)
    {
        if(rect.PointInside(glm::vec2(x, y)))
        {
            img.SetPixelColor(glm::uvec2(x, y), brushColor);
        }
    }
}

}
