#include "MyWidget.h"
#include <QApplication>
#include <QFont>
MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(400, 300);
    this->setWindowTitle("Qt Hello World");

    helloLabel = new QLabel("Hello World", this);

    QFont font = helloLabel->font();
    font.setPointSize(20);
    
    helloLabel->setFont(font);
    helloLabel->setGeometry(100, 70, 200, 45);

    exitButton = new QPushButton("Выход", this);
    exitButton->setGeometry(100, 160, 150, 40);

    connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
}