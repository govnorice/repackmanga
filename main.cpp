#include <iostream>
#include <string>
#include <gtk/gtk.h>
#include <unistd.h>

// Modules
#include "modules/CssManager.h"
#include "modules/Repack.h"

// Widgets
#include "widgets/ChooserDialog.h"

ChooserDialog chooserDialog;

GtkWidget *label_path;

using namespace std;

void chooser_clicked () {
    chooserDialog.init(GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Select directory", "Cancel", "Select");
    try {
        string path = chooserDialog.run();
        gtk_label_set_text(GTK_LABEL(label_path), path.c_str());
        cout << path.c_str() << endl;
        return;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return;
    }
}

void repack_clicked () {
    string path = gtk_label_get_text(GTK_LABEL(label_path));
    if (path == "The path is not chosen" || path.empty()) {
        return;
    }
    Repack::start(path);
}

void gui (int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "RepackManga");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 150);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(container, "container");
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(box1, "box1");
    GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(box2, "box2");

    label_path = gtk_label_new("The path is not chosen");
    gtk_widget_set_name(label_path, "label_path");

    GtkWidget *button_select_path = gtk_button_new_with_label("Select a path");
    gtk_widget_set_name(button_select_path, "button_select_path");

    GtkWidget *button_repack = gtk_button_new_with_label("Repack");
    gtk_widget_set_name(button_repack, "button_repack");


    gtk_container_add(GTK_CONTAINER(window), container);
    gtk_box_pack_start(GTK_BOX(container), box1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), box2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box1), label_path, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box1), button_select_path, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button_repack, TRUE, TRUE, 0);

    g_signal_connect(button_select_path, "clicked", G_CALLBACK(chooser_clicked), NULL);
    g_signal_connect(button_repack, "clicked", G_CALLBACK(repack_clicked), NULL);

    //CSS
    CssManager css;

    string path_css = "style.css";
//    string path = string(homeDir) + "/.config/powerdialog/style.css";

//    css.loadFromHex(powerdialog_style_css, powerdialog_style_css_len);
    css.loadFromFile(path_css.c_str());

    gtk_widget_show_all(window);
    gtk_main();
}

int main (int argc, char *argv[]) {
    if (argc >= 3) {
        string option1 = argv[1];
        string option2 = argv[2];

        if (option1 == "--path") {
            Repack::start(option2);
        }
    } else if (argc == 1) {
        gui(argc, argv);
    } else {
        cerr << "Error: Insufficient arguments" << endl;
        return 1;
    }
    return 0;
}