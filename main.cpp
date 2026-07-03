#include "weatherapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    WeatherApp window;
    window.show();
    window.resize(800, 800);
    return app.exec();
}
