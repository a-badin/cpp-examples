#pragma once

#include <QMainWindow>
#include <memory>
#include "imdb_localizator.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MyWindow : public QMainWindow
{
    Q_OBJECT
public:
    MyWindow(QWidget* parent = nullptr);


public slots:
    void OnOpenFileButtonClicked();
    void OnSearchEditChanged(const QString& query);

private:
    Ui::MainWindow* ui_;
    std::unique_ptr<ImdbLocalizator> localizator_;
};
