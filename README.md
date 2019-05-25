gSimPIDBench
============

Application implements the simple model of PID governor with plant process 

##How to build

MahtGL version 2.3.2 was required, with 2.4.3 does not commpile due to changes in API for C interfacing

Building MathGL
~~~
cmake .
cmake .
make
sudo make install
#If there are problems with loading shared library
LD_LIBRARY_PATH=/usr/local/lib/
export LD_LIBRARY_PATH
~~~
Removing MathGL
~~~
cat install_manifest.txt | xargs echo sudo rm | sh
~~~
Building using existing cmake configuration
~~~
cmake .
cmake .
make
~~~
Running application
~~~
./gSimPIDBench 
~~~
Building sources of tool one by one for debugging
~~~
functions.c

cc -c functions.c -o functions.o -I/usr/include/gtk-3.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0 -I/usr/include/mgl2

"/usr/include/gtk-3.0"
"/usr/include/at-spi2-atk/2.0"
"/usr/include/gtk-3.0"
"/usr/include/gio-unix-2.0/"
"/usr/include/cairo"
"/usr/include/pango-1.0"
"/usr/include/atk-1.0"
"/usr/include/cairo"
"/usr/include/pixman-1"
"/usr/include/freetype2"
"/usr/include/libpng16"
"/usr/include/harfbuzz"
"/usr/include/freetype2"
"/usr/include/harfbuzz"
"/usr/include/libdrm"
"/usr/include/libpng16"
"/usr/include/gdk-pixbuf-2.0"
"/usr/include/libpng16"
"/usr/include/glib-2.0"
"/usr/lib/glib-2.0/include"
~~~