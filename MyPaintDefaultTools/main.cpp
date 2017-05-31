#include "main.h"
#include "tools/toolEllipse.h"
#include "tools/toolLine.h"
#include "tools/toolRectangle.h"
#include "image_formats/tgaLoader.h"

mp::CTool*          CToolEllipseNew(mp::CMyPaint* ow)   { return new mp::CEllipseTool(ow); }
mp::CTool*          CToolLineNew(mp::CMyPaint* ow)      { return new mp::CLineTool(ow); }
mp::CTool*          CToolRectangleNew(mp::CMyPaint* ow) { return new mp::CRectangleTool(ow); }
mp::CImageLoader*   CTGALoaderNew()                     { return new mp::CTGALoader(); }

toolFactoryPtr plugin_ToolFactories[] =
{
    &CToolLineNew,
    &CToolRectangleNew,
    &CToolEllipseNew
};

loaderFactoryPtr plugin_LoaderFactories[] =
{
    &CTGALoaderNew
};

int toolFactoryNumElements   = ARRAYCOUNT(plugin_ToolFactories);
int loaderFactoryNumElements = ARRAYCOUNT(plugin_LoaderFactories);

