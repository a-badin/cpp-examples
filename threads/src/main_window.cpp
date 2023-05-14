#include "main_window.hpp"

#include <chrono>
#include <sstream>
#include <thread>
#include <QWidget>
#include <QVBoxLayout>
#include <QDateTime>

using namespace std::chrono_literals;


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    executor_button_ = new QPushButton("Start with Executor", this);
    layout->addWidget(executor_button_);

    qthread_button_ = new QPushButton("Start with QThread", this);
    layout->addWidget(qthread_button_);

    list_widget_ = new QListWidget(this);
    layout->addWidget(list_widget_);

    connect(executor_button_, &QPushButton::clicked, this, &MainWindow::onExecutorClicked);
    connect(qthread_button_, &QPushButton::clicked, this, &MainWindow::onQThreadClicked);

    thread_.start();
}

void MainWindow::onExecutorClicked() {
    QDateTime now = QDateTime::currentDateTime();
    list_widget_->addItem(now.toString("============== hh:mm:ss =============="));

    executor_.Add([this]{
        std::string result = downloader_.download("http://example.com");
        QMetaObject::invokeMethod(this, [this, result]{
            onDataReady(QString::fromStdString(result));
        });
    });
}

void MainWindow::onQThreadClicked() {
    QDateTime now = QDateTime::currentDateTime();
    list_widget_->addItem(now.toString("============== hh:mm:ss =============="));
    QString target_url = "http://example.com";
    DownloadTask* downloadTask;
    downloadTask = new DownloadTask(target_url, &downloader_);

    // У каждого QObject есть ассоциированный QThread
    downloadTask->moveToThread(&thread_); 

    // Если thread отправителя сигнала и thread получателя не совпадают,
    // вызов слота добавится в оцередь событий получателя
    connect(downloadTask, &DownloadTask::Ready, this, &MainWindow::onDataReady);

    // DownloadTask должен кто-то удалить, можно сделать это через deleteLater
    // deleteLater удалит объект в следующий заход очереди событий
    connect(downloadTask, &DownloadTask::Ready, downloadTask, &QObject::deleteLater);


    // Метод позволяющий вызвать слот без сигнала
    // Мы не можем просто вызывать downloadTask->Start() иначе заблокируемся
    // invokeMethod принимает объект, чтобы знать в очередь какой QThread положить вызов
    QMetaObject::invokeMethod(downloadTask, [downloadTask]() {
        downloadTask->Start();
    });

}

void MainWindow::onDataReady(QString data) {
    data.remove('\r');
    QStringList list = data.split('\n');
    for (QString& str : list) {
        list_widget_->addItem(std::move(str));
    }
}


