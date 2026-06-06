#include "windows.h"

struct appWidgets_t{
    GtkWidget *window;   
    GtkWidget *notebook;     
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

static void add_matrix_tab(gpointer user_data){
    appContext *ctx = (appContext *)user_data;
    if(!ctx) return;

    Matrix *new_matrix = create_matrix(3, 3);
    if(!new_matrix) return;

    if(ctx->count == ctx->capacity){
        ctx->capacity *= 2;

        Matrix **tmp = realloc(ctx->matrix, ctx->capacity * sizeof(Matrix*));
        if(!tmp) return; 
        ctx->matrix = tmp;

        GtkWidget **tmp_rows = realloc(ctx->widgets->spin_rows, ctx->capacity * sizeof(GtkWidget*));
        if(!tmp_rows) return; 
        ctx->widgets->spin_rows= tmp_rows;

        GtkWidget **tmp_cols = realloc(ctx->widgets->spin_cols, ctx->capacity * sizeof(GtkWidget*));
        if(!tmp_cols) return; 
        ctx->widgets->spin_cols= tmp_cols;

        GtkWidget **tmp_matrix = realloc(ctx->widgets->grid_matrix, ctx->capacity * sizeof(GtkWidget*));
        if(!tmp_matrix) return; 
        ctx->widgets->grid_matrix= tmp_matrix;
    }

    GtkWidget *tab_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *grid = gtk_grid_new();
    gtk_box_append(GTK_BOX(tab_box), grid);

    for(unsigned int i = 0; i < get_cols(new_matrix->rows); i++){
        for(unsigned int j = 0; j < get_rows(new_matrix->cols); j++){
            GtkWidget *entry = ???;
            gtk_grid_attach(GTK_GRID(grid), entry, ???, ???, 1, 1);
        }
}

    // 4. Ajouter ce widget comme nouvel onglet dans le notebook
    // 5. Stocker la Matrix dans ctx->matrix[ctx->count]
    // 6. Stocker les widgets dans ctx->widgets->grid_matrix[ctx->count] etc.
    // 7. Incrémenter ctx->count
}

void create_window(GtkApplication *app, gpointer userData){
    if (app == NULL) return;

    appContext *ctx = malloc(sizeof(appContext));
    if (!ctx) return;

    ctx->widgets = malloc(sizeof(appWidgets));
    if (!ctx->widgets){ free(ctx); return; }

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

    ctx->widgets->window = gtk_application_window_new(app);
    gtk_window_set_title(ctx->widgets->window,"Matrix Visualiser");
    gtk_window_set_default_size(ctx->widgets->window,700,500);
    g_signal_connect(ctx->widgets->window, "destroy",G_CALLBACK(on_destroy_window), ctx);

    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_window_set_child(GTK_WINDOW(ctx->widgets->window), main_box);

    
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(main_box), left_box);

    ctx->widgets->notebook = gtk_notebook_new();
    gtk_box_append(GTK_BOX(main_box), ctx->widgets->notebook);

    gtk_window_present(ctx->widgets->window);
}

void on_dimensions_changed(GtkWidget *widget, gpointer user_data){
    if (widget == NULL) return;
    appContext *ctx = (appContext *) user_data;
    if (!ctx) return;

    unsigned int new_rows = (unsigned int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(ctx->widgets->spin_rows));
    unsigned int new_cols = (unsigned int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(ctx->widgets->spin_cols));

    // Sécurité : Éviter de recréer la matrice si les dimensions n'ont pas changé


    free_matrix(ctx->matrix);

    ctx->matrix = create_matrix(new_rows, new_cols);

    // 5. TODO: Appel de votre fonction pour mettre à jour la grille graphique
    // clean_and_rebuild_grid(ctx);
}