#ifndef DATASET_H
#define DATASET_H

#include <gtk/gtk.h>
#include <stddef.h>

typedef struct {

     double dState;  // last differential component value
     double iState;  // integrator state
     double pastPos; // last measured plant output, used only in update_controller_output_astrom


     double pGain,  // proportional gain, also used in update_controller_output_astrom
            iGain,  // integral gain, not used in update_controller_output_astrom
            dGain,  // derivative gain not used in update_controller_output_astrom
            iPos, iNeg; // maximum and minimum allowable integrator state, not used in update_controller_output_basic

     // parameters for update_controller_output_astrom
     double pWeight,// setpoint weight for P component
            bDiff,  // differentiating factor
            aDiff;  // differentiating filter factor

     double clockTime; // used only in update_controller_output_basic

} PID;

typedef struct {

  int height;
  int width;

} Size;

typedef struct {

    double x;
    double y;
    double value;

} Node;

typedef struct {

    int maxNumOfSymbols;
    int maxNumOfWords;
    int numOfLines;
    double * dataTable;
    Node node1;
    Node node2;
    Node node3;
    Node node4;
    Node point;

} LookupTable;

typedef struct {

     // controller data
     PID ctrl;

     // model data
     double timeMin;
     double timeMax;
     double timeStep;
     double ctrlClockTime;

     double timeLoadRise;
     double timeLoadFall;
     double timeSpeedRise;
     double timeSpeedFall;
     double timeInputRise;
     double timeInputFall;

     double inertFac;
     double inputFac;
     double speedFac;
     int inputLag;

     double loadSetpoint;
     double initTorqueLoad;
     double speedSetpoint;
     double initSpeedEng;
     double inputSetpoint;
     double tmpControlAction;

     //plot data
     gboolean plotCreated;

     int plotWidth;
     int plotHeight;
     int arrayLength;

     double yMin;
     double yMax;

     size_t time;
     size_t inputEng;
     size_t torqueEng;
     size_t accelEng;
     size_t torqueLoad;
     size_t speedEngSetpoint;
     size_t speedEng;

     size_t plotSpeed;
     size_t plotLoad;
     size_t plotSet;
     size_t plotInput;

     size_t pTrqTable;
     size_t pRailTable;

     Size oldSize;
     Size newSize;

     gboolean consoleMode;
     char outfile[256];

} ModelData;

#endif // DATASET_H