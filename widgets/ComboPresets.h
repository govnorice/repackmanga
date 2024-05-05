#ifndef REPACKMANGA_COMBOPRESETS_H
#define REPACKMANGA_COMBOPRESETS_H

#include <gtk/gtk.h>
#include <string>
#include <boost/filesystem.hpp>
#include <iostream>

#include "../modules/Json.hpp"

using namespace std;
using json = nlohmann::json;

namespace fs = boost::filesystem;

class ComboPresets {
private:
    json json_presets;
public:
    GtkWidget *combo_presets;

    void init(unsigned char presets_json_hex[]);
    void pack(GtkWidget *box);
    nlohmann::basic_json<> get_changed();
};


#endif //REPACKMANGA_COMBOPRESETS_H
