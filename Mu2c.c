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

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *list;
    GtkListStore *store;
    GtkTreeIter iter;
    char music_dir[] = "music";
    char mp3_files[MAX_FILES][MAX_PATH_LENGTH];
    int num_files = 0;

    gtk_init(&argc, &argv);

    // Create a window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Music Client");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a list view
    store = gtk_list_store_new(1, G_TYPE_STRING);
    list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Add music files to the list view
    if (list_mp3_files(music_dir, mp3_files, &num_files) != -1) {
        for (int i = 0; i < num_files; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, mp3_files[i], -1);
        }
    }

    // Create a tree view column
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Files", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    // Add the list view to a scrolled window
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), list);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
