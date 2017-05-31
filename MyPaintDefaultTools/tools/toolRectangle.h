#ifndef TOOL_RECTANGLE_H
#define TOOL_RECTANGLE_H
#include <tools/tool.h>

namespace mp
{

class CRectangleTool : public CTool
{
public:
    explicit CRectangleTool(CMyPaint* ow);
    ~CRectangleTool() = default;

    void Begin(const glm::ivec2& mousePos) override;
    void End(const glm::ivec2& mousePos) override;
    void Update(const glm::ivec2& mousePos) override;

private:
    void GetDrawBBox(const glm::ivec2& newPos, int& xStart, int& xEnd,
                                               int& yStart, int& yEnd) const;
    void DrawRectTo(const glm::ivec2& mousePos);

    glm::ivec2 startPosition;
    glm::ivec2 prevMousePosition;
};

}

#endif
