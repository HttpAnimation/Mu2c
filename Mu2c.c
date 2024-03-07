#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 256
#define MAX_LINE_LENGTH 256
#define MAX_DIRS 10

// Function to read the config file and extract music directories
int read_config(const char *filename, char music_dirs[MAX_DIRS][MAX_PATH_LENGTH], int *num_dirs) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening config file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    *num_dirs = 0;
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character if present
        line[strcspn(line, "\n")] = 0;

        // Only process non-empty lines
        if (line[0] != '\0') {
            // Copy the directory path to the array
            strncpy(music_dirs[*num_dirs], line, MAX_PATH_LENGTH - 1);
            music_dirs[*num_dirs][MAX_PATH_LENGTH - 1] = '\0';
            (*num_dirs)++;

            // Ensure we don't exceed maximum directories
            if (*num_dirs >= MAX_DIRS) {
                fprintf(stderr, "Maximum number of directories reached.\n");
                break;
            }
        }
    }

    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *list;
    GtkListStore *store;
    GtkTreeIter iter;
    char music_dirs[MAX_DIRS][MAX_PATH_LENGTH];
    int num_dirs;

    gtk_init(&argc, &argv);

    // Read config file
    if (read_config("config.conf", music_dirs, &num_dirs) == -1) {
        exit(EXIT_FAILURE);
    }

    // Create a window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Music Client");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a list view
    store = gtk_list_store_new(1, G_TYPE_STRING);
    list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Add music directories to the list view
    for (int i = 0; i < num_dirs; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, music_dirs[i], -1);
    }

    // Add the list view to a scrolled window
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), list);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
