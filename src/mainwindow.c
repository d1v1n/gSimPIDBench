#include "mainwindow.h"
#include "dataset.h"
#include "functions.h"

int create_main_window (ModelData *modelData, int *argc, char **argv[]) {

// creating window

     gtk_init(argc, argv);
     window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

     gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
     gtk_window_set_title(GTK_WINDOW(window), "Test View");
     gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
     gtk_container_set_border_width(GTK_CONTAINER(window), 2);

// creating layout elements

     //vboxForPlots = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
     /*
     aspectFrame = gtk_aspect_frame_new (NULL,    // label
                                         0.5,     // center x
                                         0.5,     // center y
                                         1.5,     // xsize/ysize = 1.5
                                         FALSE ); // ignore child's aspect
     */

     scrolled = gtk_scrolled_window_new (NULL, NULL);
     vboxForButtons = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
     hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
     notebook = gtk_notebook_new();
     grid_input = gtk_grid_new ();
     gtk_grid_set_column_homogeneous (GTK_GRID(grid_input), TRUE);
     grid_plant = gtk_grid_new ();
     gtk_grid_set_column_homogeneous (GTK_GRID(grid_plant), TRUE);
     grid_controller = gtk_grid_new ();
     gtk_grid_set_column_homogeneous (GTK_GRID(grid_controller), TRUE);

// creating buttons

     buttonUpdate = gtk_button_new_with_label ("Update plot");
     buttonSave = gtk_button_new_with_label ("Save data");

// creating labels

     label_Input = gtk_label_new ("Input");
     label_loadSetpoint = gtk_label_new ("Load setpoint, N*m");
     gtk_misc_set_alignment (GTK_MISC(label_loadSetpoint), 0, 0);
     label_initTorqueLoad = gtk_label_new ("Initial load torque, N*m");
     gtk_misc_set_alignment (GTK_MISC(label_initTorqueLoad), 0, 0);
     label_timeLoad = gtk_label_new ("Load Rise/Fall time, s");
     gtk_misc_set_alignment (GTK_MISC(label_timeLoad), 0, 0);
     label_speedSetpoint = gtk_label_new ("Engine setpoint, s-1");
     gtk_misc_set_alignment (GTK_MISC(label_speedSetpoint), 0, 0);
     label_initSpeedEng = gtk_label_new ("Initial engine speed, s-1");
     gtk_misc_set_alignment (GTK_MISC(label_initSpeedEng), 0, 0);
     label_timeSpeed = gtk_label_new ("Setpoint Rise/Fall time, s");
     gtk_misc_set_alignment (GTK_MISC(label_timeSpeed), 0, 0);
     label_inputSetpoint = gtk_label_new ("Manual input, n/a");
     gtk_misc_set_alignment (GTK_MISC(label_inputSetpoint), 0, 0);
     label_timeInput = gtk_label_new ("Setpoint Rise/Fall time, s");
     gtk_misc_set_alignment (GTK_MISC(label_timeInput), 0, 0);

     label_Plant = gtk_label_new ("Plant");
     label_timeMax = gtk_label_new ("Max simulation time, c");
     gtk_misc_set_alignment (GTK_MISC(label_timeMax), 0, 0);
     label_timeStep = gtk_label_new ("Time step, c");
     gtk_misc_set_alignment (GTK_MISC(label_timeStep), 0, 0);
     label_inertFac = gtk_label_new ("Inertion of object, kg*m2");
     gtk_misc_set_alignment (GTK_MISC(label_inertFac), 0, 0);
     label_inputFac = gtk_label_new ("Input factor, N*m");
     gtk_misc_set_alignment (GTK_MISC(label_inputFac), 0, 0);
     label_speedFac = gtk_label_new ("Speed factor, N*m*s");
     gtk_misc_set_alignment (GTK_MISC(label_speedFac), 0, 0);


     label_Controller = gtk_label_new ("Controller");
     label_P = gtk_label_new ("P = ");
     gtk_misc_set_alignment (GTK_MISC(label_P), 0, 0);
     label_I = gtk_label_new ("I = ");
     gtk_misc_set_alignment (GTK_MISC(label_I), 0, 0);
     label_D = gtk_label_new ("D = ");
     gtk_misc_set_alignment (GTK_MISC(label_D), 0, 0);
     label_iPos = gtk_label_new ("I positive limit = ");
     gtk_misc_set_alignment (GTK_MISC(label_iPos), 0, 0);
     label_iNeg = gtk_label_new ("I negative limit = ");
     gtk_misc_set_alignment (GTK_MISC(label_iNeg), 0, 0);
     label_ctrlClockTime = gtk_label_new ("Controller clock period, s");
     gtk_misc_set_alignment (GTK_MISC(label_ctrlClockTime), 0, 0);

     label_pWeight = gtk_label_new ("Setpoint weitgh for P = ");
     gtk_misc_set_alignment (GTK_MISC(label_pWeight), 0, 0);
     label_bDiff = gtk_label_new ("b factor for D = ");
     gtk_misc_set_alignment (GTK_MISC(label_bDiff), 0, 0);
     label_aDiff = gtk_label_new ("a factor for D = ");
     gtk_misc_set_alignment (GTK_MISC(label_aDiff), 0, 0);


// creating spin tab_Input
     spin_loadSetpoint = gtk_spin_button_new_with_range(lim_initTorque_B, lim_initTorque_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_loadSetpoint), modelData->loadSetpoint);

     spin_initTorqueLoad = gtk_spin_button_new_with_range(lim_initTorque_B, lim_initTorque_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_initTorqueLoad), modelData->initTorqueLoad);

     spin_timeLoadRise = gtk_spin_button_new_with_range(lim_timeMax_B, lim_timeMax_T, TIME_STEP);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_timeLoadRise), modelData->timeLoadRise);

     spin_timeLoadFall = gtk_spin_button_new_with_range(lim_timeMax_B, lim_timeMax_T, TIME_STEP);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_timeLoadFall), modelData->timeLoadFall);


     spin_speedSetpoint = gtk_spin_button_new_with_range(lim_speedSetpoint_B, lim_speedSetpoint_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_speedSetpoint), modelData->speedSetpoint);

     spin_initSpeedEng = gtk_spin_button_new_with_range(lim_initSpeedEng_B, lim_initSpeedEng_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_initSpeedEng), modelData->initSpeedEng);

     spin_timeSpeedRise = gtk_spin_button_new_with_range(lim_timeMax_B, lim_timeMax_T, TIME_STEP);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_timeSpeedRise), modelData->timeSpeedRise);

     spin_timeSpeedFall = gtk_spin_button_new_with_range(lim_timeMax_B, lim_timeMax_T, TIME_STEP);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_timeSpeedFall), modelData->timeSpeedFall);

     spin_inputSetpoint = gtk_spin_button_new_with_range(lim_inputSetpoint_B, lim_inputSetpoint_T, 0.05);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_inputSetpoint), modelData->inputSetpoint);

     spin_timeInputRise = gtk_spin_button_new_with_range(lim_timeMax_B, lim_timeMax_T, TIME_STEP);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_timeInputRise), modelData->timeInputRise);

     spin_timeInputFall = gtk_spin_button_new_with_range(lim_timeMax_B, lim_timeMax_T, TIME_STEP);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_timeInputFall), modelData->timeInputFall);

// creating spin tab_Plant

     spin_timeMax = gtk_spin_button_new_with_range(lim_timeMax_B, lim_timeMax_T, 1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_timeMax), modelData->timeMax);

     spin_timeStep = gtk_spin_button_new_with_range(lim_timeStep_B, lim_timeStep_T, 0.001);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_timeStep), modelData->timeStep);

     spin_inertFac = gtk_spin_button_new_with_range(lim_inertFac_B, lim_inertFac_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_inertFac), modelData->inertFac);

     spin_inputFac = gtk_spin_button_new_with_range(lim_inputFac_B, lim_inputFac_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_inputFac), modelData->inputFac);

     spin_speedFac = gtk_spin_button_new_with_range(lim_speedFac_B, lim_speedFac_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_speedFac), modelData->speedFac);

// creating spin tab_Controller

     spin_P = gtk_spin_button_new_with_range(lim_pGain_B, lim_pGain_T, 0.0001);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_P), modelData->ctrl.pGain);

     spin_I = gtk_spin_button_new_with_range(lim_iGain_B, lim_iGain_T, 0.0001);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_I), modelData->ctrl.iGain);

     spin_D = gtk_spin_button_new_with_range(lim_dGain_B, lim_dGain_T, 0.0001);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_D), modelData->ctrl.dGain);

     spin_iPos = gtk_spin_button_new_with_range(lim_iPos_B, lim_iPos_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_iPos), modelData->ctrl.iPos);

     spin_iNeg = gtk_spin_button_new_with_range(lim_iNeg_B, lim_iNeg_T, 0.1);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_iNeg), - modelData->ctrl.iNeg);

     spin_ctrlClockTime = gtk_spin_button_new_with_range(lim_ctrlClockTime_B, lim_ctrlClockTime_T, 0.001);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_ctrlClockTime), modelData->ctrl.clockTime);

     spin_pWeight = gtk_spin_button_new_with_range(lim_pWeight_B, lim_pWeight_T, 0.01);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_pWeight), modelData->ctrl.pWeight);

     spin_bDiff = gtk_spin_button_new_with_range(lim_bDiff_B, lim_bDiff_T, 0.0001);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_bDiff), modelData->ctrl.bDiff);

     spin_aDiff = gtk_spin_button_new_with_range(lim_aDiff_B, lim_aDiff_T, 0.0001);
     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_aDiff), modelData->ctrl.aDiff);

// creating combo

     combo_typeOfPID = gtk_combo_box_text_new ();

     gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo_typeOfPID), "0", "Simple PID");
     gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo_typeOfPID), "1", "Astrom PID");

     gtk_combo_box_set_active_id (GTK_COMBO_BOX(combo_typeOfPID), "0");

// creating plots

     initiate_data_for_plots (modelData);
     initiate_plot (modelData);

// making plot image resizeble



// packing GUI

     //gtk_box_pack_start(GTK_BOX (vboxForPlots), modelData->plotImage, TRUE, TRUE, 5);
     //gtk_container_add (GTK_CONTAINER(aspectFrame), modelData->plotImage);
     gtk_container_add (GTK_CONTAINER(scrolled), modelData->plotImage);

     // grid_input
     gtk_grid_attach (GTK_GRID(grid_input), label_loadSetpoint, 0, 1, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), label_initTorqueLoad, 0, 2, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), label_timeLoad, 0, 3, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), label_speedSetpoint, 0, 4, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), label_initSpeedEng, 0, 5, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), label_timeSpeed, 0, 6, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), label_inputSetpoint, 0, 7, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), label_timeInput, 0, 8, 1, 1);

     gtk_grid_attach (GTK_GRID(grid_input), spin_loadSetpoint, 1, 1, 2, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_initTorqueLoad, 1, 2, 2, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_timeLoadRise, 1, 3, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_timeLoadFall, 2, 3, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_speedSetpoint, 1, 4, 2, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_initSpeedEng, 1, 5, 2, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_timeSpeedRise, 1, 6, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_timeSpeedFall, 2, 6, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_inputSetpoint, 1, 7, 2, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_timeInputRise, 1, 8, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_input), spin_timeInputFall, 2, 8, 1, 1);

     // grid_plant
     gtk_grid_attach (GTK_GRID(grid_plant), label_timeMax, 0, 0, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_plant), label_timeStep, 0, 1, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_plant), label_inertFac, 0, 2, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_plant), label_inputFac, 0, 3, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_plant), label_speedFac, 0, 4, 1, 1);

     gtk_grid_attach (GTK_GRID(grid_plant), spin_timeMax, 1, 0, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_plant), spin_timeStep, 1, 1, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_plant), spin_inertFac, 1, 2, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_plant), spin_inputFac, 1, 3, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_plant), spin_speedFac, 1, 4, 1, 1);

     // grid_controller
     gtk_grid_attach (GTK_GRID(grid_controller), combo_typeOfPID, 0, 0, 2, 1);

     gtk_grid_attach (GTK_GRID(grid_controller), label_P, 0, 1, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), label_I, 0, 2, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), label_D, 0, 3, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), label_iPos, 0, 4, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), label_iNeg, 0, 5, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), label_ctrlClockTime, 0, 6, 1, 1);

     gtk_grid_attach (GTK_GRID(grid_controller), label_pWeight, 0, 7, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), label_bDiff, 0, 8, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), label_aDiff, 0, 9, 1, 1);

     gtk_grid_attach (GTK_GRID(grid_controller), spin_P, 1, 1, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), spin_I, 1, 2, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), spin_D, 1, 3, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), spin_iPos, 1, 4, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), spin_iNeg, 1, 5, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), spin_ctrlClockTime, 1, 6, 1, 1);

     gtk_grid_attach (GTK_GRID(grid_controller), spin_pWeight, 1, 7, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), spin_bDiff, 1, 8, 1, 1);
     gtk_grid_attach (GTK_GRID(grid_controller), spin_aDiff, 1, 9, 1, 1);

     // notebook
     gtk_notebook_append_page (GTK_NOTEBOOK(notebook), grid_input, label_Input);
     gtk_notebook_append_page (GTK_NOTEBOOK(notebook), grid_plant, label_Plant);
     gtk_notebook_append_page (GTK_NOTEBOOK(notebook), grid_controller, label_Controller);

     // other
     gtk_box_pack_start(GTK_BOX (vboxForButtons), notebook, FALSE, FALSE, 5);
     gtk_box_pack_start(GTK_BOX (vboxForButtons), buttonUpdate, FALSE, FALSE, 5);
     gtk_box_pack_start(GTK_BOX (vboxForButtons), buttonSave, FALSE, FALSE, 5);

     gtk_box_pack_start(GTK_BOX (hbox), vboxForButtons, TRUE, TRUE, 5);
     //gtk_box_pack_start(GTK_BOX (hbox), vboxForPlots, TRUE, TRUE, 5);

     gtk_box_pack_start(GTK_BOX (hbox), scrolled, TRUE, TRUE, 5);

     gtk_container_add(GTK_CONTAINER(window), hbox);


// managing signals

     g_signal_connect_swapped (G_OBJECT(window), "destroy", G_CALLBACK(exit_callback), modelData);

     g_signal_connect (G_OBJECT (buttonUpdate), "clicked", G_CALLBACK (update_plot_callback), modelData);
     g_signal_connect (G_OBJECT (buttonSave), "clicked", G_CALLBACK (buttonSave_clicked), modelData);


     g_signal_connect_swapped (G_OBJECT (spin_timeMax), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_timeStep), "value-changed", G_CALLBACK (spin_changed),modelData);
     g_signal_connect_swapped (G_OBJECT (spin_inertFac), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_inputFac), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_speedFac), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_speedSetpoint), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_initSpeedEng), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_P), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_I), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_D), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_iPos), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_iNeg), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_ctrlClockTime), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_pWeight), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_bDiff), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_aDiff), "value-changed", G_CALLBACK (spin_changed), modelData);

     g_signal_connect_swapped (G_OBJECT (spin_timeLoadRise), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_timeLoadFall), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_timeSpeedRise), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_timeSpeedFall), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_timeInputRise), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_timeInputFall), "value-changed", G_CALLBACK (spin_changed), modelData);

     g_signal_connect_swapped (G_OBJECT (spin_initTorqueLoad), "value-changed", G_CALLBACK (spin_changed), modelData);
     g_signal_connect_swapped (G_OBJECT (spin_loadSetpoint), "value-changed", G_CALLBACK (spin_changed), modelData);

     g_signal_connect_swapped (G_OBJECT (spin_inputSetpoint), "value-changed", G_CALLBACK (spin_changed), modelData);

     combo_changed(GTK_COMBO_BOX(combo_typeOfPID), NULL); // run callback to setup activity of interface items for start conditions

     g_signal_connect (G_OBJECT (combo_typeOfPID), "changed", G_CALLBACK (combo_changed), NULL);
     g_signal_connect (window, "check-resize", G_CALLBACK (resize_plot), modelData);
     g_signal_connect (window, "window-state-event", G_CALLBACK (window_state_changed), modelData);

// initiate size of window struct and set size of scrolled

     gtk_window_get_size (GTK_WINDOW (window), &(modelData->oldSize.width), &(modelData->oldSize.height));
     gtk_widget_set_size_request (scrolled, modelData->plotWidth, modelData->plotHeight); //sets size of container, that size will be minimum allowed

// show widgets and run gtk_main

     gtk_widget_show_all(window);

     gtk_main();

     return 0;
}