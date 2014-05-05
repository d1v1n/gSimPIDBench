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

int setup_plot_in_pixbuf(ModelData *modelData) {

     mgl_set_ranges((HMGL)modelData->plot, modelData->timeMin, modelData->timeMax, modelData->yMin, modelData->yMax, -1.0, 1.0); // values for x, y, z axis
     mgl_axis((HMGL)modelData->plot, "xyz", "", "");
     mgl_axis_grid((HMGL)modelData->plot,"xy" ,"" ,"" );
     mgl_label((HMGL)modelData->plot, 'x', "Time, seconds", 0, "");
     mgl_label((HMGL)modelData->plot, 'y', "Y-axis", 0, "");

     mgl_plot_xy ((HMGL)modelData->plot, (HMDT)modelData->time, (HMDT)modelData->speedEng, "", ""); // plot data

     const unsigned char *imageInMemory = mgl_get_rgb((HMGL)modelData->plot); // get pointer on plot in memory
     modelData->pixbufForPlot = gdk_pixbuf_new_from_data (imageInMemory, GDK_COLORSPACE_RGB, FALSE, 8, modelData->plotWidth, modelData->plotHeight, modelData->plotWidth * 3, NULL, NULL); // making pixbuf from memory data...

     return 0;
}

int initiate_data_for_plots(ModelData *modelData){

     modelData->time = (size_t)mgl_create_data_size(1, 1, 1);
     modelData->inputEng = (size_t)mgl_create_data_size(1, 1, 1);
     modelData->torqueEng = (size_t)mgl_create_data_size(1, 1, 1);
     modelData->torqueLoad = (size_t)mgl_create_data_size(1, 1, 1);
     modelData->accelEng = (size_t)mgl_create_data_size(1, 1, 1);
     modelData->speedEng = (size_t)mgl_create_data_size(1, 1, 1);
     modelData->speedEngSetpoint = (size_t)mgl_create_data_size(1, 1, 1);

     return 0;
}

int initiate_plot(ModelData *modelData) {

     modelData->plot = (size_t)mgl_create_graph(modelData->plotWidth, modelData->plotHeight); // create graph
     setup_plot_in_pixbuf(modelData);
     modelData->plotImage = gtk_image_new_from_pixbuf(modelData->pixbufForPlot); // making image widget from pixbuf

     update_plot_callback(NULL, modelData);

     modelData->plotCreated = TRUE;

     //printf("Plot initiated\n");

     return 0;
}

int validate_data(ModelData *modelData) {

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

double update_controller_output_simple(PID *ctrl, double setponit, double position) {

     double pTerm = 0;
     double dTerm = 0;
     double iTerm = 0;
     double error = setponit - position;

     pTerm = ctrl->pGain * error; // calculate the proportional term

     ctrl->iState += error; // calculate the integral state with appropriate limiting

     if (ctrl->iState > ctrl->iPos) ctrl->iState = ctrl->iPos;

     if (ctrl->iState < ctrl->iNeg) ctrl->iState = ctrl->iNeg;

     iTerm = ctrl->iGain * ctrl->iState; // calculate the integral term

     dTerm = ctrl->dGain * (error - ctrl->dState); // calculate the derivative term

     ctrl->dState = error;

     return (pTerm + iTerm + dTerm); // return controller output
}

double update_controller_output_astrom (PID *ctrl, double setpoint, double position) {

     double pTerm = 0;
     double dTerm = 0;
     double iTerm = 0;
     double error = setpoint - position;

     pTerm = ctrl->pGain * (ctrl->pWeight * setpoint - position); // calculate the proportional term

     ctrl->iState += error; // calculate the integral state with appropriate limiting

     if (ctrl->iState > ctrl->iPos) ctrl->iState = ctrl->iPos;

     if (ctrl->iState < ctrl->iNeg) ctrl->iState = ctrl->iNeg;

     iTerm = ctrl->iGain * ctrl->iState; // calculate the integral term

     ctrl->dState = ctrl->aDiff * ctrl->dState - ctrl->bDiff * (position - ctrl->pastPos); // calculate the derivative term

     dTerm = ctrl->dState;
     ctrl->pastPos = position;

     return (pTerm + iTerm + dTerm); // return controller output
}

int calculate_input_signal (ModelData *modelData, int n) {

     int i = 0;
     int loadRisePoint = (int) (modelData->timeLoadRise / modelData->timeStep);
     int loadFallPoint = (int) (modelData->timeLoadFall / modelData->timeStep);
     int speedRisePoint = (int) (modelData->timeSpeedRise / modelData->timeStep);
     int speedFallPoint = (int) (modelData->timeSpeedFall / modelData->timeStep);

     int inputRisePoint = (int) (modelData->timeInputRise / modelData->timeStep);
     int inputFallPoint = (int) (modelData->timeInputFall / modelData->timeStep);
     modelData->tmpControlAction = ((modelData->initTorqueLoad - modelData->speedFac * modelData->initSpeedEng) / (/*0.01 **/ modelData->inputFac)); // stationary conditions

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


int calculate_data(ModelData *modelData) {

     if (validate_data(modelData)) {

          int i = 0;
          int n = (int) modelData->timeMax / modelData->timeStep;

          //g_print("calculate_data call\n");
          //g_print("number of iterations = %i\n", n);
          double lastCtrlTime = 0;
          double tmpTime = 0;
          double tmpTrqEng = 0;
          double tmpAccelEng = 0;
          double tmpSpeedEng = 0;

          // move variable inside dataset, place calculation of it inside calculate_input_signal function
          //double tmpControlAction = ((modelData->initTorqueLoad - modelData->speedFac * modelData->initSpeedEng) / (/*0.01 **/ modelData->inputFac)); // stationary conditions

          // reset controller states!

          modelData->ctrl.dState = 0;
          modelData->ctrl.iState = 0;
          modelData->ctrl.pastPos = 0;

          modelData->time = (size_t)mgl_create_data_size(n, 1, 1);
          modelData->inputEng = (size_t)mgl_create_data_size(n, 1, 1);
          modelData->speedEng = (size_t)mgl_create_data_size(n, 1, 1);

          modelData->speedEngSetpoint = (size_t)mgl_create_data_size(n, 1, 1);
          modelData->torqueLoad = (size_t)mgl_create_data_size(n, 1, 1);

          // in testing purpose
          calculate_input_signal (modelData, n);
          // in testing purpose

          modelData->torqueEng = (size_t)mgl_create_data_size(n, 1, 1);
          modelData->accelEng = (size_t)mgl_create_data_size(n, 1, 1);

          modelData->yMin = modelData->initSpeedEng;
          modelData->yMax = modelData->initSpeedEng;

          mgl_data_set_value((HMDT)modelData->time, 0.0, 0, 0, 0);
          mgl_data_set_value((HMDT)modelData->speedEng, modelData->initSpeedEng, 0, 0, 0);

          // place inside calculate_input_signal function
          //mgl_data_set_value((HMDT)modelData->inputEng, tmpControlAction, 0, 0, 0);

          mgl_data_set_value((HMDT)modelData->torqueEng, modelData->initTorqueLoad, 0, 0, 0);
          mgl_data_set_value((HMDT)modelData->accelEng, 0.0, 0, 0, 0);

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
               mgl_data_set_value((HMDT)modelData->time, tmpTime, i, 0, 0);


               if ((tmpTime - lastCtrlTime) >= modelData->ctrl.clockTime) {

                    lastCtrlTime = tmpTime;

                    if (typeOfPID == 0) modelData->tmpControlAction = update_controller_output_simple ( &modelData->ctrl, mgl_data_get_value((HMDT)modelData->speedEngSetpoint, (i - 1), 0, 0), mgl_data_get_value((HMDT)modelData->speedEng, (i - 1), 0, 0));
                    if (typeOfPID == 1) modelData->tmpControlAction = update_controller_output_astrom ( &modelData->ctrl, mgl_data_get_value((HMDT)modelData->speedEngSetpoint, (i - 1), 0, 0), mgl_data_get_value((HMDT)modelData->speedEng, (i - 1), 0, 0));

                    // actuator limitation
                    if (modelData->tmpControlAction > 1) modelData->tmpControlAction = 1;
                    if (modelData->tmpControlAction < 0) modelData->tmpControlAction = 0;

               }

               if (((tmpTime < modelData->timeInputRise) || (tmpTime > modelData->timeInputFall)) || ((modelData->timeInputRise == 0.0) && (modelData->timeInputFall == 0.0))) {

                    mgl_data_set_value((HMDT)modelData->inputEng, modelData->tmpControlAction, i, 0, 0);

               }

               tmpTrqEng = modelData->inputFac * mgl_data_get_value((HMDT)modelData->inputEng, (i - 1) , 0, 0) + modelData->speedFac * mgl_data_get_value((HMDT)modelData->speedEng, (i - 1), 0, 0);
               mgl_data_set_value((HMDT)modelData->torqueEng, tmpTrqEng, i, 0, 0);

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

     mgl_clf((HMGL)modelData->plot);

     //g_print ("plot update callback\n");
     //g_print ("modelData.n changed %f\n", modelData->n);
     //g_print ("modelData.w_s changed %f\n", modelData->w_s);
     //g_print ("modelData.w changed %f\n", modelData->w);

     // these part is only for data filling in testing

     calculate_data(modelData);

     // these part is only for data filling in testing

     setup_plot_in_pixbuf(modelData);
     gtk_image_set_from_pixbuf (GTK_IMAGE(modelData->plotImage), modelData->pixbufForPlot);
     if (modelData->plotCreated == FALSE) gtk_widget_show(modelData->plotImage);
     modelData->plotCreated = FALSE;

}

/*
 void recreate_plot_callback(GtkButton *button, ModelData *modelData) {

     //can be used only if all plot data was deleted
     //use after delete_plot_callback

     modelData->plot = mgl_create_graph(modelData->plotWidth, modelData->plotHeight); // create graph

     g_print ("plot update callback\n");
     g_print ("modelData.n changed %f\n", modelData->n);
     g_print ("modelData.w_s changed %f\n", modelData->w_s);
     g_print ("modelData.w changed %f\n", modelData->w);

     // these part is only for data filling in testing

     calculate_data(modelData);

     // these part is only for data filling in testing

     setup_plot_in_pixbuf(modelData);

     gtk_image_set_from_pixbuf (GTK_IMAGE(modelData->plotImage), modelData->pixbufForPlot);

     if (modelData->plotCreated == FALSE) gtk_widget_show(modelData->plotImage);
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

     g_object_unref(modelData->pixbufForPlot);

     return 0;
}
/*
 void clear_plot_callback(GtkButton *button, ModelData *modelData) {

     if (modelData->plotCreated == FALSE) {

          gtk_widget_hide(modelData->plotImage);

          delete_plot_data(modelData);

          mgl_clf((HMGL)modelData->plot); // cleaning plot

          //mgl_delete_graph(modelData->plot); // delete plot structure

          modelData->plotCreated = FALSE;
     }
}


 void delete_plot_callback(GtkButton *button, ModelData *modelData) {

     if (modelData->plotCreated == FALSE) {

          gtk_widget_hide(modelData->plotImage);

          delete_plot_data(modelData);

          mgl_delete_graph(modelData->plot); // delete plot structure
          g_object_unref(modelData->pixbufForPlot);

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
          fprintf (pFile, "Sorce data:\n\n");

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

void spin_changed(ModelData *modelData) {

     modelData->timeMax = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_timeMax));
     //g_print ("modelData.timeMax changed %f\n", modelData->timeMax);
     modelData->timeStep = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_timeStep));
     //g_print ("modelData.timeStep changed %f\n", modelData->timeStep);
     modelData->inertFac = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_inertFac));
     //g_print ("modelData.inertFac changed %f\n", modelData->inertFac);
     modelData->inputFac = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_inputFac));
     //g_print ("modelData.inputFac changed %f\n", modelData->inputFac);
     modelData->speedFac = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_speedFac));
     //g_print ("modelData.speedFac changed %f\n", modelData->speedFac);
     modelData->ctrl.pGain = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_P));
     //g_print ("pGain changed %f\n", modelData->ctrl.pGain);
     modelData->ctrl.iGain = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_I));
     //g_print ("iGain changed %f\n", modelData->ctrl.iGain);
     modelData->ctrl.dGain = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_D));
     //g_print ("dGain changed %f\n", modelData->ctrl.dGain);
     modelData->ctrl.iPos = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_iPos));
     //g_print ("iPos changed %f\n", modelData->ctrl.iPos);
     modelData->ctrl.iNeg = - gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_iNeg));
     //g_print ("iNeg changed %f\n", modelData->ctrl.iNeg);
     modelData->ctrl.clockTime = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_ctrlClockTime));
     //g_print ("ctrl.clockTime changed %f\n", modelData->ctrl.clockTime);
     modelData->ctrl.pWeight = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_pWeight));
     //g_print ("ctrl.pWeight changed %f\n", modelData->ctrl.pWeight);
     modelData->ctrl.bDiff = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_bDiff));
     //g_print ("ctrl.bDiff changed %f\n", modelData->ctrl.bDiff);
     modelData->ctrl.aDiff = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_aDiff));
     //g_print ("ctrl.aDiff changed %f\n", modelData->ctrl.aDiff);

     modelData->initTorqueLoad = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_initTorqueLoad));
     //g_print ("modelData.initTorqueLoad changed %f\n", modelData->initTorqueLoad);

     modelData->loadSetpoint = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_loadSetpoint));
     //g_print ("modelData.loadSetpoint changed %f\n", modelData->loadSetpoint);

     modelData->initSpeedEng = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_initSpeedEng));
     //g_print ("modelData.initSpeedEng changed %f\n", modelData->initSpeedEng);

     modelData->speedSetpoint = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_speedSetpoint));
     //g_print ("modelData.speedSetpoint changed %f\n", modelData->speedSetpoint);

     modelData->inputSetpoint = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_inputSetpoint));

     modelData->timeLoadRise = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_timeLoadRise));
     modelData->timeLoadFall = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_timeLoadFall));
     modelData->timeSpeedRise = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_timeSpeedRise));
     modelData->timeSpeedFall = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_timeSpeedFall));

     modelData->timeInputRise = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_timeInputRise));
     modelData->timeInputFall = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_timeInputFall));

     if (modelData->timeLoadRise > modelData->timeLoadFall) modelData->timeLoadFall = modelData->timeMax;
     if (modelData->timeSpeedRise > modelData->timeSpeedFall) modelData->timeSpeedFall = modelData->timeMax;

     if (modelData->timeInputRise > modelData->timeInputFall) modelData->timeInputFall = modelData->timeInputRise;
     //g_print ("timeLoadRise %f\n", modelData->timeLoadRise);
     //g_print ("timeLoadFall %f\n", modelData->timeLoadFall);
     //g_print ("timeSpeedRise %f\n", modelData->timeSpeedRise);
     //g_print ("timeSpeedFall %f\n", modelData->timeSpeedFall);

     //update_plot_callback(NULL, modelData);

}

void combo_changed(GtkComboBox *combo, gpointer *userData) {

     typeOfPID = gtk_combo_box_get_active(combo);

     //g_print ("Combo changed %d\n", typeOfPID);

     if (typeOfPID == 0) {

          gtk_widget_set_state_flags (GTK_WIDGET(spin_D), GTK_STATE_FLAG_NORMAL, TRUE);
          gtk_widget_set_state_flags (GTK_WIDGET(spin_pWeight), GTK_STATE_FLAG_INSENSITIVE, TRUE);
          gtk_widget_set_state_flags (GTK_WIDGET(spin_bDiff), GTK_STATE_FLAG_INSENSITIVE, TRUE);
          gtk_widget_set_state_flags (GTK_WIDGET(spin_aDiff), GTK_STATE_FLAG_INSENSITIVE, TRUE);

     }

     if (typeOfPID == 1) {

          gtk_widget_set_state_flags (GTK_WIDGET(spin_D), GTK_STATE_FLAG_INSENSITIVE, TRUE);
          gtk_widget_set_state_flags (GTK_WIDGET(spin_pWeight), GTK_STATE_FLAG_NORMAL, TRUE);
          gtk_widget_set_state_flags (GTK_WIDGET(spin_bDiff), GTK_STATE_FLAG_NORMAL, TRUE);
          gtk_widget_set_state_flags (GTK_WIDGET(spin_aDiff), GTK_STATE_FLAG_NORMAL, TRUE);

     }

}

gboolean resize_plot (GtkWidget *window, ModelData *modelData) {

     gtk_window_get_size (GTK_WINDOW (window), &(modelData->newSize.width), &(modelData->newSize.height));

     if ((modelData->oldSize.width != modelData->newSize.width) || (modelData->oldSize.height != modelData->newSize.height)) {

          GtkAllocation* alloc = g_new(GtkAllocation, 1);
          gtk_widget_get_allocation(scrolled, alloc);

          modelData->scaledPix = gdk_pixbuf_scale_simple(modelData->pixbufForPlot, alloc->width, alloc->height, GDK_INTERP_BILINEAR);

          if (modelData->scaledPix == NULL) {

               g_printerr("Failed to resize image\n");
               return TRUE;

          }

      gtk_image_set_from_pixbuf(GTK_IMAGE(modelData->plotImage), modelData->scaledPix);

      g_free(alloc);

      modelData->oldSize = modelData->newSize;

  }

  return FALSE;
}


gboolean window_state_changed (GtkWidget *window, GdkEventWindowState *event, ModelData *modelData) {

  if (event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) {

     GtkAllocation* alloc = g_new(GtkAllocation, 1);
     gtk_widget_get_allocation(scrolled, alloc);

     modelData->scaledPix = gdk_pixbuf_scale_simple(modelData->pixbufForPlot, alloc->width, alloc->height, GDK_INTERP_BILINEAR);

     if (modelData->scaledPix == NULL) {

          g_printerr("Failed to resize image\n");
          return TRUE;

     }

     gtk_image_set_from_pixbuf(GTK_IMAGE(modelData->plotImage), modelData->scaledPix);

     g_free(alloc);

  }

  return FALSE;

}

void exit_callback (ModelData *modelData) {

     gtk_main_quit();
     delete_plot(modelData);

}