/* gtkmm example Copyright (C) 2013 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <gtkmm.h>
#include <gtkmm/application.h>
#include <memory>
#include <iostream>
#include <thread>
#include <QApplication>

#include "errors.hpp"
#include "imdb_localizator.hpp"
#include "qtwindow.hpp"

class Worker {

};

class FileDataSource : public IDataSource {
public:
    explicit FileDataSource(std::ifstream&& f)
        : f_{std::move(f)} {}

    std::optional<std::string> getNextLine() override {
        std::string tmp;
        if (std::getline(f_, tmp)) {
            return tmp;
        }
        return std::nullopt;
    }
private:
    std::ifstream f_;
};

class Window : public Gtk::ApplicationWindow {
public:
    Window(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
        : Gtk::ApplicationWindow(cobject) {

        refBuilder->get_widget("SearchEntry", search_entry_);
        refBuilder->get_widget("FileChooserButton", file_chooser_button_);
        refBuilder->get_widget("ListBox", list_box_);

        search_entry_->signal_search_changed().connect(sigc::mem_fun(*this, &Window::on_search_clicked));
        file_chooser_button_->signal_file_set().connect(sigc::mem_fun(*this, &Window::on_file_opened));

        //dispatcher_.connect(sigc::mem_fun(*this, &Window::on_progress));
    }


    void on_progress() {
        std::cout << "Progress" << std::endl;
    }

    void on_search_clicked() {
        std::string result;
        try {
            result = localizator_->findTitleForId(search_entry_->get_text());
            Gtk::ListBoxRow* row = Gtk::make_managed<Gtk::ListBoxRow>();
            Gtk::Label* label = Gtk::make_managed<Gtk::Label>();
            label->set_text(result);
            row->add(*label);
            row->show_all();
            list_box_->append(*row);
        } catch (Error& e) {
            set_title("Not found");
        }
    }

    void on_file_opened() {
        for (Gtk::Widget* widget : list_box_->get_children()) {
            list_box_->remove(*widget);
        }
        set_title(file_chooser_button_->get_uri());
        auto file = file_chooser_button_->get_file();
        auto fileSource = std::make_unique<FileDataSource>(std::ifstream(file->get_path()));
        localizator_ = std::make_unique<ImdbLocalizator>(std::move(fileSource));
        set_title(file->get_path());
    }

private:
    std::thread t;
    Glib::Dispatcher dispatcher_;
    Gtk::ListBox* list_box_;
    Gtk::SearchEntry* search_entry_;
    Gtk::FileChooserButton* file_chooser_button_;
    std::unique_ptr<ImdbLocalizator> localizator_;
    size_t i{0};
};


int main (int argc, char* argv[])
{
  //auto app = Gtk::Application::create(argc, argv, "org.park.example");

  //auto builder = Gtk::Builder::create_from_file("/home/abadin/main.glade");
  //Window *window;
  //builder->get_widget_derived("MainWindow", window);

  //app->add_window(*window);
  //return app->run(*window);
  //
  QApplication app(argc, argv);
  MyWindow w;
  w.show();
  
  return app.exec();


}
