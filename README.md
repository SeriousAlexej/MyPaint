MyPaint
=====

![screenshot of MyPaint](https://raw.githubusercontent.com/SeriousAlexej/MyPaint/master/output/demo.png)

Test task of Serov Alexey

MyPaint is a simple drawing program.
It consists of four components:

  - MyPaint                    - library containing main logic, has public API
  - MyPaintGUI                 - an implementation of GUI, links to MyPaint library
  - MyPaintDefaultTools        - library, containing default tools which are loaded as plugins in runtime
  - MyPaintPluginPen           - library with additional 'Pen' tool, loaded as plugin in runtime


Building
-------------

Project uses CMake. It has no options and provides an install target, which copies
binaries to the 'output' folder.

Example of building and launching MyPaint on linux:
`$ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release && make install && ./output/MyPaint`

Tested on Windows and Linux with following generators:

  - MinGW Makefiles
  - Unix Makefiles
  - Visual Studio 14 2015

	
Notes
-------------

As tools and support for image formats are implemented in plugins,
if 'plugins' directory is empty, MyPaint shall launch normally but
will have no tools in toolbar.
