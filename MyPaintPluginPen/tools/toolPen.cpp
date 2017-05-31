#include "toolPen.h"
#include <event_manager/eventmanager.h>
#include <mypaint.h>
#include <image/image.h>
#include <glm/vec3.hpp>
#include <glm/common.hpp>

namespace mp
{

CPenTool::CPenTool(CMyPaint* ow) :
    CTool(ow)
{
    name = "Pen";
}

void CPenTool::Begin(const glm::ivec2 &mousePos)
{
    started = true;
    prevPosition = mousePos;
}

void CPenTool::End(const glm::ivec2 &mousePos)
{
    started = false;
}

void CPenTool::Update(const glm::ivec2 &mousePos)
{
    if(!started)
    {
        return;
    }

    DrawBresenhamLine(prevPosition.x, prevPosition.y, mousePos.x, mousePos.y);

    prevPosition = mousePos;

    CEventManager::GetInstance().SendEvent(CEvent("ImageUpdated"));
}

void CPenTool::DrawBresenhamLine(int x1, int y1, int x2, int y2)
{
    const glm::uvec3 brushColor = owner->GetBrushColor();
    CImage& img = *(owner->GetCurrentImage());


    int delta_x(x2 - x1);
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    img.SetPixelColor(glm::uvec2(x1, y1), brushColor);

    if (delta_x >= delta_y)
    {
        int error(delta_y - (delta_x >> 1));

        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }

            error += delta_y;
            x1 += ix;

            img.SetPixelColor(glm::uvec2(x1, y1), brushColor);
        }
    }
    else
    {
        int error(delta_x - (delta_y >> 1));

        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }

            error += delta_x;
            y1 += iy;

            img.SetPixelColor(glm::uvec2(x1, y1), brushColor);
        }
    }
}

}
