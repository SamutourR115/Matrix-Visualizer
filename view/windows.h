#ifndef __WINDOWS__
#define __WINDOWS__

#include <stdio.h>
#include <gtk/gtk.h>
#include "../model/function.h"

typedef struct appWidgets_t appWidgets;

typedef struct appContext_t appContext;

/**
 * create_window
 * 
 * Create the main windows for the gui
 * 
 * @pre: app != NULL && userData!= NULL
 * @post: /
 * 
 * @param app Main application of the app
 * @param userData All the data about the uti
 */
void create_window(GtkApplication *app, gpointer userData);


#endif
