#pragma once

#include <cassert>
#include <thread>
#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QThread>
#include "thread_worker.hpp"
#include "downloader.hpp"


class DownloadTask : public QObject {
    Q_OBJECT
public:
    DownloadTask(QString url, Downloader* downloader) {
        assert(downloader != nullptr);
        url_ = url;
        downloader_ = downloader;
    }

    ~DownloadTask() {
        qDebug() << "DownloadTask die";
    }

public slots:
    void Start() {
        std::string result = downloader_->download(url_);
        emit Ready(QString::fromStdString(result));
    }

signals:
    void Ready(QString data);

private:
    QString url_;
    Downloader* downloader_;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

    ~MainWindow() {
        thread_.quit();
        thread_.wait();
        executor_.Stop();
    }

public slots:

    void onExecutorClicked();
    void onQThreadClicked();
    void onDataReady(QString data);

private:
    QPushButton* executor_button_;
    QPushButton* qthread_button_;
    QListWidget* list_widget_;
    QThread thread_;
    Executor executor_;
    Downloader downloader_;
};

