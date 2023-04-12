#pragma once

#include <QObject>
#include <QtDebug>

class CustomObject : public QObject {
    Q_OBJECT

public:
  CustomObject(QObject *parent = nullptr) : QObject(parent) {}

  void emitCustomSignal() {
      emit custom_signal();
  }

signals:
  void custom_signal();

public slots:

  void custom_slot() {
      qDebug() << "123";
  }

};
