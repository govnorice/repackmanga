#include <iostream>
#include <string>
#include <gtk/gtk.h>
#include <unistd.h>

// Modules
#include "modules/CssManager.h"
#include "modules/Repack.h"
#include "modules/Json.hpp"

// Hex
#include "style.hex"
#include "presets.hex"

// Widgets
#include "widgets/ChooserDialog.h"
#include "widgets/ComboPresets.h"

const string homeDir = getenv("HOME");

Repack repacker;
ChooserDialog chooserDialog;
ComboPresets comboPresets;

GtkWidget *entry_path;

using namespace std;
using json = nlohmann::json;

void chooser_clicked () {
    chooserDialog.init(GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Select directory", "Cancel", "Select");
    try {
        string path = chooserDialog.run();
        gtk_entry_set_text(GTK_ENTRY(entry_path), path.c_str());
        cout << path.c_str() << endl;
        return;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return;
    }
}

void repack_clicked () {
    json changed_json = comboPresets.get_changed();
    string path = gtk_entry_get_text(GTK_ENTRY(entry_path));
    if (path.empty()) {
        return;
    }
    repacker.start(path, changed_json["pattern"]["volume"], changed_json["pattern"]["chapter"]);
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
    GtkWidget *box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(box3, "box3");

    entry_path = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_path), "Enter path");
    gtk_widget_set_name(entry_path, "entry_path");

    GtkWidget *button_select_path = gtk_button_new_with_label("Select a path");
    gtk_widget_set_name(button_select_path, "button_select_path");

    comboPresets.init(presets_json);

    GtkWidget *button_repack = gtk_button_new_with_label("Repack");
    gtk_widget_set_name(button_repack, "button_repack");


    gtk_container_add(GTK_CONTAINER(window), container);
    gtk_box_pack_start(GTK_BOX(container), box1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), box2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), box3, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box1), entry_path, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(box1), button_select_path, FALSE, FALSE, 0);
    comboPresets.pack(box2);
    gtk_box_pack_start(GTK_BOX(box3), button_repack, TRUE, TRUE, 0);

    g_signal_connect(button_select_path, "clicked", G_CALLBACK(chooser_clicked), NULL);
    g_signal_connect(button_repack, "clicked", G_CALLBACK(repack_clicked), NULL);

    //CSS
    CssManager css;

    string path = homeDir + "/.config/powerdialog/style.css";

    css.loadFromHex(style_css, style_css_len);
    css.loadFromFile(path.c_str());

    gtk_widget_show_all(window);
    gtk_main();
}

int main(int argc, char *argv[]) {
    if (argc >= 5) {
        string option1 = argv[1];
        string option2 = argv[2];
        string option3 = argv[3];
        string option4 = argv[4];

        if (option1 == "--path") {
            if (option2.empty()) {
                cerr << "Error: The path is empty" << endl;
                return 2;
            }
            if (option3 == "--preset") {
                if (option4.empty()) {
                    cerr << "Error: The preset is empty" << endl;
                    return 3;
                }
                json json_presets = json::parse(presets_json);
                for (const auto& preset : json_presets["presets"]) {
                    if (!preset.is_object()) {
                        cerr << "Error: Invalid JSON format" << endl;
                        return 4;
                    }
                    string name = preset["name"];
                    if (name == option4) {
                        repacker.start(option2, preset["pattern"]["volume"], preset["pattern"]["chapter"]);
                        break;
                    }
                }
            }
        }
    } else if (argc == 1) {
        gui(argc, argv);
    } else {
        cerr << "Error: Insufficient arguments" << endl;
        return 1;
    }
    return 0;
}