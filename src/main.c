#include "mainwindow.h"

int main( int argc, char *argv[]) {

// initial dataset

ModelData modelData;

     modelData.loadSetpoint = 290;
     modelData.initTorqueLoad = 290;
     modelData.speedSetpoint = 82;
     modelData.initSpeedEng = 64;

     modelData.timeLoadRise = 0;
     modelData.timeLoadFall = 1;
     modelData.timeSpeedRise = 0.05;
     modelData.timeSpeedFall = 1;


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
