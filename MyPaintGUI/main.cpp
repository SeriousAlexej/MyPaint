#include <stdexcept>
#include <iostream>
#include <mypaint.h>
#include "mypaintgui.h"

int main(int argc, char** argv)
{
    {
        try
        {
            mp::CMyPaint myPaint(argv[0]);
            mp::CMyPaintGUI myPaintGUI(&myPaint);

            myPaintGUI.Run();
        } catch(std::exception e)
        {
            std::cerr << e.what();
            return 1;
        }
    }
    return 0;
}
