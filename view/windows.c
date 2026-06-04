#include "windows.h"

struct appWidgets_t{
    GtkWidget *window;        
    GtkWidget **grid_matrix;  
    GtkWidget **spin_rows;     
    GtkWidget **spin_cols;     
};

struct appContext_t{
    Matrix **matrix;         
    appWidgets *widgets; 
    unsigned int count;
    unsigned int capacity;
};


static void on_destroy_window(GtkWidget *widget, gpointer user_data){
    appContext *ctx = (appContext *)user_data;
    if (ctx) {
        if (ctx->matrix){
            free_matrix(ctx->matrix);
        }
        if (ctx->widgets){
            free(ctx->widgets);
        }
        free(ctx);
    }
}

void create_window(GtkApplication *app, gpointer userData){
    if (app == NULL) return;

    appContext *ctx = malloc(sizeof(appContext));
    if (!ctx) return;

    ctx->widgets = malloc(sizeof(appWidgets));
    if (!ctx->widgets){ free(ctx->widgets); free(ctx); return; }

    ctx->capacity = 3;
    ctx->count = 0;

    ctx->matrix = malloc(ctx->capacity * sizeof(Matrix*));
    if(!ctx->matrix){ free(ctx->widgets); free(ctx); return; }

    ctx->widgets->grid_matrix = malloc(ctx->capacity * sizeof(GtkWidget*));
    if(!ctx->widgets->grid_matrix){ free(ctx->matrix); free(ctx->widgets); free(ctx); return; }

    ctx->widgets->spin_rows = malloc(ctx->capacity * sizeof(GtkWidget*));
    if(!ctx->widgets->spin_rows){ 
        free(ctx->widgets->grid_matrix); 
        free(ctx->matrix); 
        free(ctx->widgets); 
        free(ctx); 
        return; 
    }

    ctx->widgets->spin_cols = malloc(ctx->capacity * sizeof(GtkWidget*));
    if(!ctx->widgets->spin_cols){ 
        free(ctx->widgets->spin_rows);
        free(ctx->widgets->grid_matrix); 
        free(ctx->matrix); 
        free(ctx->widgets); 
        free(ctx); 
        return; 
    }
}

void on_dimensions_changed(GtkWidget *widget, gpointer user_data){
    if (widget == NULL) return;
    appContext *ctx = (appContext *) user_data;
    if (!ctx) return;

    unsigned int new_rows = (unsigned int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(ctx->widgets->spin_rows));
    unsigned int new_cols = (unsigned int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(ctx->widgets->spin_cols));

    // Sécurité : Éviter de recréer la matrice si les dimensions n'ont pas changé
    // (Optionnel, mais plus performant)

    free_matrix(ctx->matrix);

    ctx->matrix = create_matrix(new_rows, new_cols);

    // 5. TODO: Appel de votre fonction pour mettre à jour la grille graphique
    // clean_and_rebuild_grid(ctx);
}