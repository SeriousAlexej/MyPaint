#ifndef TOOL_PEN_H
#define TOOL_PEN_H
#include <tools/tool.h>
#include <glm/vec2.hpp>

namespace mp
{

class CPenTool : public CTool
{
public:
    explicit CPenTool(CMyPaint* ow);
    ~CPenTool() = default;

    void Begin(const glm::ivec2& mousePos) override;
    void End(const glm::ivec2& mousePos) override;
    void Update(const glm::ivec2& mousePos) override;

private:
    void DrawBresenhamLine(int x1, int y1, int x2, int y2);

    glm::ivec2 prevPosition;
};

}

#endif
