#include "main.h"
#include "tools/toolPen.h"

mp::CTool* CToolPenNew(mp::CMyPaint* ow) { return new mp::CPenTool(ow); }

toolFactoryPtr plugin_ToolFactories[] =
{
    &CToolPenNew,
};

loaderFactoryPtr plugin_LoaderFactories[] =
{
	nullptr
};

int toolFactoryNumElements   = ARRAYCOUNT(plugin_ToolFactories);
int loaderFactoryNumElements = 0;

