#ifndef __WINDOWS__
#define __WINDOWS__

#include <stdio.h>
#include <gtk/gtk.h>
#include "../model/function.h"

typedef struct appWidgets_t appWidgets;

typedef struct appContext_t appContext;

static void add_matrix_tab(gpointer user_data);

static void on_destroy_window(GtkWidget *widget, gpointer user_data);

void create_window(GtkApplication *app, gpointer userData);


#endif
