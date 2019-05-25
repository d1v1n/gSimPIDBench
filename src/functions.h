#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <mgl2/mgl_cf.h>
#include <math.h>
#include <stddef.h>
#include <time.h>
#include "dataset.h"

int initiate_data_for_plots ( ModelData* );

int initiate_plot ( ModelData* );

int set_default_plot_decor ( size_t, char* );

int setup_plot_in_pixbuf ( ModelData* );

int validate_data ( ModelData* );

double update_controller_output_simple ( PID*, double, double );

double update_controller_output_astrom ( PID*, double, double );

int calculate_input_signal ( ModelData*, int );

int calculate_data ( ModelData* );

int delete_plot_data ( ModelData* );

int delete_plot ( ModelData* );

void update_plot_callback ( GtkButton*, ModelData* );

int save_to_file ( char*,  ModelData* );

void buttonSave_clicked ( ModelData* );

void buttonLoadTable_clicked ( GtkButton*, size_t );

void use_tables_callback ( GtkToggleButton*, gpointer );

void spin_changed ( ModelData* );

void combo_changed ( GtkComboBox*, gpointer );

int resize_plot ( ModelData* );

gboolean resize_plot_callback ( GtkWidget*, ModelData* );

gboolean window_state_changed ( GtkWidget*, GdkEventWindowState*, ModelData* );

FILE * open_file ( char filename [100] );

LookupTable * prepare_lookup_table ( char* );

int count_words_symbols ( int*, int*, int*, FILE* );

int parse_line ( char*, double* );

double * new_table ( int, int );

int fill_table ( double*, int, int, int, FILE* );

double interpolate ( Node* pNode1, Node* pNode2, Node* pNode3, Node* pNode4, Node* pPoint );

int get_corner_position ( LookupTable*, double, double, int*, int*);

double get_data_from_table ( LookupTable* , double, double );

int free_lookup_table ( size_t );

void exit_callback ( ModelData* );

#endif // FUNCTIONS_H
