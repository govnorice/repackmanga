#include "ChooserDialog.h"

void ChooserDialog::init(GtkFileChooserAction action, string title, string cancel_name, string accept_name) {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), title.c_str());

    dialog = gtk_file_chooser_dialog_new(title.c_str(),
                                         GTK_WINDOW(window),
                                         action,
                                         cancel_name.c_str(),
                                         GTK_RESPONSE_CANCEL,
                                         accept_name.c_str(),
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);
}
string ChooserDialog::run() {
    string path;
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        path = gtk_file_chooser_get_filename(chooser);
    } else if (res == GTK_RESPONSE_CANCEL) {
        gtk_widget_destroy(dialog);
        throw runtime_error("The user has closed the path selection window");
    } else {
        gtk_widget_destroy(dialog);
        throw runtime_error("[!] Unexpected error");
    }

    gtk_widget_destroy(dialog);

    return path;
}