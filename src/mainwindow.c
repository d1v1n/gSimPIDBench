#include "mainwindow.h"
#include "dataset.h"
#include "functions.h"

int create_main_window (ModelData *modelData, int *argc, char **argv[]) {

// creating window

     gtk_init(argc, argv);

     initiate_data_for_plots (modelData);
     initiate_plot (modelData);

     if ( !modelData->consoleMode ) {

          window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

          gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
          gtk_window_set_title(GTK_WINDOW(window), "Test View");
          gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
          gtk_container_set_border_width(GTK_CONTAINER(window), 2);

          // creating layout elements

          vboxForPlots_1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
          vboxForPlots_2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);

          scrolledSpeed = gtk_scrolled_window_new (NULL, NULL);
          aspectFrameSpeed = gtk_aspect_frame_new ("Speed plot",    // label
                                             0.5,     // center x
                                             0.5,     // center y
                                             1.5,  // xsize/ysize
                                             FALSE ); // ignore child's aspect

          scrolledLoad = gtk_scrolled_window_new (NULL, NULL);
          aspectFrameLoad = gtk_aspect_frame_new ("Load plot",    // label
                                             0.5,     // center x
                                             0.5,     // center y
                                             1.5,  // xsize/ysize
                                             FALSE ); // ignore child's aspect

          scrolledSet = gtk_scrolled_window_new (NULL, NULL);
          aspectFrameSet = gtk_aspect_frame_new ("Setpoint plot",    // label
                                             0.5,     // center x
                                             0.5,     // center y
                                             1.5,  // xsize/ysize
                                             FALSE ); // ignore child's aspect

          scrolledInput = gtk_scrolled_window_new (NULL, NULL);
          aspectFrameInput = gtk_aspect_frame_new ("Input plot",    // label
                                             0.5,     // center x
                                             0.5,     // center y
                                             1.5,  // xsize/ysize
                                             FALSE ); // ignore child's aspect

          vboxForButtons = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
          hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
          notebook = gtk_notebook_new();
          grid_input = gtk_grid_new ();
          gtk_grid_set_column_homogeneous (GTK_GRID(grid_input), TRUE);
          grid_plant = gtk_grid_new ();
          gtk_grid_set_column_homogeneous (GTK_GRID(grid_plant), TRUE);
          grid_controller = gtk_grid_new ();
          gtk_grid_set_column_homogeneous (GTK_GRID(grid_controller), TRUE);

          //grid_plots = gtk_grid_new ();
          //gtk_grid_set_column_homogeneous (GTK_GRID(grid_plots),TRUE);

          // creating buttons

          buttonUpdate = gtk_button_new_with_label ("Update plot");
          buttonSave = gtk_button_new_with_label ("Save data");
          buttonTrqTable = gtk_button_new_with_label ("Default");
          buttonRailTable = gtk_button_new_with_label ("Default");
          // creating check buttons

          checkUseTables = gtk_check_button_new_with_label ( "Use tables for torque calculation" );
          gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(checkUseTables), TRUE);

          // creating labels

          label_Input = gtk_label_new ("Input");
          label_loadSetpoint = gtk_label_new ("Load setpoint, N*m");
          gtk_widget_set_halign (GTK_WIDGET(label_loadSetpoint), GTK_ALIGN_START);
          label_initTorqueLoad = gtk_label_new ("Initial load torque, N*m");
          gtk_widget_set_halign (GTK_WIDGET(label_initTorqueLoad), GTK_ALIGN_START);
          label_timeLoad = gtk_label_new ("Load Rise/Fall time, s");
          gtk_widget_set_halign (GTK_WIDGET(label_timeLoad), GTK_ALIGN_START);
          label_speedSetpoint = gtk_label_new ("Engine setpoint, s-1");
          gtk_widget_set_halign (GTK_WIDGET(label_speedSetpoint), GTK_ALIGN_START);
          label_initSpeedEng = gtk_label_new ("Initial engine speed, s-1");
          gtk_widget_set_halign (GTK_WIDGET(label_initSpeedEng), GTK_ALIGN_START);
          label_timeSpeed = gtk_label_new ("Setpoint Rise/Fall time, s");
          gtk_widget_set_halign (GTK_WIDGET(label_timeSpeed), GTK_ALIGN_START);
          label_inputSetpoint = gtk_label_new ("Manual input, n/a");
          gtk_widget_set_halign (GTK_WIDGET(label_inputSetpoint), GTK_ALIGN_START);
          label_timeInput = gtk_label_new ("Setpoint Rise/Fall time, s");
          gtk_widget_set_halign (GTK_WIDGET(label_timeInput), GTK_ALIGN_START);

          label_Plant = gtk_label_new ("Plant");
          label_timeMax = gtk_label_new ("Max simulation time, c");
          gtk_widget_set_halign (GTK_WIDGET(label_timeMax), GTK_ALIGN_START);
          label_timeStep = gtk_label_new ("Time step, c");
          gtk_widget_set_halign (GTK_WIDGET(label_timeStep), GTK_ALIGN_START);
          label_inertFac = gtk_label_new ("Inertion of object, kg*m2");
          gtk_widget_set_halign (GTK_WIDGET(label_inertFac), GTK_ALIGN_START);
          label_inputFac = gtk_label_new ("Input factor, N*m");
          gtk_widget_set_halign (GTK_WIDGET(label_inputFac), GTK_ALIGN_START);
          label_speedFac = gtk_label_new ("Speed factor, N*m*s");
          gtk_widget_set_halign (GTK_WIDGET(label_speedFac), GTK_ALIGN_START);
          label_inputLag = gtk_label_new ("Input Lag, samples");
          gtk_widget_set_halign (GTK_WIDGET(label_inputLag), GTK_ALIGN_START);
          label_trqTable = gtk_label_new ("Torque table");
          gtk_widget_set_halign (GTK_WIDGET(label_trqTable), GTK_ALIGN_START);
          label_railTable = gtk_label_new ("Rail table");
          gtk_widget_set_halign (GTK_WIDGET(label_railTable), GTK_ALIGN_START);


          label_Controller = gtk_label_new ("Controller");
          label_P = gtk_label_new ("P = ");
          gtk_widget_set_halign (GTK_WIDGET(label_P), GTK_ALIGN_START);
          label_I = gtk_label_new ("I = ");
          gtk_widget_set_halign (GTK_WIDGET(label_I), GTK_ALIGN_START);
          label_D = gtk_label_new ("D = ");
          gtk_widget_set_halign (GTK_WIDGET(label_D), GTK_ALIGN_START);
          label_iPos = gtk_label_new ("I positive limit = ");
          gtk_widget_set_halign (GTK_WIDGET(label_iPos), GTK_ALIGN_START);
          label_iNeg = gtk_label_new ("I negative limit = ");
          gtk_widget_set_halign (GTK_WIDGET(label_iNeg), GTK_ALIGN_START);
          label_ctrlClockTime = gtk_label_new ("Controller clock period, s");
          gtk_widget_set_halign (GTK_WIDGET(label_ctrlClockTime), GTK_ALIGN_START);

          label_pWeight = gtk_label_new ("Setpoint weitgh for P = ");
          gtk_widget_set_halign (GTK_WIDGET(label_pWeight), GTK_ALIGN_START);
          label_bDiff = gtk_label_new ("b factor for D = ");
          gtk_widget_set_halign (GTK_WIDGET(label_bDiff), GTK_ALIGN_START);
          label_aDiff = gtk_label_new ("a factor for D = ");
          gtk_widget_set_halign (GTK_WIDGET(label_aDiff), GTK_ALIGN_START);


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

          spin_inputSetpoint = gtk_spin_button_new_with_range(lim_inputSetpoint_B, lim_inputSetpoint_T, 0.001);
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

          spin_inputLag = gtk_spin_button_new_with_range(lim_inputLag_B, lim_inputLag_T, 1);
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_inputLag), modelData->inputLag);

          // creating spin tab_Controller

          spin_P = gtk_spin_button_new_with_range(lim_pGain_B, lim_pGain_T, 0.0001);
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_P), modelData->ctrl.pGain);

          spin_I = gtk_spin_button_new_with_range(lim_iGain_B, lim_iGain_T, 0.00001);
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

          // packing GUI

          gtk_container_add (GTK_CONTAINER(scrolledSpeed), plotImageSpeed);
          gtk_container_add (GTK_CONTAINER(aspectFrameSpeed), scrolledSpeed);
          gtk_box_pack_start(GTK_BOX (vboxForPlots_1), aspectFrameSpeed, TRUE, TRUE, 5);

          gtk_container_add (GTK_CONTAINER(scrolledLoad), plotImageLoad);
          gtk_container_add (GTK_CONTAINER(aspectFrameLoad), scrolledLoad);
          gtk_box_pack_start(GTK_BOX (vboxForPlots_1), aspectFrameLoad, TRUE, TRUE, 5);

          gtk_container_add (GTK_CONTAINER(scrolledSet), plotImageSet);
          gtk_container_add (GTK_CONTAINER(aspectFrameSet), scrolledSet);
          gtk_box_pack_start(GTK_BOX (vboxForPlots_2), aspectFrameSet, TRUE, TRUE, 5);

          gtk_container_add (GTK_CONTAINER(scrolledInput), plotImageInput);
          gtk_container_add (GTK_CONTAINER(aspectFrameInput), scrolledInput);
          gtk_box_pack_start(GTK_BOX (vboxForPlots_2), aspectFrameInput, TRUE, TRUE, 5);

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
          gtk_grid_attach (GTK_GRID(grid_plant), label_inputLag, 0, 5, 1, 1);

          gtk_grid_attach (GTK_GRID(grid_plant), checkUseTables, 1, 6, 1, 1);

          gtk_grid_attach (GTK_GRID(grid_plant), label_trqTable, 0, 7, 1, 1);
          gtk_grid_attach (GTK_GRID(grid_plant), label_railTable, 0, 8, 1, 1);

          gtk_grid_attach (GTK_GRID(grid_plant), spin_timeMax, 1, 0, 1, 1);
          gtk_grid_attach (GTK_GRID(grid_plant), spin_timeStep, 1, 1, 1, 1);
          gtk_grid_attach (GTK_GRID(grid_plant), spin_inertFac, 1, 2, 1, 1);
          gtk_grid_attach (GTK_GRID(grid_plant), spin_inputFac, 1, 3, 1, 1);
          gtk_grid_attach (GTK_GRID(grid_plant), spin_speedFac, 1, 4, 1, 1);
          gtk_grid_attach (GTK_GRID(grid_plant), spin_inputLag, 1, 5, 1, 1);
          gtk_grid_attach (GTK_GRID(grid_plant), buttonTrqTable, 1, 7, 1, 1);
          gtk_grid_attach (GTK_GRID(grid_plant), buttonRailTable, 1, 8, 1, 1);

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

          gtk_box_pack_start(GTK_BOX (hbox), vboxForPlots_1, TRUE, TRUE, 5);
          gtk_box_pack_start(GTK_BOX (hbox), vboxForPlots_2, TRUE, TRUE, 5);
          //gtk_box_pack_start(GTK_BOX (hbox), grid_plots, TRUE, TRUE, 5);
          //gtk_box_pack_start(GTK_BOX (hbox), aspectFrameSpeed, TRUE, TRUE, 5);

          gtk_container_add(GTK_CONTAINER(window), hbox);


          // managing signals

          g_signal_connect_swapped (G_OBJECT(window), "destroy", G_CALLBACK(exit_callback), modelData);

          g_signal_connect (G_OBJECT (buttonUpdate), "clicked", G_CALLBACK (update_plot_callback), modelData);
          g_signal_connect_swapped (G_OBJECT (buttonSave), "clicked", G_CALLBACK (buttonSave_clicked), modelData);

          g_signal_connect (G_OBJECT (checkUseTables), "toggled", G_CALLBACK (use_tables_callback), NULL);
          g_signal_connect (G_OBJECT (buttonTrqTable), "clicked", G_CALLBACK (buttonLoadTable_clicked), (gpointer) modelData->pTrqTable);
          g_signal_connect (G_OBJECT (buttonRailTable), "clicked", G_CALLBACK (buttonLoadTable_clicked), (gpointer) modelData->pRailTable);


          g_signal_connect_swapped (G_OBJECT (spin_timeMax), "value-changed", G_CALLBACK (spin_changed), modelData);
          g_signal_connect_swapped (G_OBJECT (spin_timeStep), "value-changed", G_CALLBACK (spin_changed),modelData);
          g_signal_connect_swapped (G_OBJECT (spin_inertFac), "value-changed", G_CALLBACK (spin_changed), modelData);
          g_signal_connect_swapped (G_OBJECT (spin_inputFac), "value-changed", G_CALLBACK (spin_changed), modelData);
          g_signal_connect_swapped (G_OBJECT (spin_speedFac), "value-changed", G_CALLBACK (spin_changed), modelData);
          g_signal_connect_swapped (G_OBJECT (spin_inputLag), "value-changed", G_CALLBACK (spin_changed), modelData);
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
          g_signal_connect (window, "check-resize", G_CALLBACK (resize_plot_callback), modelData);
          g_signal_connect (window, "window-state-event", G_CALLBACK (window_state_changed), modelData);

          // initiate size of window struct and set size of scrolled

          gtk_window_get_size (GTK_WINDOW (window), &(modelData->oldSize.width), &(modelData->oldSize.height));

          gtk_widget_set_size_request (scrolledSpeed,300, 200); //sets size of container, that size will be minimum allowed
          gtk_widget_set_size_request (scrolledLoad, 300, 200);
          gtk_widget_set_size_request (scrolledSet,  300, 200);
          gtk_widget_set_size_request (scrolledInput,300, 200);

          // show widgets and run gtk_main

          gtk_widget_show_all(window);
          gtk_main();


     } else {

          save_to_file ( modelData->outfile, modelData);
          free_lookup_table ( modelData->pTrqTable );
          free_lookup_table ( modelData->pRailTable );
          delete_plot(modelData);
          g_printf( "Complete!\n" );

     }



     return 0;
}
