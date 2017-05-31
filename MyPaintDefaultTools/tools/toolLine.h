#ifndef TOOL_LINE_H
#define TOOL_LINE_H
#include <tools/tool.h>
#include <glm/vec2.hpp>

namespace mp
{

class CLineTool : public CTool
{
public:
    explicit CLineTool(CMyPaint* ow);
    ~CLineTool() = default;

    void Begin(const glm::ivec2& mousePos) override;
    void End(const glm::ivec2& mousePos) override;
    void Update(const glm::ivec2& mousePos) override;

private:
    struct SConvexRectangle
    {
        glm::vec2 vertex[4];
        bool PointInside(const glm::vec2& point);
    };

    void GetDrawBBox(const glm::ivec2& newPos, int& xStart, int& xEnd,
                                               int& yStart, int& yEnd) const;
    void DrawLineTo(const glm::ivec2& mousePos);

    glm::ivec2 startPosition;
    glm::ivec2 prevMousePosition;
};

}

#endif
