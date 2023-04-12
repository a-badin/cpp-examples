#include "gtkwindow.hpp"

#include "common.hpp"

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject) {
    refBuilder->get_widget("list_box", list_box_);
    refBuilder->get_widget("button", button_);
    refBuilder->get_widget("search_entry", search_entry_);
    
    button_->signal_file_set().connect(sigc::mem_fun(*this, &MainWindow::on_file_selected));
    search_entry_->signal_search_changed().connect(sigc::mem_fun(*this, &MainWindow::on_search_changed));
}

void MainWindow::on_file_selected() {
    std::string path = button_->get_file()->get_path();
    std::ifstream file{path};
    auto fileSource = std::make_unique<FileDataSource>(std::move(file));
    localizator_ = std::make_unique<ImdbLocalizator>(std::move(fileSource));
}

void MainWindow::on_search_changed() {
    std::string result;
    try {
        result = localizator_->findTitleForId(search_entry_->get_text());
        Gtk::ListBoxRow* row = Gtk::make_managed<Gtk::ListBoxRow>();
        row->add(*Gtk::make_managed<Gtk::Label>(result));
        row->show_all();
        list_box_->append(*row);
    } catch (Error& e) {
        set_title("Not found");
    }
}
