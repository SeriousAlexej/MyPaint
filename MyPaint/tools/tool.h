#ifndef MY_PAINT_TOOL_H
#define MY_PAINT_TOOL_H
#include <glm/vec2.hpp>
#include <string>
#include "../mypaint_api.h"

namespace mp
{

class CMyPaint;

/**
 * CTool class is an interface for
 * plugins that provide custom tools
 * for processing images
 */
class MYPAINT_API CTool
{
public:
    explicit CTool(CMyPaint* ow) : owner(ow) {}
    virtual ~CTool() = default;

    const std::string& GetName() const { return name; }

    virtual void SetActive() {}
    virtual void SetInactive() { started = false; }
    virtual void Begin(const glm::ivec2& mousePos) = 0;
    virtual void End(const glm::ivec2& mousePos) = 0;
    virtual void Update(const glm::ivec2& mousePos) = 0;

protected:
    bool        started = false;
    std::string name = "<unnamed_tool>";
    CMyPaint*   owner;
};

}

#endif
