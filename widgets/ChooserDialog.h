#ifndef REPACKMANGA_CHOOSERDIALOG_H
#define REPACKMANGA_CHOOSERDIALOG_H

#include <gtk/gtk.h>
#include <string>
#include <stdexcept>

using namespace std;

class ChooserDialog {
private:
    GtkWidget *dialog, *window;
    gint res;
public:
    void init(GtkFileChooserAction action, string title, string cancel_name, string accept_name);
    string run();
};


#endif