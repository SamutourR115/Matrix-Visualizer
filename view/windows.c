#include "windows.h"

struct appWidgets_t{
    GtkWidget *window;        
    GtkWidget *grid_matrix;  
    GtkWidget *spin_rows;     
    GtkWidget *spin_cols;     
};

struct appContext_t{
    Matrix *matrix_a;         
    appWidgets *widgets; 
};


static void on_destroy_window(GtkWidget *widget, gpointer user_data){
    appContext *ctx = (appContext *)user_data;
    if (ctx) {
        if (ctx->matrix_a){
            free_matrix(ctx->matrix_a);
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
    if (!ctx->widgets){
        free(ctx);
        return;
    }

    ctx->matrix_a = create_matrix(3, 3);

    ctx->widgets->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(ctx->widgets->window), "MatrixCalculator");
    gtk_window_set_default_size(GTK_WINDOW(ctx->widgets->window), 700, 500);

    g_signal_connect(ctx->widgets->window, "destroy", G_CALLBACK(on_destroy_window), ctx);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_window_set_child(GTK_WINDOW(ctx->widgets->window), main_box);

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(main_box), left_box);
    
    // TODO: Ajouter ici vos GtkSpinButton pour les lignes/colonnes
    // et les stocker dans ctx->widgets->spin_rows et spin_cols

    ctx->widgets->grid_matrix = gtk_grid_new();
    gtk_box_append(GTK_BOX(main_box), ctx->widgets->grid_matrix);

    gtk_window_present(GTK_WINDOW(ctx->widgets->window));
}

void on_dimensions_changed(GtkWidget *widget, gpointer user_data){
    if (widget == NULL) return;
    appContext *ctx = (appContext *) user_data;
    if (!ctx) return;

    unsigned int new_rows = (unsigned int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(ctx->widgets->spin_rows));
    unsigned int new_cols = (unsigned int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(ctx->widgets->spin_cols));

    // Sécurité : Éviter de recréer la matrice si les dimensions n'ont pas changé
    // (Optionnel, mais plus performant)

    free_matrix(ctx->matrix_a);

    ctx->matrix_a = create_matrix(new_rows, new_cols);

    // 5. TODO: Appel de votre fonction pour mettre à jour la grille graphique
    // clean_and_rebuild_grid(ctx);
}