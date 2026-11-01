#include <QApplication>
#include "WidgetContainer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WidgetContainer container;
    container.show();

    return app.exec();
}