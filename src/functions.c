#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <mgl2/mgl_cf.h>
#include <math.h>
#include <stddef.h>
#include <time.h>

#include "dataset.h"
#include "functions.h"
#include "mainwindow.h"

static int typeOfPID = 0;
static int useTable = 1; // lets use default tables in "res/" folder
static LookupTable* global_pLookupTable = NULL;

int initiate_data_for_plots ( ModelData* modelData ){

     modelData->time = ( size_t ) mgl_create_data_size ( 1, 1, 1 );
     modelData->inputEng = ( size_t ) mgl_create_data_size ( 1, 1, 1 );
     modelData->torqueEng = ( size_t ) mgl_create_data_size ( 1, 1, 1 );
     modelData->torqueLoad = ( size_t ) mgl_create_data_size ( 1, 1, 1 );
     modelData->accelEng = ( size_t ) mgl_create_data_size ( 1, 1, 1 );
     modelData->speedEng = ( size_t ) mgl_create_data_size ( 1, 1, 1 );
     modelData->speedEngSetpoint = ( size_t ) mgl_create_data_size ( 1, 1, 1 );

     //loading example tables
     modelData->pTrqTable = (size_t) (prepare_lookup_table ("res/example_trqTable.txt"));
     modelData->pRailTable = (size_t) (prepare_lookup_table ("res/example_railTable.txt"));

     return 0;
}

int initiate_plot ( ModelData* modelData ) {

     mgl_def_font ( "cursor", "res/" ); // loading default font before creating of plot

     modelData->plotSpeed = ( size_t ) mgl_create_graph ( modelData->plotWidth, modelData->plotHeight ); // create graph Speed

     modelData->plotLoad = ( size_t ) mgl_create_graph ( modelData->plotWidth, modelData->plotHeight ); // create graph Load

     modelData->plotSet = ( size_t ) mgl_create_graph ( modelData->plotWidth, modelData->plotHeight ); // create graph Set

     modelData->plotInput = ( size_t ) mgl_create_graph ( modelData->plotWidth, modelData->plotHeight ); // create graph Man

     setup_plot_in_pixbuf(modelData);

     plotImageSpeed = gtk_image_new_from_pixbuf ( pixbufForSpeed ); // making image widget from pixbuf
     plotImageLoad = gtk_image_new_from_pixbuf ( pixbufForLoad );
     plotImageSet = gtk_image_new_from_pixbuf ( pixbufForSet );
     plotImageInput = gtk_image_new_from_pixbuf ( pixbufForInput );

     update_plot_callback ( NULL, modelData );

     modelData->plotCreated = TRUE;

     //printf("Plot initiated\n");

     return 0;
}

int set_default_plot_decor ( size_t plot, char * axisName ) {

     mgl_set_font_size ( ( HMGL ) plot, 3 ); // setting size of font
     mgl_axis ( ( HMGL ) plot, "xyz", "", "" );
     mgl_axis_grid ( ( HMGL ) plot,"xy" ,"" ,"" );
     mgl_label ( ( HMGL ) plot, 'x', "t, s", 0, "" );
     mgl_label ( ( HMGL ) plot, 'y', axisName, 0, "" );

     return 0;
}

int setup_plot_in_pixbuf ( ModelData *modelData ) {

     mgl_set_ranges ( ( HMGL ) modelData->plotSpeed, modelData->timeMin, modelData->timeMax, modelData->yMin, modelData->yMax, -1.0, 1.0 ); // values for x, y, z axis
     set_default_plot_decor ( modelData->plotSpeed, "speed, s-1" );
     mgl_plot_xy ( ( HMGL ) modelData->plotSpeed, ( HMDT ) modelData->time, ( HMDT ) modelData->speedEng, "", "" ); // plot data
     const unsigned char* imageInMemory = mgl_get_rgb ( ( HMGL ) modelData->plotSpeed ); // get pointer on plot in memory
     pixbufForSpeed = gdk_pixbuf_new_from_data ( imageInMemory, GDK_COLORSPACE_RGB, FALSE, 8, modelData->plotWidth, modelData->plotHeight, modelData->plotWidth * 3, NULL, NULL ); // making pixbuf from memory data...

     mgl_set_ranges ( ( HMGL ) modelData->plotLoad, modelData->timeMin, modelData->timeMax, ( modelData->initTorqueLoad * 0.9 ), ( modelData->loadSetpoint * 1.1 ) , -1.0, 1.0 );
     set_default_plot_decor ( modelData->plotLoad, "load, Nm" );
     mgl_plot_xy ( ( HMGL ) modelData->plotLoad, ( HMDT ) modelData->time, ( HMDT ) modelData->torqueLoad, "", "" );
     imageInMemory = mgl_get_rgb ( ( HMGL ) modelData->plotLoad );
     pixbufForLoad = gdk_pixbuf_new_from_data ( imageInMemory, GDK_COLORSPACE_RGB, FALSE, 8, modelData->plotWidth, modelData->plotHeight, modelData->plotWidth * 3, NULL, NULL );

     mgl_set_ranges ( ( HMGL ) modelData->plotSet, modelData->timeMin, modelData->timeMax, modelData->yMin, modelData->yMax, -1.0, 1.0 );
     set_default_plot_decor ( modelData->plotSet, "set, s-1" );
     mgl_plot_xy ( ( HMGL ) modelData->plotSet, ( HMDT ) modelData->time, ( HMDT ) modelData->speedEngSetpoint, "", "" );
     imageInMemory = mgl_get_rgb ( ( HMGL ) modelData->plotSet );
     pixbufForSet = gdk_pixbuf_new_from_data ( imageInMemory, GDK_COLORSPACE_RGB, FALSE, 8, modelData->plotWidth, modelData->plotHeight, modelData->plotWidth * 3, NULL, NULL );

     mgl_set_ranges ( ( HMGL ) modelData->plotInput, modelData->timeMin, modelData->timeMax, 0, 1.1, -1.0, 1.0 );
     set_default_plot_decor ( modelData->plotInput, "input, -" );
     mgl_plot_xy ( ( HMGL ) modelData->plotInput, ( HMDT ) modelData->time, ( HMDT ) modelData->inputEng, "", "" );
     imageInMemory = mgl_get_rgb ( ( HMGL ) modelData->plotInput );
     pixbufForInput = gdk_pixbuf_new_from_data ( imageInMemory, GDK_COLORSPACE_RGB, FALSE, 8, modelData->plotWidth, modelData->plotHeight, modelData->plotWidth * 3, NULL, NULL );

     return 0;
}

int validate_data ( ModelData *modelData ) {

     if (!((modelData->timeMax > lim_timeMax_B) && (modelData->timeMax <= lim_timeMax_T))) {

          printf("Input data validation failed: timeMax\n");
          return 0;
     }

     if (!((modelData->timeStep > lim_timeStep_B) && (modelData->timeStep <= lim_timeStep_T))) {

          printf("Input data validation failed: timeStep\n");
          return 0;
     }

     if (!((modelData->inertFac > lim_inertFac_B) && (modelData->inertFac <= lim_inertFac_T))) {

          printf("Input data validation failed: inertFac\n");
          return 0;
     }

     if (!((modelData->inputFac > lim_inputFac_B) && (modelData->inputFac <= lim_inputFac_T))) {

          printf("Input data validation failed: inputFac\n");
          return 0;
     }

     if (!((modelData->speedFac > lim_speedFac_B) && (modelData->speedFac <= lim_speedFac_T))) {

          printf("Input data validation failed: speedFac\n");
          return 0;
     }

     if (!((modelData->initTorqueLoad >= lim_initTorque_B) && (modelData->initTorqueLoad <= lim_initTorque_T))) {

          printf("Input data validation failed: initTorque\n");
          return 0;
     }

     if (!((modelData->speedSetpoint  >= lim_speedSetpoint_B) && (modelData->speedSetpoint  <= lim_speedSetpoint_T))) {

          printf("Input data validation failed: speedSetpoint\n");
          return 0;
     }

     if (!((modelData->initSpeedEng  >= lim_initSpeedEng_B) && (modelData->initSpeedEng < lim_initSpeedEng_T))) {

          printf("Input data validation failed: initSpeedEng\n");
          return 0;
     }

     if (!((modelData->ctrl.pGain >= lim_pGain_B) && (modelData->ctrl.pGain <= lim_pGain_T))) {

          printf("Input data validation failed: pGain\n");
          return 0;
     }

     if (!((modelData->ctrl.iGain >= lim_iGain_B) && (modelData->ctrl.iGain <= lim_iGain_T))) {

          printf("Input data validation failed: iGain\n");
          return 0;
     }

     if (!((modelData->ctrl.dGain >= lim_dGain_B) && (modelData->ctrl.dGain <= lim_dGain_T))) {

          printf("Input data validation failed: dGain\n");
          return 0;
     }

     if (!((modelData->ctrl.iPos >= lim_iPos_B) && (modelData->ctrl.iPos <= lim_iPos_T))) {

          printf("Input data validation failed: iPos\n");
          return 0;
     }

     if (!((modelData->ctrl.iNeg >= -lim_iNeg_T) && (modelData->ctrl.iNeg <= lim_iNeg_B))) {

          printf("Input data validation failed: iNeg\n");
          return 0;
     }

     if (!((modelData->ctrl.clockTime > lim_ctrlClockTime_B) && (modelData->ctrl.clockTime <= lim_ctrlClockTime_T))) {

          printf("Input data validation failed: ctrlClockTime\n");
          return 0;
     }

          if (!((modelData->ctrl.pWeight >= lim_pWeight_B) && (modelData->ctrl.pWeight <= lim_pWeight_T))) {

          printf("Input data validation failed: pWeight\n");
          return 0;
     }

          if (!((modelData->ctrl.bDiff >= lim_bDiff_B) && (modelData->ctrl.bDiff <= lim_bDiff_T))) {

          printf("Input data validation failed: bDiff\n");
          return 0;
     }

        if (!((modelData->ctrl.aDiff >= lim_aDiff_B) && (modelData->ctrl.aDiff <= lim_aDiff_T))) {

          printf("Input data validation failed: aDiff\n");
          return 0;
     }
     //printf("Input data validation successfull\n");
     return 1;
}

double update_controller_output_simple ( PID* ctrl, double setponit, double position ) {

     double pTerm = 0;
     double dTerm = 0;
     double iTerm = 0;
     double error = setponit - position;

     pTerm = ctrl->pGain * error; // calculate the proportional term

     ctrl->iState += error; // calculate the integral state with appropriate limiting

//     if (ctrl->iState > ctrl->iPos) ctrl->iState = ctrl->iPos;
//
//     if (ctrl->iState < ctrl->iNeg) ctrl->iState = ctrl->iNeg;

     //printf("iState = %f\n", ctrl->iState);

     iTerm = ctrl->iGain * ctrl->iState; // calculate the integral term

     dTerm = ctrl->dGain * (error - ctrl->dState); // calculate the derivative term

     ctrl->dState = error;

     return (pTerm + iTerm + dTerm); // return controller output
}

double update_controller_output_astrom ( PID* ctrl, double setpoint, double position ) {

     double pTerm = 0;
     double dTerm = 0;
     double iTerm = 0;
     double error = setpoint - position;

     pTerm = ctrl->pGain * (ctrl->pWeight * setpoint - position); // calculate the proportional term

     ctrl->iState += error; // calculate the integral state with appropriate limiting

     if (ctrl->iState > ctrl->iPos) ctrl->iState = ctrl->iPos;

     if (ctrl->iState < ctrl->iNeg) ctrl->iState = ctrl->iNeg;

     //printf("iState = %f\n", ctrl->iState);

     iTerm = ctrl->iGain * ctrl->iState; // calculate the integral term

     ctrl->dState = ctrl->aDiff * ctrl->dState - ctrl->bDiff * (position - ctrl->pastPos); // calculate the derivative term

     dTerm = ctrl->dState;
     ctrl->pastPos = position;

     return (pTerm + iTerm + dTerm); // return controller output
}

int calculate_input_signal ( ModelData* modelData, int n ) {

     int i = 0;
     int loadRisePoint = (int) (modelData->timeLoadRise / modelData->timeStep);
     int loadFallPoint = (int) (modelData->timeLoadFall / modelData->timeStep);
     int speedRisePoint = (int) (modelData->timeSpeedRise / modelData->timeStep);
     int speedFallPoint = (int) (modelData->timeSpeedFall / modelData->timeStep);

     int inputRisePoint = (int) (modelData->timeInputRise / modelData->timeStep);
     int inputFallPoint = (int) (modelData->timeInputFall / modelData->timeStep);
     //modelData->tmpControlAction = ((modelData->initTorqueLoad - modelData->speedFac * modelData->initSpeedEng) / (/*0.01 **/ modelData->inputFac)); // stationary conditions
     modelData->tmpControlAction = 0;
     //double speedRisePoint = modelData->timeSpeedRise;
     //double speedFallPoint = modelData->timeSpeedFall;

     //g_print ("Speed setpoints %f\t%f\n", modelData->timeSpeedRise, modelData->timeSpeedFall);

     modelData->speedEngSetpoint = (size_t)mgl_create_data_size(n, 1, 1);
     modelData->torqueLoad = (size_t)mgl_create_data_size(n, 1, 1);

     mgl_data_set_value((HMDT)modelData->speedEngSetpoint, modelData->initSpeedEng, 0, 0, 0);

     for (i = 0; i < speedRisePoint; i++) {

          mgl_data_set_value((HMDT)modelData->speedEngSetpoint, modelData->initSpeedEng, i, 0, 0);
          //g_print ("%d\t%f\n", i, modelData->initSpeedEng);

     }

     for (i = speedRisePoint; i < speedFallPoint; i++) {

          mgl_data_set_value((HMDT)modelData->speedEngSetpoint, modelData->speedSetpoint, i, 0, 0);
          //g_print ("%d\t%f\n", i, modelData->speedSetpoint);

     }

     for (i = speedFallPoint; i < n; i++) {

          mgl_data_set_value((HMDT)modelData->speedEngSetpoint, modelData->initSpeedEng, i, 0, 0);
          //g_print ("%d\t%f\n", i, modelData->initSpeedEng);

     }

     //g_print ("Load setpoints %f\t%f\n", modelData->timeLoadRise, modelData->timeLoadFall);

     for (i = 0; i < loadRisePoint; i++) {

          mgl_data_set_value((HMDT)modelData->torqueLoad, modelData->initTorqueLoad, i, 0, 0);
	  //g_print ("%d\t%f\n", i, modelData->initTorqueLoad);

     }

     for (i = loadRisePoint; i < loadFallPoint; i++) {

          mgl_data_set_value((HMDT)modelData->torqueLoad, (modelData->loadSetpoint), i, 0, 0);
	  //g_print ("%d\t%f\n", i, modelData->loadSetpoint);

     }

     for (i = loadFallPoint; i < n; i++) {

          mgl_data_set_value((HMDT)modelData->torqueLoad, (modelData->initTorqueLoad), i, 0, 0);
	  //g_print ("%d\t%f\n", i, modelData->initTorqueLoad);
     }

     for (i = 0; i < inputRisePoint; i++) {

          mgl_data_set_value((HMDT)modelData->inputEng, modelData->tmpControlAction, i, 0, 0);
          //g_print ("%d\t%f\n", i, modelData->tmpControlAction);

     }

     for (i = inputRisePoint; i < inputFallPoint; i++) {

          mgl_data_set_value((HMDT)modelData->inputEng, (modelData->inputSetpoint), i, 0, 0);
          //g_print ("%d\t%f\n", i, modelData->inputSetpoint);

     }

     for (i = inputFallPoint; i < n; i++) {

          mgl_data_set_value((HMDT)modelData->inputEng, (modelData->tmpControlAction), i, 0, 0);
          //g_print ("%d\t%f\n", i, modelData->tmpControlAction);
     }

     return 0;
}


int calculate_data ( ModelData* modelData ) {

     if (validate_data(modelData)) {

          int i = 0;
          int n = ( int ) modelData->timeMax / modelData->timeStep;
          int lagIter = 0;

          //g_print("calculate_data call\n");
          //g_print("number of iterations = %i\n", n);
          double lastCtrlTime = 0;
          double tmpTime = 0;
          double tmpEngInput = 0;
          double tmpEngSpeed = 0;
          double tmpInjection = 0;
          double tmpTrqEng = 0;
          double tmpAccelEng = 0;
          double tmpSpeedEng = 0;

          // reset controller states!

          modelData->ctrl.dState = 0;
          modelData->ctrl.iState = 0;
          modelData->ctrl.pastPos = 0;

          modelData->time = ( size_t ) mgl_create_data_size ( n, 1, 1 );
          modelData->inputEng = ( size_t ) mgl_create_data_size ( n, 1, 1 );
          modelData->speedEng = ( size_t ) mgl_create_data_size ( n, 1, 1 );

          modelData->speedEngSetpoint = ( size_t ) mgl_create_data_size ( n, 1, 1 );
          modelData->torqueLoad = ( size_t ) mgl_create_data_size ( n, 1, 1 );

          // in testing purpose
          calculate_input_signal ( modelData, n );
          // in testing purpose

          modelData->torqueEng = ( size_t ) mgl_create_data_size ( n, 1, 1 );
          modelData->accelEng = ( size_t ) mgl_create_data_size ( n, 1, 1 );

          modelData->yMin = modelData->initSpeedEng;
          modelData->yMax = modelData->initSpeedEng;

          mgl_data_set_value ( ( HMDT ) modelData->time, 0.0, 0, 0, 0 );
          mgl_data_set_value ( ( HMDT ) modelData->speedEng, modelData->initSpeedEng, 0, 0, 0 );

          // place inside calculate_input_signal function
          //mgl_data_set_value((HMDT)modelData->inputEng, tmpControlAction, 0, 0, 0);

          mgl_data_set_value ( ( HMDT ) modelData->torqueEng, modelData->initTorqueLoad, 0, 0, 0 );
          mgl_data_set_value ( ( HMDT ) modelData->accelEng, 0.0, 0, 0, 0 );

          //g_print("******************************\n");
          //g_print("Time\t TrqEng\t AccelEng\t SpeedEng\n");
          //g_print("******************************\n");
          //g_print("************************************************************\n");
          //g_print("Time\tSet\tCurr\tCtrl\n");
          //g_print("************************************************************\n");

          //g_print("%f\t %f\t %f\t %f\n", mgl_data_get_value((HMDT)modelData->time, 0, 0, 0), mgl_data_get_value((HMDT)modelData->torqueEng, 0, 0, 0), mgl_data_get_value((HMDT)modelData->accelEng, 0, 0, 0), mgl_data_get_value((HMDT)modelData->speedEng, 0, 0, 0));
          //g_print("%5.3f\t %5.3f\t %5.3f\t %5.3f\n", mgl_data_get_value((HMDT)modelData->time, 0, 0, 0), mgl_data_get_value((HMDT)modelData->speedEngSetpoint, 0, 0, 0), mgl_data_get_value((HMDT)modelData->speedEng, 0, 0, 0), modelData->tmpControlAction);

          for ( i = 1; i < n; i++ ) {

               tmpTime = i * modelData->timeStep;
               mgl_data_set_value( ( HMDT ) modelData->time, tmpTime, i, 0, 0 );


               if ( ( tmpTime - lastCtrlTime ) >= modelData->ctrl.clockTime ) {

                    lastCtrlTime = tmpTime;

                    if (typeOfPID == 0) modelData->tmpControlAction = update_controller_output_simple ( &modelData->ctrl, mgl_data_get_value((HMDT)modelData->speedEngSetpoint, (i - 1), 0, 0), mgl_data_get_value((HMDT)modelData->speedEng, (i - 1), 0, 0));
                    if (typeOfPID == 1) modelData->tmpControlAction = update_controller_output_astrom ( &modelData->ctrl, mgl_data_get_value((HMDT)modelData->speedEngSetpoint, (i - 1), 0, 0), mgl_data_get_value((HMDT)modelData->speedEng, (i - 1), 0, 0));

                    // actuator limitation
                    if (modelData->tmpControlAction > 1) modelData->tmpControlAction = 1;
                    if (modelData->tmpControlAction < 0) modelData->tmpControlAction = 0;

               }

               if ( ( ( tmpTime < modelData->timeInputRise ) || ( tmpTime > modelData->timeInputFall ) ) ||  ( ( modelData->timeInputRise == 0.0 ) && ( modelData->timeInputFall == 0.0 ) ) ) {

                    mgl_data_set_value ( ( HMDT ) modelData->inputEng, modelData->tmpControlAction, i, 0, 0 );

               }

               // transport delay calculation
               // length of delay determined by number of plant iterations in variable inputLag

               if ( ( ( i - 1 ) - modelData->inputLag ) < 0) {

                    lagIter = 0;

               } else {

                    lagIter = ((i - 1) - modelData->inputLag);

               }
               if (useTable == 0) {

                    tmpTrqEng = modelData->inputFac * mgl_data_get_value((HMDT)modelData->inputEng, (lagIter) , 0, 0) + modelData->speedFac * mgl_data_get_value((HMDT)modelData->speedEng, (i - 1), 0, 0);
                    mgl_data_set_value((HMDT)modelData->torqueEng, tmpTrqEng, i, 0, 0);

               } else if (useTable == 1) {

                    tmpEngInput = mgl_data_get_value((HMDT)modelData->inputEng, (lagIter) , 0, 0);
                    tmpEngSpeed = mgl_data_get_value((HMDT)modelData->speedEng, (i - 1), 0, 0);
                    //printf ("tmpEngSpeed %f tmpEngInput %f\n", tmpEngSpeed, tmpEngInput);
                    tmpInjection = get_data_from_table ((LookupTable *)modelData->pRailTable, tmpEngSpeed, tmpEngInput);
                    tmpTrqEng = get_data_from_table ((LookupTable *)modelData->pTrqTable, tmpEngSpeed, tmpInjection);
                    //printf ("tmpTrqEng = %f\n", tmpTrqEng);
                    mgl_data_set_value((HMDT)modelData->torqueEng, tmpTrqEng, i, 0, 0);

               }


               tmpAccelEng = (mgl_data_get_value((HMDT)modelData->torqueEng, (i - 1), 0, 0) - mgl_data_get_value((HMDT)modelData->torqueLoad, (i - 1), 0, 0)) / modelData->inertFac;
               mgl_data_set_value((HMDT)modelData->accelEng, tmpAccelEng, i, 0, 0);

               tmpSpeedEng = mgl_data_get_value((HMDT)modelData->speedEng, (i - 1), 0, 0) + tmpAccelEng * modelData->timeStep;

               if (modelData->yMin > tmpSpeedEng) modelData->yMin = tmpSpeedEng;
               if (modelData->yMax < tmpSpeedEng) modelData->yMax = tmpSpeedEng;

               mgl_data_set_value((HMDT)modelData->speedEng, tmpSpeedEng, i, 0, 0);

               //g_print("%f\t %f\t %f\t %f\n", mgl_data_get_value((HMDT)modelData->time, i, 0, 0), mgl_data_get_value((HMDT)modelData->torqueEng, i, 0, 0), mgl_data_get_value((HMDT)modelData->accelEng, i, 0, 0), mgl_data_get_value((HMDT)modelData->speedEng, i, 0, 0));
               //g_print("%5.3f\t %5.3f\t %5.3f\t %5.3f\n", mgl_data_get_value((HMDT)modelData->time, i, 0, 0), mgl_data_get_value((HMDT)modelData->speedEngSetpoint, i, 0, 0), tmpSpeedEng, mgl_data_get_value((HMDT)modelData->inputEng, i, 0, 0));
          }

          modelData->yMin = modelData->yMin * 0.98;
          modelData->yMax = modelData->yMax * 1.02;
     }

     return 0;
}

void update_plot_callback(GtkButton *button, ModelData *modelData) {

     mgl_clf((HMGL)modelData->plotSpeed);
     mgl_clf((HMGL)modelData->plotLoad);
     mgl_clf((HMGL)modelData->plotSet);
     mgl_clf((HMGL)modelData->plotInput);

     //g_print ("plot update callback\n");
     //g_print ("modelData.n changed %f\n", modelData->n);
     //g_print ("modelData.w_s changed %f\n", modelData->w_s);
     //g_print ("modelData.w changed %f\n", modelData->w);

     // these part is only for data filling in testing

     calculate_data(modelData);

     // these part is only for data filling in testing

     setup_plot_in_pixbuf(modelData);
     gtk_image_set_from_pixbuf (GTK_IMAGE(plotImageSpeed), pixbufForSpeed);
     gtk_image_set_from_pixbuf (GTK_IMAGE(plotImageLoad), pixbufForLoad);
     gtk_image_set_from_pixbuf (GTK_IMAGE(plotImageSet), pixbufForSet);
     gtk_image_set_from_pixbuf (GTK_IMAGE(plotImageInput), pixbufForInput);

     if ( ( !modelData->consoleMode ) && ( modelData->plotCreated ) )
          resize_plot (modelData);

     modelData->plotCreated = TRUE;

}

/*
 void recreate_plot_callback(GtkButton *button, ModelData *modelData) {

     //can be used only if all plot data was deleted
     //use after delete_plot_callback

     modelData->plotSpeed = mgl_create_graph(modelData->plotWidth, modelData->plotHeight); // create graph

     g_print ("plot update callback\n");
     g_print ("modelData.n changed %f\n", modelData->n);
     g_print ("modelData.w_s changed %f\n", modelData->w_s);
     g_print ("modelData.w changed %f\n", modelData->w);

     // these part is only for data filling in testing

     calculate_data(modelData);

     // these part is only for data filling in testing

     setup_plot_in_pixbuf(modelData);

     gtk_image_set_from_pixbuf (GTK_IMAGE(plotImageSpeed), pixbufForSpeed);

     if (modelData->plotCreated == FALSE) gtk_widget_show(plotImageSpeed);
     modelData->plotCreated = TRUE;

}
*/

int delete_plot_data(ModelData *modelData) {

     mgl_delete_data((HMDT)modelData->time); // delete datasets
     mgl_delete_data((HMDT)modelData->inputEng);
     mgl_delete_data((HMDT)modelData->torqueEng);
     mgl_delete_data((HMDT)modelData->torqueLoad);
     mgl_delete_data((HMDT)modelData->accelEng);
     mgl_delete_data((HMDT)modelData->speedEng);
     mgl_delete_data((HMDT)modelData->speedEngSetpoint);

     return 0;
}

int delete_plot(ModelData *modelData) {

     if (modelData->plotCreated == FALSE) {

          delete_plot_data(modelData);

          modelData->plotCreated = FALSE;

     }

     g_object_unref(pixbufForSpeed);

     return 0;
}
/*
 void clear_plot_callback(GtkButton *button, ModelData *modelData) {

     if (modelData->plotCreated == FALSE) {

          gtk_widget_hide(plotImageSpeed);

          delete_plot_data(modelData);

          mgl_clf((HMGL)modelData->plotSpeed); // cleaning plot

          //mgl_delete_graph(modelData->plotSpeed); // delete plot structure

          modelData->plotCreated = FALSE;
     }
}


 void delete_plot_callback(GtkButton *button, ModelData *modelData) {

     if (modelData->plotCreated == FALSE) {

          gtk_widget_hide(plotImageSpeed);

          delete_plot_data(modelData);

          mgl_delete_graph(modelData->plotSpeed); // delete plot structure
          g_object_unref(pixbufForSpeed);

          modelData->plotCreated = FALSE;
     }
}
*/

int save_to_file (char *filename, ModelData *modelData) {

     time_t rawTime;
     struct tm *timeInfo;
     time (&rawTime);
     timeInfo = localtime (&rawTime);

     FILE *pFile;
     int i = 0;
     int n = (int) modelData->timeMax / modelData->timeStep;

     pFile = fopen (filename, "w");

     if (pFile == NULL) {

          printf("Unable to open file for write operation!\n");

     } else {

          fprintf (pFile, "************************************\n");
          fprintf (pFile, "%s\n", asctime (timeInfo));
          fprintf (pFile, "Source data:\n\n");

          fprintf (pFile, "Inertion factor: %5.3f\nInput factor: %5.3f\nSpeed factor: %5.3f\n",
               modelData->inertFac,
               modelData->inputFac,
               modelData->speedFac);

          fprintf (pFile, "Load: %6.3f\nDesired speed: %5.3f\nInitial speed: %5.3f\n",
               modelData->initTorqueLoad,
               modelData->speedSetpoint,
               modelData->initSpeedEng);

          if (typeOfPID == 0) {

               fprintf (pFile, "P: %5.3f I: %5.3f D: %5.3f\n",
                    modelData->ctrl.pGain,
                    modelData->ctrl.iGain,
                    modelData->ctrl.dGain);

               fprintf (pFile, "%5.3f <  Isum < %5.3f\n\n",
                    modelData->ctrl.iNeg,
                    modelData->ctrl.iPos);

          }


          if (typeOfPID == 1) {

               fprintf (pFile, "P: %5.3f pWeight: %5.3f iGain: %5.3f\n",
                    modelData->ctrl.pGain,
                    modelData->ctrl.pWeight,
                    modelData->ctrl.iGain);

               fprintf (pFile, "bDiff: %5.3f aDiff: %5.3f\n",
                    modelData->ctrl.bDiff,
                    modelData->ctrl.aDiff);

               fprintf (pFile, "%5.3f <  Isum < %5.3f\n\n",
                    modelData->ctrl.iNeg,
                    modelData->ctrl.iPos);
          }

          //fprintf (pFile,"Inertion factor: %5.3f\tInput factor: %5.3f\tSpeed factor: %5.3f\nLoad: %6.3f\tDesired speed: %5.3f\tInitial speed: %5.3f\nP: %5.3f\tI: %5.3f\tD: %5.3\n%5.3f  Isum  %5.3f\n",

          fprintf (pFile,"************************************\n");
          fprintf (pFile,"Time \tSet  \tCurr \tCtrl\n");
          fprintf (pFile,"************************************\n");

          for (i = 0; i < n; i++) {

               fprintf (pFile, "%5.3f\t%5.3f\t%5.3f\t%5.3f\n",
                        mgl_data_get_value ((HMDT)modelData->time, i, 0, 0),
                        mgl_data_get_value ((HMDT)modelData->speedEngSetpoint, i, 0, 0),
                        mgl_data_get_value ((HMDT)modelData->speedEng, i, 0, 0),
                        mgl_data_get_value ((HMDT)modelData->inputEng, i, 0, 0));

          }

     }

     fclose (pFile);

     return 0;
}

void buttonSave_clicked (ModelData *modelData) {

     GtkWidget *dialog;

     dialog = gtk_file_chooser_dialog_new ("Save File As ...", GTK_WINDOW (window),
                                           GTK_FILE_CHOOSER_ACTION_SAVE,
                                           ("_Cancel"),
                                           GTK_RESPONSE_CANCEL,
                                           ("_Save"),
                                           GTK_RESPONSE_ACCEPT,
                                           NULL);
     gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

     if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {

          char *filename;
          filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

          gtk_button_set_label (GTK_BUTTON (buttonSave), filename);
          save_to_file (filename, modelData);

          g_free (filename);
     }

     gtk_widget_destroy (dialog);

}

void buttonLoadTable_clicked (GtkButton *button, size_t pLookupTable) {

     GtkWidget *dialog;

     dialog = gtk_file_chooser_dialog_new ("Open Table", GTK_WINDOW (window),
                                           GTK_FILE_CHOOSER_ACTION_OPEN,
                                           ("_Cancel"),
                                           GTK_RESPONSE_CANCEL,
                                           ("_Open"),
                                           GTK_RESPONSE_ACCEPT,
                                           NULL);
     //gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

     if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {

          char *filename;
          filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

          gtk_button_set_label (GTK_BUTTON (button), "Loaded");
          pLookupTable = (size_t) (prepare_lookup_table (filename));

          g_free (filename);
     }

     gtk_widget_destroy (dialog);

}

void use_tables_callback (GtkToggleButton *toggleButton, gpointer userData) {

     if (gtk_toggle_button_get_active (toggleButton)) {

          useTable = 1;
          //printf ("Use tables is active\n");

     } else {

          useTable = 0;
          //printf ("Use tables is inactive\n");

     }

}

void spin_changed(ModelData *modelData) {

     modelData->timeMax = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_timeMax ) );
     //g_print ("modelData.timeMax changed %f\n", modelData->timeMax);
     modelData->timeStep = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_timeStep ) );
     //g_print ("modelData.timeStep changed %f\n", modelData->timeStep);
     modelData->inertFac = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_inertFac ) );
     //g_print ("modelData.inertFac changed %f\n", modelData->inertFac);
     modelData->inputFac = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_inputFac ) );
     //g_print ("modelData.inputFac changed %f\n", modelData->inputFac);
     modelData->speedFac = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_speedFac ) );
     //g_print ("modelData.speedFac changed %f\n", modelData->speedFac);
     modelData->inputLag = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_inputLag ) );
     //g_print ("modelData.inputLag changed %d\n", modelData->inputLag);
     modelData->ctrl.pGain = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_P ) );
     //g_print ("pGain changed %f\n", modelData->ctrl.pGain);
     modelData->ctrl.iGain = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_I ) );
     //g_print ("iGain changed %f\n", modelData->ctrl.iGain);
     modelData->ctrl.dGain = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_D ) );
     //g_print ("dGain changed %f\n", modelData->ctrl.dGain);
     modelData->ctrl.iPos = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_iPos ) );
     //g_print ("iPos changed %f\n", modelData->ctrl.iPos);
     modelData->ctrl.iNeg = - gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_iNeg ) );
     //g_print ("iNeg changed %f\n", modelData->ctrl.iNeg);
     modelData->ctrl.clockTime = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_ctrlClockTime ) );
     //g_print ("ctrl.clockTime changed %f\n", modelData->ctrl.clockTime);
     modelData->ctrl.pWeight = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_pWeight ) );
     //g_print ("ctrl.pWeight changed %f\n", modelData->ctrl.pWeight);
     modelData->ctrl.bDiff = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_bDiff ) );
     //g_print ("ctrl.bDiff changed %f\n", modelData->ctrl.bDiff);
     modelData->ctrl.aDiff = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_aDiff ) );
     //g_print ("ctrl.aDiff changed %f\n", modelData->ctrl.aDiff);

     modelData->initTorqueLoad = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_initTorqueLoad ) );
     //g_print ("modelData.initTorqueLoad changed %f\n", modelData->initTorqueLoad);

     modelData->loadSetpoint = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_loadSetpoint ) );
     //g_print ("modelData.loadSetpoint changed %f\n", modelData->loadSetpoint);

     modelData->initSpeedEng = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_initSpeedEng ) );
     //g_print ("modelData.initSpeedEng changed %f\n", modelData->initSpeedEng);

     modelData->speedSetpoint = gtk_spin_button_get_value (GTK_SPIN_BUTTON ( spin_speedSetpoint ) );
     //g_print ("modelData.speedSetpoint changed %f\n", modelData->speedSetpoint);

     modelData->inputSetpoint = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_inputSetpoint ) );

     modelData->timeLoadRise = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_timeLoadRise ) );
     modelData->timeLoadFall = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_timeLoadFall ) );
     modelData->timeSpeedRise = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_timeSpeedRise ) );
     modelData->timeSpeedFall = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_timeSpeedFall ) );

     modelData->timeInputRise = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_timeInputRise ) );
     modelData->timeInputFall = gtk_spin_button_get_value ( GTK_SPIN_BUTTON ( spin_timeInputFall ) );

     if ( modelData->timeLoadRise > modelData->timeLoadFall ) modelData->timeLoadFall = modelData->timeMax;
     if ( modelData->timeSpeedRise > modelData->timeSpeedFall ) modelData->timeSpeedFall = modelData->timeMax;

     if ( modelData->timeInputRise > modelData->timeInputFall ) modelData->timeInputFall = modelData->timeInputRise;
     //g_print ("timeLoadRise %f\n", modelData->timeLoadRise);
     //g_print ("timeLoadFall %f\n", modelData->timeLoadFall);
     //g_print ("timeSpeedRise %f\n", modelData->timeSpeedRise);
     //g_print ("timeSpeedFall %f\n", modelData->timeSpeedFall);

     //update_plot_callback(NULL, modelData);

}

void combo_changed ( GtkComboBox *combo, gpointer userData ) {

     typeOfPID = gtk_combo_box_get_active ( combo );

     //g_print ("Combo changed %d\n", typeOfPID);

     if (typeOfPID == 0) {

          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_D ), GTK_STATE_FLAG_NORMAL, TRUE );
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_pWeight ), GTK_STATE_FLAG_INSENSITIVE, TRUE );
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_bDiff ), GTK_STATE_FLAG_INSENSITIVE, TRUE );
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_aDiff ), GTK_STATE_FLAG_INSENSITIVE, TRUE );
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_iPos ), GTK_STATE_FLAG_INSENSITIVE, TRUE );
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_iNeg ), GTK_STATE_FLAG_INSENSITIVE, TRUE );

     }

     if (typeOfPID == 1) {

          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_D ), GTK_STATE_FLAG_INSENSITIVE, TRUE);
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_pWeight ), GTK_STATE_FLAG_NORMAL, TRUE);
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_bDiff ), GTK_STATE_FLAG_NORMAL, TRUE);
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_aDiff ), GTK_STATE_FLAG_NORMAL, TRUE);
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_iPos ), GTK_STATE_FLAG_NORMAL, TRUE );
          gtk_widget_set_state_flags ( GTK_WIDGET ( spin_iNeg ), GTK_STATE_FLAG_NORMAL, TRUE );

     }

}

int resize_plot (ModelData *modelData) {

     GtkAllocation* alloc = g_new ( GtkAllocation, 1 ) ;
     gtk_widget_get_allocation ( scrolledSpeed, alloc ); // assumed that size of all plot scrolledWindows is the same

     // g_print ("Plot size changed: %dx%d\n", alloc->width, alloc->height);

     scaledPixSpeed = gdk_pixbuf_scale_simple ( pixbufForSpeed, alloc->width, alloc->height, GDK_INTERP_BILINEAR );
     scaledPixSet = gdk_pixbuf_scale_simple ( pixbufForSet, alloc->width, alloc->height, GDK_INTERP_BILINEAR );
     scaledPixLoad = gdk_pixbuf_scale_simple ( pixbufForLoad, alloc->width, alloc->height, GDK_INTERP_BILINEAR );
     scaledPixInput = gdk_pixbuf_scale_simple ( pixbufForInput, alloc->width, alloc->height, GDK_INTERP_BILINEAR );

     if ( (scaledPixSpeed == NULL) || (scaledPixLoad == NULL) || (scaledPixSet == NULL) || (scaledPixInput == NULL) ) {

          g_printerr("Failed to resize image\n");
          return TRUE;

     }

     gtk_image_set_from_pixbuf (GTK_IMAGE ( plotImageSpeed ), scaledPixSpeed );
     gtk_image_set_from_pixbuf (GTK_IMAGE ( plotImageSet ), scaledPixSet );
     gtk_image_set_from_pixbuf (GTK_IMAGE ( plotImageLoad ), scaledPixLoad );
     gtk_image_set_from_pixbuf (GTK_IMAGE ( plotImageInput ), scaledPixInput );

     g_free(alloc);

}

gboolean resize_plot_callback (GtkWidget* widget, ModelData* modelData) {

     gtk_window_get_size ( GTK_WINDOW ( window ), & ( modelData->newSize.width ), & ( modelData->newSize.height ) );

     if ( ( modelData->oldSize.width != modelData->newSize.width ) || ( modelData->oldSize.height != modelData->newSize.height ) ) {

          resize_plot ( modelData );

          modelData->oldSize = modelData->newSize;

     }

  return FALSE;
}


gboolean window_state_changed ( GtkWidget* window, GdkEventWindowState* event, ModelData* modelData) {

  if ( event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED ) {

     GtkAllocation* alloc = g_new(GtkAllocation, 1);
     gtk_widget_get_allocation(scrolledSpeed, alloc);

     scaledPixSpeed = gdk_pixbuf_scale_simple(pixbufForSpeed, alloc->width, alloc->height, GDK_INTERP_BILINEAR);

     if ( scaledPixSpeed == NULL ) {

          g_printerr ( "Failed to resize image\n" );
          return TRUE;

     }

     gtk_image_set_from_pixbuf ( GTK_IMAGE(plotImageSpeed), scaledPixSpeed );

     g_free ( alloc );

  }

  return FALSE;

}

FILE* open_file ( char filename[100] ) {

    FILE* pFile = NULL;

    pFile = fopen ( filename , "r" );

    if ( pFile != NULL ) {

         //printf ( "Loading file %s...\n", filename );

    } else {

         printf ( "Error! File %s not found!\n", filename );

    }

    return pFile;

}

LookupTable* prepare_lookup_table (char* filename) {

    FILE* pFile = open_file ( filename );

    LookupTable* pLookupTable = NULL;

    if ( pFile != NULL ) {

     int maxNumOfSymbols = 0;
     int maxNumOfWords = 0;
     int numOfLines = 0;

     pLookupTable = malloc(3 * sizeof(int) + sizeof (size_t) + 5 * 3 * sizeof (double));

     if ( count_words_symbols ( &maxNumOfSymbols, &maxNumOfWords, &numOfLines, pFile ) != 0 )
         printf ( "Error is in function count_words_symbols... \n");

     pLookupTable->maxNumOfSymbols = maxNumOfSymbols;
     pLookupTable->maxNumOfWords = maxNumOfWords;
     pLookupTable->numOfLines = numOfLines;

     pLookupTable->dataTable = new_table(numOfLines, maxNumOfWords);

     //printf("numOfLines: %d;\nmaxNumOfWords, found in line: %d;\nmaxNumOfSymbols, found in line: %d;\n\n", pLookupTable->numOfLines, pLookupTable->maxNumOfWords, pLookupTable->maxNumOfSymbols);

     rewind ( pFile );

     if ( fill_table ( pLookupTable->dataTable , maxNumOfSymbols, maxNumOfWords, numOfLines, pFile ) != 0 )
         printf ( "Error is in function fill_table... \n" );

     fclose ( pFile );


      int i = 0;
      int j = 0;

//      for (i = 0; i <= pLookupTable->numOfLines - 1; i++ ) {
//           for (j = 0; j <= pLookupTable->maxNumOfWords - 1; j++) {
//           printf ("%6.1f ", *(pLookupTable->dataTable + (i * pLookupTable->maxNumOfWords) + j));
//           }
//           printf ("\n");
//      }

    }

    return pLookupTable;

}

int count_words_symbols (int* maxNumOfSymbols, int* maxNumOfWords, int* numOfLines, FILE* pFile ) {

    int c = 0;
    int inWord = 0;
    int nOfWords = 0;
    int nOfSymbols = 0;
    int nOfLines = 0;

    * maxNumOfSymbols = 0;
    * maxNumOfWords = 0;
    * numOfLines = 0;

    while (1) {

     inWord = 0;
     nOfSymbols = 0;
     nOfWords = 0;

     while (1) {

         c = getc( pFile );
         //printf("%c", (char)c);

         if (( c == '\n') || ( c == EOF )) {

          break;

         } else {

          nOfSymbols++;

          if (c == ' ' || c == '\t')
              inWord = 0;

          else if ( inWord == 0 ) {

              inWord = 1;
              nOfWords++;

          }
         }
     }

     if ( * maxNumOfSymbols < nOfSymbols )
         * maxNumOfSymbols = nOfSymbols;

     if ( * maxNumOfWords < nOfWords )
         * maxNumOfWords = nOfWords;

     if ( nOfWords != 0 )
         ++nOfLines;

     if (c == EOF) {

       break;

     }

    }

     *numOfLines = nOfLines;

//      printf("Result: %d lines %d columns successfully loaded!\n", * numOfLines, * maxNumOfWords);

    return 0;

}

int parse_line ( char* pStringFromFile, double* outputArray) {

    //printf ( "\nI'm parsing lines\n" );

    int i = 0;

    char * pLexemeFromFile = NULL;

    pLexemeFromFile = strtok ( pStringFromFile," ;" );

    while ( pLexemeFromFile != NULL ) {

        *( outputArray + i ) = atof ( pLexemeFromFile );

        //printf ( "%f\t" , atof ( pLexemeFromFile ) );

        pLexemeFromFile = strtok (NULL, " ;");

        i++;

        }
    //printf ("\n");
    return 0;
}

double * new_table ( int numOfLines, int numOfWordsInLine ) {

    double * pDataTable = NULL;

    pDataTable = malloc ( numOfLines * numOfWordsInLine * sizeof (double) );

    return pDataTable;

    }


int fill_table ( double* pDataTable , int maxNumOfSymbols, int maxNumOfWords, int numOfLines, FILE* pFile ) {

    //printf ( "I'm filling data table!\n" );

    char pStringFromFile [maxNumOfSymbols];

    int i = 0;

    for ( i = 0 ; i < numOfLines; i++ ) {

        fgets ( pStringFromFile , (maxNumOfSymbols + 2), pFile ); // terminating characters of string should be included
        //printf ( "%s" , pStringFromFile );

        if ( parse_line ( pStringFromFile, (pDataTable + i * maxNumOfWords) ) != 0 )
         printf ( "Error is in function parse_line... \n" );

        }

    return 0;

}

int get_corner_position ( LookupTable* pLookupTable, double x, double y, int* line, int* column ) {

     int goal = 0;

     int i = 1;
     int j = 1;

     while ( ( goal != 1 ) || ( j < ( pLookupTable->maxNumOfWords ) ) ) {

          if ( ( x >= *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + j) ) && ( x < *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + (j + 1)))) {

               goal = 1;
               *column = j;

          }

          j++;

     }

     goal = 0;

     while ( ( goal != 1 ) || ( i < ( pLookupTable->numOfLines ) ) ) {

          if ( ( y >= *(pLookupTable->dataTable + (i * pLookupTable->maxNumOfWords) + 0) ) && ( y < *(pLookupTable->dataTable + ((i + 1) * pLookupTable->maxNumOfWords) + 0))) {

               goal = 1;
               *line = i;

          }

          i++;

     }

     if ( y >= *( pLookupTable->dataTable + ( ( pLookupTable->numOfLines - 1 ) * pLookupTable->maxNumOfWords ) + 0) )
          *line = pLookupTable->numOfLines - 2;

     if ( x >= *( pLookupTable->dataTable + ( 0 * pLookupTable->maxNumOfWords ) + ( pLookupTable->maxNumOfWords - 1 ) ) )
          *column = pLookupTable->maxNumOfWords - 2;

     //printf ("line = %d column = %d\n", *line, *column);

     return 0;

}

double interpolate ( Node* pNode1, Node* pNode2, Node* pNode3, Node* pNode4, Node* pPoint ) {

    double refVal1 = 0;
    double refVal2 = 0;

    refVal1 = pNode1->value * (pNode2->x - pPoint->x) / (pNode2->x - pNode1->x) + pNode2->value * (pPoint->x - pNode1->x) / (pNode2->x - pNode1->x);
    //printf ( "refVal1 = %f\n", refVal1 );

    refVal2 = pNode3->value * (pNode2->x - pPoint->x) / (pNode2->x - pNode1->x) + pNode4->value * (pPoint->x - pNode1->x) / (pNode2->x - pNode1->x);
    //printf ( "refVal2 = %f\n", refVal2 );

    pPoint->value = refVal2 * (pNode1->y - pPoint->y) / (pNode1->y - pNode3->y) + refVal1 * (pPoint->y - pNode3->y) / (pNode1->y - pNode3->y);
    //printf ( "Ppoint->value = %f\n", pPoint->value );

    return pPoint->value;

    }

double get_data_from_table ( LookupTable* pLookupTable, double x, double y ) {

     int cornerLineNumber = 0;
     int cornerColumnNumber = 0;

     double value = 0;


     if (( x <  *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + 1) ) ||
         ( x > *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + (pLookupTable->maxNumOfWords - 1))) ||
         ( y <  *(pLookupTable->dataTable + (1 * pLookupTable->maxNumOfWords) + 0) ) ||
         ( y > *(pLookupTable->dataTable + ((pLookupTable->numOfLines - 1) * pLookupTable->maxNumOfWords) + 0))) {

          printf ( "Requested values are out of data table's range (%f < x < %f) or (%f < y < %f) \n",
          *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + 1),
          *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + (pLookupTable->maxNumOfWords - 1)),
          *(pLookupTable->dataTable + (1 * pLookupTable->maxNumOfWords) + 0),
          *(pLookupTable->dataTable + ((pLookupTable->numOfLines - 1) * pLookupTable->maxNumOfWords) + 0));

     } else {

          if (get_corner_position (pLookupTable, x, y, &cornerLineNumber, &cornerColumnNumber)) {

               printf ("Error while finding corner coordinates for interpolation!\n");

          } else {

               pLookupTable->node1.x =     *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + cornerColumnNumber);
               pLookupTable->node1.y =     *(pLookupTable->dataTable + (cornerLineNumber * pLookupTable->maxNumOfWords) + 0);
               pLookupTable->node1.value = *(pLookupTable->dataTable + (cornerLineNumber * pLookupTable->maxNumOfWords) + cornerColumnNumber);

               //printf ( "Node 1: x = %f , y = %f , value = %f \n", pLookupTable->node1.x, pLookupTable->node1.y, pLookupTable->node1.value );

               pLookupTable->node2.x =     *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + (cornerColumnNumber + 1));
               pLookupTable->node2.y =     *(pLookupTable->dataTable + (cornerLineNumber * pLookupTable->maxNumOfWords) + 0);
               pLookupTable->node2.value = *(pLookupTable->dataTable + (cornerLineNumber * pLookupTable->maxNumOfWords) + (cornerColumnNumber + 1));

               //printf ( "Node 2: x = %f , y = %f , value = %f \n", pLookupTable->node2.x, pLookupTable->node2.y, pLookupTable->node2.value );

               pLookupTable->node3.x =     *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + cornerColumnNumber);
               pLookupTable->node3.y =     *(pLookupTable->dataTable + ((cornerLineNumber + 1) * pLookupTable->maxNumOfWords) + 0);
               pLookupTable->node3.value = *(pLookupTable->dataTable + ((cornerLineNumber + 1) * pLookupTable->maxNumOfWords) + cornerColumnNumber);

               //printf ( "Node 3: x = %f , y = %f , value = %f \n", pLookupTable->node3.x, pLookupTable->node3.y, pLookupTable->node3.value );

               pLookupTable->node4.x =     *(pLookupTable->dataTable + (0 * pLookupTable->maxNumOfWords) + (cornerColumnNumber + 1));
               pLookupTable->node4.y =     *(pLookupTable->dataTable + ((cornerLineNumber + 1) * pLookupTable->maxNumOfWords) + 0);
               pLookupTable->node4.value = *(pLookupTable->dataTable + ((cornerLineNumber + 1) * pLookupTable->maxNumOfWords) + (cornerColumnNumber + 1));

               //printf ( "Node 4: x = %f , y = %f , value = %f \n", pLookupTable->node4.x, pLookupTable->node4.y, pLookupTable->node4.value );

               pLookupTable->point.x = x;
               pLookupTable->point.y = y;
               pLookupTable->point.value = 0;

               value = interpolate ( &pLookupTable->node1, &pLookupTable->node2, &pLookupTable->node3, &pLookupTable->node4, &pLookupTable->point );

               //printf ("Value %f\n", value);


          }

     }

  return value;
}

int free_lookup_table ( size_t pLookupTable ) {

     if ( pLookupTable != 0 ) {
          //printf ("Free lookup table called!\n");
          free ( ( ( LookupTable * ) pLookupTable )->dataTable );
          free ( ( LookupTable * ) pLookupTable );

     }
}

void exit_callback ( ModelData* modelData ) {

     gtk_main_quit();
     free_lookup_table ( modelData->pTrqTable );
     free_lookup_table ( modelData->pRailTable );
     delete_plot(modelData);

}
