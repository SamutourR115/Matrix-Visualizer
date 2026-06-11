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

/**
 * Declaration of static function
 */
static void add_matrix_tab(GtkWidget *widget, gpointer user_data);

static void on_destroy_window(GtkWidget *widget, gpointer user_data);

static void clean_and_rebuild_grid(appContext *ctx, int page);

/**
 * Definition of static function
 */

static void on_destroy_window(GtkWidget *widget, gpointer user_data){
    if (widget == NULL || user_data == NULL) return;
    appContext *ctx = (appContext *)user_data;
    for(unsigned int i = 0; i < ctx->count; i++){
        free_matrix(ctx->matrix[i]);
    }

    if(ctx->widgets){
        free(ctx->widgets->grid_matrix);
        free(ctx->widgets->spin_rows);
        free(ctx->widgets->spin_cols);
        free(ctx->widgets);
    }

    free(ctx->matrix);
    free(ctx);
}

static void on_dimensions_changed(GtkWidget *widget, gpointer user_data){
    if (widget == NULL || user_data == NULL) return;
    appContext *ctx = (appContext *) user_data;
    if (!ctx) return;

    int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(ctx->widgets->notebook));
    if(page < 0) return;

    unsigned int new_rows = (unsigned int)gtk_spin_button_get_value(
        GTK_SPIN_BUTTON(ctx->widgets->spin_rows[page]));
    unsigned int new_cols = (unsigned int)gtk_spin_button_get_value(
        GTK_SPIN_BUTTON(ctx->widgets->spin_cols[page]));

    if(get_rows(ctx->matrix[page]) == new_rows && get_cols(ctx->matrix[page]) == new_cols) return;

    free_matrix(ctx->matrix[page]);
    ctx->matrix[page] = create_matrix(new_rows, new_cols);

    clean_and_rebuild_grid(ctx, page);
}

static void add_matrix_tab(GtkWidget *widget,gpointer user_data){
    if (widget == NULL || user_data == NULL) return;
    (void)widget;
    appContext *ctx = (appContext *)user_data;
    if(!ctx) return;

    Matrix *new_matrix = create_matrix(1, 1);
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

    for(unsigned int i = 0; i < get_cols(new_matrix); i++){
        for(unsigned int j = 0; j < get_rows(new_matrix); j++){
            GtkWidget *entry = gtk_entry_new();
            gtk_grid_attach(GTK_GRID(grid), entry, i, j, 1, 1);
        }
    }

    char label[8];
    snprintf(label, sizeof(label), "M%u", ctx->count + 1);

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *spin_r = gtk_spin_button_new_with_range(1, 10, 1);
    GtkWidget *spin_c = gtk_spin_button_new_with_range(1, 10, 1);
    gtk_box_append(GTK_BOX(right_box), spin_r);
    gtk_box_append(GTK_BOX(right_box), spin_c);
    gtk_box_append(GTK_BOX(tab_box), right_box);

    gtk_notebook_append_page(GTK_NOTEBOOK(ctx->widgets->notebook), tab_box, gtk_label_new(label));

    g_signal_connect(spin_r, "value-changed", G_CALLBACK(on_dimensions_changed), ctx);
    g_signal_connect(spin_c, "value-changed", G_CALLBACK(on_dimensions_changed), ctx);

    ctx->matrix[ctx->count] = new_matrix;

    ctx->widgets->grid_matrix[ctx->count] = grid;

    ctx->widgets->spin_rows[ctx->count] = spin_r;
    ctx->widgets->spin_cols[ctx->count] = spin_c;

    ctx->count++;
}

static void clean_and_rebuild_grid(appContext *ctx, int page){
    if (ctx == NULL) return;

    GtkWidget *child;
    while((child = gtk_widget_get_first_child(ctx->widgets->grid_matrix[page])) != NULL){
        gtk_grid_remove(GTK_GRID(ctx->widgets->grid_matrix[page]), child);
    }

    for(unsigned int i = 0; i < get_cols(ctx->matrix[page]); i++){
        for(unsigned int j = 0; j < get_rows(ctx->matrix[page]); j++){
            GtkWidget *entry = gtk_entry_new();
            gtk_grid_attach(GTK_GRID(ctx->widgets->grid_matrix[page]), entry, i, j, 1, 1);
        }
    }
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
    gtk_window_set_title(GTK_WINDOW(ctx->widgets->window), "Matrix Visualiser");
    gtk_window_set_default_size(GTK_WINDOW(ctx->widgets->window), 700, 500);
    g_signal_connect(ctx->widgets->window, "destroy",G_CALLBACK(on_destroy_window), ctx);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_window_set_child(GTK_WINDOW(ctx->widgets->window), main_box);
    
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(main_box), left_box);

    GtkWidget *btn_add = gtk_button_new_with_label("+");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(add_matrix_tab), ctx);
    gtk_box_append(GTK_BOX(left_box), btn_add);

    ctx->widgets->notebook = gtk_notebook_new();
    gtk_box_append(GTK_BOX(main_box), ctx->widgets->notebook);

    gtk_window_present(GTK_WINDOW(ctx->widgets->window));
    add_matrix_tab(NULL,ctx);

}