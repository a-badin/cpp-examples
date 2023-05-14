#include <QApplication>
#include "main_window.hpp"

using namespace std::chrono_literals;


void Log(const std::string& str) {
    static std::mutex m;
    std::lock_guard _(m);
    std::cerr << str << std::endl;
}


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}

