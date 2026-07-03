#ifndef WEATHERAPP_H
#define WEATHERAPP_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QNetworkAccessManager>

class WeatherApp : public QWidget {
    Q_OBJECT
public:
    WeatherApp(QWidget *parent = nullptr);

private:
    void setupUI();
    void fetchWeather();
    void fetchCitySuggestion();

    QLabel *title;
    QLabel *cityLabel;
    QLabel *iconWeather;
    QLabel *tempLabel;
    QLabel *descriptionLabel;
    QLabel *feelsLabel;
    QLabel *humidityLabel;
    QLabel *windLabel;
    QPushButton *refresh;
    QLabel *statusLabel;
    QLineEdit *cityEdit;
    QPushButton *searchBtn;

    QNetworkAccessManager *manager;
};

#endif // WEATHERAPP_H
