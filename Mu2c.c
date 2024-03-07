#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 256
#define MAX_FILES 1000

// Function to list mp3 files in a directory
int list_mp3_files(const char *dir, char mp3_files[MAX_FILES][MAX_PATH_LENGTH], int *num_files) {
    DIR *dp;
    struct dirent *ep;
    char filename[MAX_PATH_LENGTH];

    dp = opendir(dir);
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            if (strstr(ep->d_name, ".mp3") != NULL) {
                snprintf(filename, MAX_PATH_LENGTH, "%s/%s", dir, ep->d_name);
                strncpy(mp3_files[*num_files], filename, MAX_PATH_LENGTH - 1);
                mp3_files[*num_files][MAX_PATH_LENGTH - 1] = '\0';
                (*num_files)++;
            }
        }
        (void)closedir(dp);
        return 0;
    } else {
        perror("Error opening directory");
        return -1;
    }
}

// Callback function for handling double-click events
void on_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data) {
    GtkTreeModel *model;
    GtkTreeIter   iter;

    // Get the model associated with the tree view
    model = gtk_tree_view_get_model(tree_view);

    // Get the selected row
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gchar *filename;
        gtk_tree_model_get(model, &iter, 0, &filename, -1);

        // Print the filename (In actual implementation, you would play the file here)
        g_print("Playing: %s\n", filename);

        g_free(filename);
    }
}

// Callback function for handling play button click events
void on_play_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Play button clicked\n");
    // Add code to play the selected MP3 file
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *list;
    GtkListStore *store;
    GtkTreeSelection *selection;
    GtkWidget *play_button;
    char music_dir[] = "music";
    char mp3_files[MAX_FILES][MAX_PATH_LENGTH];
    int num_files = 0;

    gtk_init(&argc, &argv);

    // Create a window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Music Client");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create a list view
    store = gtk_list_store_new(1, G_TYPE_STRING);
    list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Add music files to the list view
    if (list_mp3_files(music_dir, mp3_files, &num_files) != -1) {
        GtkTreeIter iter;
        for (int i = 0; i < num_files; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, mp3_files[i], -1);
        }
    }

    // Create a tree view column
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Files", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    // Connect signal for handling double-click events
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
    g_signal_connect(selection, "changed", G_CALLBACK(on_row_activated), NULL);

    // Add the list view to the grid layout
    gtk_grid_attach(GTK_GRID(grid), list, 0, 0, 1, 1);

    // Create a play button
    play_button = gtk_button_new_with_label("Play");
    g_signal_connect(play_button, "clicked", G_CALLBACK(on_play_button_clicked), NULL);

    // Add the play button to the grid layout
    gtk_grid_attach(GTK_GRID(grid), play_button, 0, 1, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
