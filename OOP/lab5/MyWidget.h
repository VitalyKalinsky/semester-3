#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class MyWidget : public QWidget
{
    Q_OBJECT
    QLabel *helloLabel;
    QPushButton *exitButton;
public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget() {}
};