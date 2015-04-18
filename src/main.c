#include "mainwindow.h"
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[] ) {

// initial dataset

     ModelData modelData;
     modelData.pTrqTable = 0;
     modelData.pRailTable = 0;

     modelData.loadSetpoint = 0;
     modelData.initTorqueLoad = 0;
     modelData.speedSetpoint = 100;
     modelData.initSpeedEng = 64;
     modelData.inputSetpoint = 0;

     modelData.timeLoadRise = 0;
     modelData.timeLoadFall = 0;
     modelData.timeSpeedRise = 1;
     modelData.timeSpeedFall = 8;
     modelData.timeInputRise = 0;
     modelData.timeInputFall = 0;

     modelData.plotWidth = 600;
     modelData.plotHeight = 400;
     modelData.arrayLength = 100;
     modelData.timeMin = 0.0;
     modelData.yMin = -2.0;
     modelData.yMax = 2.0;

     modelData.timeMax = 8;
     modelData.timeStep = 0.001;

     modelData.inertFac = 4.2;
     modelData.inputFac = 800;
     modelData.speedFac = 3.4;
     modelData.inputLag = 5;

     modelData.ctrl.dState = 0;
     modelData.ctrl.iState = 0;
     modelData.ctrl.pastPos = 0;

     modelData.ctrl.iPos = 10000;
     modelData.ctrl.iNeg = -10000;

     modelData.ctrl.iGain = 0.0014;
     modelData.ctrl.pGain = 0.69;
     modelData.ctrl.dGain = 0;

     modelData.ctrl.clockTime = 0.005;

     modelData.ctrl.pWeight = 1;
     modelData.ctrl.bDiff = 0;
     modelData.ctrl.aDiff = 0;

     modelData.consoleMode = FALSE;
     modelData.plotCreated = FALSE;

     if ( argc != 7 ) {

          if ( argc == 1 ) {

               g_printf ( "No parameters listed \n" );
               create_main_window (&modelData, &argc, &argv);

          } else {

               g_printf ( "Start example:\n./gSimPIDBench -p 0.001 -i 0.001 -o outfile.txt\n-p - proportional PID factor\n-i - integral PID factor\n-o - path to results file *.txt" );

          }

     } else {

          modelData.consoleMode = TRUE;

          int i = 0;

          for ( i = 1 ; i < argc ; i += 2) {

               if  ( !( strcmp( "-p", argv[i] ) ) ) {

                    modelData.ctrl.pGain = atof (argv[i + 1]);
                    g_printf( "pGain = %f\n", modelData.ctrl.pGain );

               } else {

                    if ( !( strcmp( "-i", argv[i] ) ) ) {

                         modelData.ctrl.iGain = atof (argv[i + 1]);
                         g_printf( "iGain = %f\n", modelData.ctrl.iGain );

                    } else {

                         if ( !( strcmp( "-o", argv[i] ) ) ) {

                              strcpy( modelData.outfile, argv[i + 1]);

                              g_printf( "Results saved in %s\n", modelData.outfile);

                         }
                    }
               }

          }

          create_main_window (&modelData, &argc, &argv);

     }

	return 0;
}
