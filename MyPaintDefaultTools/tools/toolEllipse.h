#ifndef TOOL_ELLIPSE_H
#define TOOL_ELLIPSE_H
#include <tools/tool.h>

namespace mp
{

class CEllipseTool : public CTool
{
public:
    explicit CEllipseTool(CMyPaint* ow);
    ~CEllipseTool() = default;

    void Begin(const glm::ivec2& mousePos) override;
    void End(const glm::ivec2& mousePos) override;
    void Update(const glm::ivec2& mousePos) override;

private:
    static bool PointInsideEllipse(float x, float y, float eX, float eY, float rX, float rY);
    void GetDrawBBox(const glm::ivec2& newPos, int& xStart, int& xEnd,
                                               int& yStart, int& yEnd) const;
    void DrawEllipseTo(const glm::ivec2& mousePos);

    glm::ivec2 startPosition;
    glm::ivec2 prevMousePosition;
};

}

#endif
