#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stddef.h>
#include <gtk/gtk.h>
#include "dataset.h"

#define lim_timeMax_B 0
#define lim_timeStep_B 0
#define lim_inertFac_B 0
#define lim_inputFac_B 0
#define lim_speedFac_B 0
#define lim_initTorque_B 0
#define lim_speedSetpoint_B 0
#define lim_initSpeedEng_B 0
#define lim_pGain_B 0
#define lim_iGain_B 0
#define lim_dGain_B 0
#define lim_iPos_B 0
#define lim_iNeg_B 0
#define lim_ctrlClockTime_B 0
#define lim_pWeight_B 0
#define lim_bDiff_B 0
#define lim_aDiff_B -100

#define lim_timeMax_T 300
#define lim_timeStep_T 0.5
#define lim_inertFac_T 5000
#define lim_inputFac_T 5000
#define lim_speedFac_T 5000
#define lim_initTorque_T 5000
#define lim_speedSetpoint_T 100
#define lim_initSpeedEng_T 3000
#define lim_pGain_T 100
#define lim_iGain_T 100
#define lim_dGain_T 100
#define lim_iPos_T 10000
#define lim_iNeg_T 10000
#define lim_ctrlClockTime_T 1
#define lim_pWeight_T 2
#define lim_bDiff_T 100
#define lim_aDiff_T 100

#define TIME_STEP 0.01

GtkWidget *window;
GtkWidget *vboxForPlots;
GtkWidget *vboxForButtons;
GtkWidget *hbox;
GtkWidget *grid_input;
GtkWidget *grid_plant;
GtkWidget *grid_controller;
GtkWidget *notebook;

GtkWidget *buttonUpdate;
GtkWidget *buttonSave;

GtkWidget *label_Input;
GtkWidget *label_loadSetpoint;
GtkWidget *label_initTorqueLoad;
GtkWidget *label_timeLoad;
GtkWidget *label_speedSetpoint;
GtkWidget *label_initSpeedEng;
GtkWidget *label_timeSpeed;

GtkWidget *label_Plant;
GtkWidget *label_timeMax;
GtkWidget *label_timeStep;
GtkWidget *label_inertFac;
GtkWidget *label_inputFac;
GtkWidget *label_speedFac;

GtkWidget *spin_loadSetpoint;
GtkWidget *spin_initTorqueLoad;
GtkWidget *spin_timeLoadRise;
GtkWidget *spin_timeLoadFall;
GtkWidget *spin_speedSetpoint;
GtkWidget *spin_initSpeedEng;
GtkWidget *spin_timeSpeedRise;
GtkWidget *spin_timeSpeedFall;

GtkWidget *spin_timeMax;
GtkWidget *spin_timeStep;
GtkWidget *spin_inertFac;
GtkWidget *spin_inputFac;
GtkWidget *spin_speedFac;

GtkWidget *label_Controller;
GtkWidget *label_P;
GtkWidget *label_I;
GtkWidget *label_D;
GtkWidget *label_iPos;
GtkWidget *label_iNeg;
GtkWidget *label_ctrlClockTime;

GtkWidget *label_pWeight;
GtkWidget *label_bDiff;
GtkWidget *label_aDiff;

GtkWidget *spin_P;
GtkWidget *spin_I;
GtkWidget *spin_D;
GtkWidget *spin_iPos;
GtkWidget *spin_iNeg;
GtkWidget *spin_ctrlClockTime;

GtkWidget *spin_pWeight;
GtkWidget *spin_bDiff;
GtkWidget *spin_aDiff;

GtkWidget *combo_typeOfPID;

//SomeGUIPtrs switchable = {0};

int create_main_window (ModelData *, int *argc, char **argv[]);

#endif // MAINWINDOW_H