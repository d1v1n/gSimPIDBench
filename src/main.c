//gcc *.c -o gsimpidbench -std=c99 -Wall -pedantic `pkg-config --cflags --libs gtk+-3.0` -lm -lmgl
//gcc *.c -o gsimpidbench -std=c99 -Wall -pedantic  -mms-bitfields -IC:/dev/mathgl/include/ -IC:/dev/GTK3/include/gtk-3.0 -IC:/dev/GTK3/include/cairo -IC:/dev/GTK3/include/pango-1.0 -IC:/dev/GTK3/include/atk-1.0 -IC:/dev/GTK3/include/cairo -IC:/dev/GTK3/include/pixman-1 -IC:/dev/GTK3/include -IC:/dev/GTK3/include/freetype2 -IC:/dev/GTK3/include -IC:/dev/GTK3/include/libpng15 -IC:/dev/GTK3/include/gdk-pixbuf-2.0 -IC:/dev/GTK3/include/libpng15 -IC:/dev/GTK3/include/glib-2.0 -IC:/dev/GTK3/lib/glib-2.0/include -LC:/dev/GTK3/lib -lgtk-3 -lgdk-3 -lgdi32 -limm32 -lshell32 -lole32 -Wl,-luuid -lpangocairo-1.0 -lpangoft2-1.0 -lfreetype -lfontconfig -lpangowin32-1.0 -lgdi32 -lpango-1.0 -lm -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl -lm -lmgl -std=c99

#include "mainwindow.h"

int main( int argc, char *argv[]) {

// initial dataset

ModelData modelData;

     modelData.plotWidth = 600;
     modelData.plotHeight = 400;
     modelData.arrayLength = 100;
     modelData.timeMin = 0.0;
     modelData.yMin = -2.0;
     modelData.yMax = 2.0;

     modelData.timeMax = 1;
     modelData.timeStep = 0.001;

     modelData.inertFac = 4.2;
     modelData.inputFac = 800;
     modelData.speedFac = 3.4;

     modelData.initTorqueLoad = 290;
     modelData.speedSetpoint  = 63.8;
     modelData.initSpeedEng   = 63.8;

     modelData.ctrl.dState = 0;
     modelData.ctrl.iState = 0;
     modelData.ctrl.pastPos = 0;

     modelData.ctrl.iPos = 10000;
     modelData.ctrl.iNeg = -10000;

     modelData.ctrl.iGain = 0.0014;
     modelData.ctrl.pGain = 0.69;
     modelData.ctrl.dGain = 1.0474;

     modelData.ctrl.clockTime = 0.005;

     modelData.ctrl.pWeight = 1;
     modelData.ctrl.bDiff = 0;
     modelData.ctrl.aDiff = 0;

     create_main_window (&modelData, &argc, &argv);

	return 0;
}
