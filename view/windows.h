#ifndef __WINDOWS__
#define __WINDOWS__

#include <stdio.h>
#include <gtk/gtk.h>
#include "../model/function.h"

typedef struct appWidgets_t appWidgets;

typedef struct appContext_t appContext;

void create_window(GtkApplication *app, gpointer userData);

void quit_cb(GtkWindow *window);

#endif
