#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <gtk/gtk.h>
#include "dataset.h"

int setup_plot_in_pixbuf(ModelData *);

int initiate_data_for_plots(ModelData *);

int initiate_plot(ModelData *);

int validate_data(ModelData *);

double update_controller_output_simple(PID *, double, double);

double update_controller_output_basic(PID *, double, double);

double update_controller_output_astrom(PID *, double, double);

int calculate_input_signal (ModelData *, int);

int calculate_data(ModelData *);

int delete_plot_data(ModelData *);

int delete_plot(ModelData *);

void update_plot_callback(GtkButton *, ModelData *);

//static void recreate_plot_callback(GtkButton *, ModelData *);

//static void clear_plot_callback(GtkButton *, ModelData *);

//static void delete_plot_callback(GtkButton *, ModelData *);

int save_to_file (char *,  ModelData *);

void buttonSave_clicked (ModelData *);

void spin_changed (ModelData *);

void combo_changed (GtkComboBox *, gpointer *);

void exit_callback (ModelData *);

#endif // FUNCTIONS_H