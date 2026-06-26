#include "windows.h"

struct appWidgets_t{
    GtkWidget *window;   
    GtkWidget *notebook;     
    GtkWidget **grid_matrix;  
    GtkWidget **spin_rows;     
    GtkWidget **spin_cols;
    GtkWidget **dropdown_a; 
    GtkWidget **dropdown_b;    
};

struct appContext_t{
    Matrix **matrix;         
    appWidgets *widgets; 
    unsigned int count;
    unsigned int capacity;
    GtkStringList *matrix_names;
};

/**
 * Declaration of static function
 */

/**
 * init_context
 * 
 * Function to init the context 
 * 
 * @pre : / 
 * @post : / 
 * 
 * @param : / 
 * 
 * @return : a appContext struct
 */
static appContext *init_context(void);

/**
 * init_widgets
 * 
 * Function who wan init all the widgets
 * 
 * @pre: ctx != NULL & app != NULL
 * @post : / 
 * 
 * @param ctx pointers to appContext struct
 * @param app pointers to GtkApplications struct
 * 
 * @return : 0 if memory problem , 1 all is goo
 *      
 */
static int init_widgets(appContext *ctx, GtkApplication *app);

/**
 * add_matrix_tab
 * 
 * Function to adding a matrix dynamically in the struct
 * 
 * @pre : widget != NULL & user_data != NULL
 * @post : Matrix added
 * 
 * @param widget Widget for added the matrix 
 * @param user_data Needed for a proper operation 
 * 
 * @return : / 
 */
static void add_matrix_tab(GtkWidget *widget, gpointer user_data);

/**
 * on_destroy_window
 * 
 * Function for destroying the windows
 * 
 * @pre : widget != NULL & user_data != NULL
 * @post : Windows destroy properly
 * 
 * @param widget Widget for added the matrix 
 * @param user_data Needed for a proper operation  
 * 
 * @return : / 
 */
static void on_destroy_window(GtkWidget *widget, gpointer user_data);

/**
 * on_dimensions_changed
 * 
 * Function to change the dimension of the matrix dynamically
 * 
 * @pre : widget != NULL & user_data != NULL
 * @post : Dimension change
 * 
 * @param widget Widget for added the matrix 
 * @param user_data Needed for a proper operation  
 * `
 * @return : / 
 */
static void on_dimensions_changed(GtkWidget *widget, gpointer user_data);

/**
 * clean_and_rebuild_grid
 * 
 * Function to build and clean the newest page of a matrix
 * 
 * @pre: ctx != NULL & page > 0
 * @post: matrix have more page
 * 
 * @param ctx pointers to a appContext struct
 * @param page page needed
 * 
 * @return /
 */
static void clean_and_rebuild_grid(appContext *ctx, int page);


static void free_context(appContext *ctx);

/**
 * Definition of static function
 */

static appContext *init_context(void){
    appContext *ctx = malloc(sizeof(appContext));
    if (!ctx) return NULL;

    ctx->widgets = malloc(sizeof(appWidgets));
    if (!ctx->widgets){ free(ctx); return NULL; }

    ctx->capacity = 3;
    ctx->count = 0;

    ctx->matrix = malloc(ctx->capacity * sizeof(Matrix*));
    if(!ctx->matrix){ free(ctx->widgets); free(ctx); return NULL; }

    ctx->widgets->grid_matrix = malloc(ctx->capacity * sizeof(GtkWidget*));
    if(!ctx->widgets->grid_matrix){ free(ctx->matrix); free(ctx->widgets); free(ctx); return NULL; }

    ctx->widgets->spin_rows = malloc(ctx->capacity * sizeof(GtkWidget*));
    if(!ctx->widgets->spin_rows){ 
        free(ctx->widgets->grid_matrix); 
        free(ctx->matrix); 
        free(ctx->widgets); 
        free(ctx); 
        return NULL;
    }

    ctx->widgets->spin_cols = malloc(ctx->capacity * sizeof(GtkWidget*));
    if(!ctx->widgets->spin_cols){ 
        free(ctx->widgets->spin_rows);
        free(ctx->widgets->grid_matrix); 
        free(ctx->matrix); 
        free(ctx->widgets); 
        free(ctx); 
        return NULL; 
    }

    ctx->widgets->dropdown_a = malloc(ctx->capacity * sizeof(GtkWidget*));
    if(!ctx->widgets->dropdown_a){ 
        free(ctx->widgets->spin_cols);
        free(ctx->widgets->spin_rows);
        free(ctx->widgets->grid_matrix); 
        free(ctx->matrix); 
        free(ctx->widgets); 
        free(ctx); 
        return NULL; 
    }

    ctx->widgets->dropdown_b = malloc(ctx->capacity * sizeof(GtkWidget*));
    if(!ctx->widgets->dropdown_b){ 
        free(ctx->widgets->dropdown_a);
        free(ctx->widgets->spin_cols);
        free(ctx->widgets->spin_rows);
        free(ctx->widgets->grid_matrix); 
        free(ctx->matrix); 
        free(ctx->widgets); 
        free(ctx); 
        return NULL; 
    }

    return ctx;
}

static int init_widgets(appContext *ctx, GtkApplication *app){
    if(!ctx || !app) return 0;

    ctx->matrix_names = gtk_string_list_new(NULL);

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

    return 1;
}

static void on_destroy_window(GtkWidget *widget, gpointer user_data){
    if (!widget) return;
    free_context((appContext *)user_data);
}

static void on_dimensions_changed(GtkWidget *widget, gpointer user_data){
    if (!widget) return;
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
    if (!widget) return;
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

        GtkWidget **tmp_grid = realloc(ctx->widgets->grid_matrix, ctx->capacity * sizeof(GtkWidget*));
        if(!tmp_grid) return; 
        ctx->widgets->grid_matrix= tmp_grid;

        GtkWidget **tmp_dropdown_a = realloc(ctx->widgets->dropdown_a, ctx->capacity * sizeof(GtkWidget*));
        if(!tmp_dropdown_a) return; 
        ctx->widgets->dropdown_a= tmp_dropdown_a;

        GtkWidget **tmp_dropdown_b = realloc(ctx->widgets->dropdown_b, ctx->capacity * sizeof(GtkWidget*));
        if(!tmp_dropdown_b) return; 
        ctx->widgets->dropdown_b= tmp_dropdown_b;
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

    gtk_string_list_append(ctx->matrix_names, label);

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *spin_r = gtk_spin_button_new_with_range(1, 10, 1);
    GtkWidget *spin_c = gtk_spin_button_new_with_range(1, 10, 1);
    GtkWidget *drop_a = gtk_drop_down_new(G_LIST_MODEL(ctx->matrix_names), NULL);
    GtkWidget *drop_b = gtk_drop_down_new(G_LIST_MODEL(ctx->matrix_names), NULL);
    gtk_box_append(GTK_BOX(right_box), spin_r);
    gtk_box_append(GTK_BOX(right_box), spin_c);
    gtk_box_append(GTK_BOX(tab_box), right_box);
    gtk_box_append(GTK_BOX(right_box), drop_a);
    gtk_box_append(GTK_BOX(right_box), drop_b);


    gtk_notebook_append_page(GTK_NOTEBOOK(ctx->widgets->notebook), tab_box, gtk_label_new(label));

    g_signal_connect(spin_r, "value-changed", G_CALLBACK(on_dimensions_changed), ctx);
    g_signal_connect(spin_c, "value-changed", G_CALLBACK(on_dimensions_changed), ctx);

    ctx->matrix[ctx->count] = new_matrix;

    ctx->widgets->grid_matrix[ctx->count] = grid;

    ctx->widgets->spin_rows[ctx->count] = spin_r;
    ctx->widgets->spin_cols[ctx->count] = spin_c;

    ctx->widgets->dropdown_a[ctx->count] = drop_a;
    ctx->widgets->dropdown_b[ctx->count] = drop_b;

    ctx->count++;
}

static void clean_and_rebuild_grid(appContext *ctx, int page){
    if (!ctx) return;

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

static void free_context(appContext *ctx){
    if (!ctx) return;
    for(unsigned int i = 0; i < ctx->count; i++){
        free_matrix(ctx->matrix[i]);
    }

    if(ctx->widgets){
        free(ctx->widgets->dropdown_a);
        free(ctx->widgets->dropdown_b);
        free(ctx->widgets->grid_matrix);
        free(ctx->widgets->spin_rows);
        free(ctx->widgets->spin_cols);
        free(ctx->widgets);
    }

    free(ctx->matrix);
    free(ctx);
}

/**
 * Definition of function
 */
void create_window(GtkApplication *app, gpointer userData){
    if (!app) return;

    appContext *ctx = init_context();
    if(!ctx) return;

    if(!(init_widgets(ctx,app))){
        free_context(ctx);
        return;
    }

    add_matrix_tab(NULL,ctx);
}