#include <gtkmm.h>
#include <memory>
#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "custom_object.h"
#include "qtwindow.hpp"
#include "gtkwindow.hpp"

static int run_gtk(int argc, char* argv[]) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "ru.example.gtk");
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("etc/main.glade");
    MainWindow* window_raw_ptr = nullptr;
    builder->get_widget_derived("main_window", window_raw_ptr);
    if (!window_raw_ptr) {
        std::cerr << "Cannot create window from glade\n";
        return -1;
    }
    std::unique_ptr<MainWindow> window_uptr{window_raw_ptr};
    app->add_window(*window_uptr);
    return app->run(*window_uptr);
}

static int run_qt(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MyWindow m;
    m.show();
    return app.exec();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "First argument must be QT or GTK\n";
        return -1;
    }
    std::vector arg_copy(argv + 2, argv + argc);
    arg_copy.insert(arg_copy.begin(), argv[0]);
    if (std::string_view(argv[1]) == "GTK") {
        std::cerr << "Run gtk\n";
        return run_gtk(arg_copy.size(), arg_copy.data());
    }
    if (std::string_view(argv[1]) == "QT") {
        std::cerr << "Run qt\n";
        return run_qt(arg_copy.size(), arg_copy.data());
    }
    std::cerr << "First argument must be QT or GTK\n";
    return -1;
}
//

