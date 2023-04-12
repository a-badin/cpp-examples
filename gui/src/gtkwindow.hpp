#pragma once
#include <gtkmm.h>

#include "imdb_localizator.hpp"

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

    void on_file_selected();
    void on_search_changed();

private:
    Gtk::FileChooserButton* button_;
    Gtk::ListBox* list_box_;
    Gtk::SearchEntry* search_entry_;
    std::unique_ptr<ImdbLocalizator> localizator_;
};

