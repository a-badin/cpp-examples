#include "qtwindow.hpp"

#include <QFileDialog>
#include <fstream>
#include "ui_qtwindow.h"

#include "errors.hpp"
#include "common.hpp"

MyWindow::MyWindow(QWidget* parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow) {
    ui_->setupUi(this);

    connect(ui_->pushButton, &QPushButton::clicked, this, &MyWindow::OnOpenFileButtonClicked);
    connect(ui_->lineEdit, &QLineEdit::textChanged, this, &MyWindow::OnSearchEditChanged);
}

void MyWindow::OnOpenFileButtonClicked() {
    QFileDialog* file = new QFileDialog(this);
    QString string = file ->getOpenFileName();
    std::ifstream fstream{string.toStdString()};
    auto data_source = std::make_unique<FileDataSource>(std::move(fstream));
    localizator_ = std::make_unique<ImdbLocalizator>(std::move(data_source));
}

void MyWindow::OnSearchEditChanged(const QString& query) {
    if (!localizator_) {
        return;
    }

    try {
      std::string result = localizator_->findTitleForId(query.toStdString());
      ui_->listWidget->addItem(QString::fromStdString(result));
    } catch (Error& e) {
      ui_->listWidget->addItem(QString::fromStdString(e.what()));
    }
}
