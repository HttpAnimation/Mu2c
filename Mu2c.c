#include <gtk/gtk.h>
#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>

// Define the directory where the music files are stored
#define MUSIC_DIRECTORY "music"

// Function to play or pause the selected music file
gboolean playing = FALSE; // Flag to track whether music is currently playing

void play_or_pause_music(const gchar *file_path, GtkWidget *play_button) {
    gchar *command;

    if (!playing) {
        command = g_strdup_printf("gst-launch-1.0 playbin uri=file://%s", file_path);
        gtk_button_set_label(GTK_BUTTON(play_button), "Pause");
        playing = TRUE;
    } else {
        command = g_strdup("gst-launch-1.0 playbin uri=NULL");
        gtk_button_set_label(GTK_BUTTON(play_button), "Play");
        playing = FALSE;
    }

    system(command);

    g_free(command);
}


// Callback function for the play/pause button
void play_button_clicked(GtkButton *button, gpointer data) {
    GtkTreeView *treeview = GTK_TREE_VIEW(data);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *filename;
    gchar *file_path;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &filename, -1);
        gchar *file_name_with_path = g_build_filename(MUSIC_DIRECTORY, filename, NULL);
        GFile *file = g_file_new_for_path(file_name_with_path);
        gchar *file_uri = g_file_get_uri(file);
        play_or_pause_music(file_uri, GTK_WIDGET(button));
        g_free(filename);
        g_free(file_name_with_path);
        g_free(file_uri);
        g_object_unref(file);
    }
}


// Function to populate the list store with music files
void populate_music_list(GtkListStore *store) {
    GDir *dir;
    const gchar *filename;

    dir = g_dir_open(MUSIC_DIRECTORY, 0, NULL);
    if (dir == NULL) {
        g_print("Could not open music directory\n");
        return;
    }

    while ((filename = g_dir_read_name(dir)) != NULL) {
        // Check if the file is an MP3 or FLAC file
        if (g_str_has_suffix(filename, ".mp3") || g_str_has_suffix(filename, ".flac")) {
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, filename, -1);
        }
    }

    g_dir_close(dir);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;
    GtkWidget *play_button;
    GtkWidget *button_box;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Music Player");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    store = gtk_list_store_new(1, G_TYPE_STRING);
    populate_music_list(store);

    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Music Files", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    play_button = gtk_button_new_with_label("Play");
    g_signal_connect(play_button, "clicked", G_CALLBACK(play_button_clicked), treeview);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_CENTER);
    gtk_container_add(GTK_CONTAINER(button_box), play_button);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), treeview, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
