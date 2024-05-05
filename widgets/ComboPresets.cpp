#include "ComboPresets.h"

void ComboPresets::init(unsigned char presets_json[]) {
    cout << presets_json;
    json_presets = json::parse(presets_json);

    combo_presets = gtk_combo_box_text_new();
    gtk_widget_set_name(combo_presets, "combo_presets");

    for (const auto& preset : json_presets["presets"]) {
        string name = preset["name"];
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_presets), name.c_str());
    }
}

void ComboPresets::pack(GtkWidget *box) {
    gtk_box_pack_start(GTK_BOX(box), combo_presets, TRUE, TRUE, 0);
}

nlohmann::basic_json<> ComboPresets::get_changed() {
    string active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_presets));
    for (const auto& preset : json_presets["presets"]) {
        string name = preset["name"];
        if (name == active_text) {
            return preset;
        }
    }
}