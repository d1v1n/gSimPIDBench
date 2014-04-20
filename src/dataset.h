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

     double inertFac;
     double inputFac;
     double speedFac;

     double loadSetpoint;
     double initTorqueLoad;
     double speedSetpoint;
     double initSpeedEng;

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
     size_t plot;

     GdkPixbuf *pixbufForPlot;
     GtkWidget *plotImage;

} ModelData;

#endif // DATASET_H