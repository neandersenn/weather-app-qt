#include "weatherapp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

WeatherApp::WeatherApp(QWidget *parent) : QWidget(parent){
    this->setStyleSheet("background-color:#f8f8f8;");
    manager = new QNetworkAccessManager(this);
    setupUI();
    fetchCitySuggestion();
    fetchWeather();

}

void WeatherApp::setupUI(){

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    title = new QLabel("Weather app", this);
    title->setStyleSheet("font-weight:600;font-size:28px;");
    title->setAlignment(Qt::AlignCenter);

    cityEdit = new QLineEdit(this);
    cityEdit->setPlaceholderText("Введите город");
    cityEdit->setStyleSheet( "background-color: #ffffff;"
                              "color: #1e293b;"
                              "border-radius: 10px;"
                              "padding: 18px 22px;"
                              "font-size: 20px;");

    cityEdit->setFixedWidth(500);

    searchBtn = new QPushButton("Search", this);
    searchBtn->setCursor(Qt::PointingHandCursor);
    searchBtn->setStyleSheet("QPushButton{"
                             "background-color: #3b82f6;"
                             "color: #1e293b;"
                             "font-weight:600;"
                             "border-radius: 10px;"
                             "padding: 14px 14px;"
                             "font-size: 18px;"
                             "}"
                             "QPushButton:hover{"
                             "background-color: #2563eb;"
                             "}");

    searchBtn->setFixedSize(160, 64);

    QHBoxLayout *enterLayout = new QHBoxLayout();
    enterLayout->addWidget(cityEdit);
    enterLayout->addWidget(searchBtn);
    enterLayout->setAlignment(Qt::AlignCenter);

    cityLabel = new QLabel("Город, Страна");
    cityLabel->setStyleSheet("font-weight:600;font-size:32px;");
    cityLabel->setAlignment(Qt::AlignCenter);

    iconWeather = new QLabel("☀️", this);
    iconWeather->setStyleSheet("font-size: 64px;");
    iconWeather->setAlignment(Qt::AlignCenter);

    tempLabel = new QLabel("25°C", this);
    tempLabel->setStyleSheet("font-weight:bold;font-size:48px;");
    tempLabel->setAlignment(Qt::AlignCenter);

    descriptionLabel = new QLabel("Ясно", this);
    descriptionLabel->setStyleSheet("font-weight:600;font-size:26px;color: #475569;");
    descriptionLabel->setAlignment(Qt::AlignCenter);

    feelsLabel = new QLabel("Ощущается как 24°C", this);
    humidityLabel = new QLabel("Влажность 60%", this);
    windLabel = new QLabel("Ветер 3 м/с", this);
    feelsLabel->setStyleSheet("background-color: #e2e8f0; padding: 8px 16px; border-radius: 8px; font-size:18px; font-weight:600;");
    humidityLabel->setStyleSheet("background-color: #e2e8f0; padding: 8px 16px; border-radius: 8px; font-size:18px; font-weight:600;");
    windLabel->setStyleSheet("background-color: #e2e8f0; padding: 8px 16px; border-radius: 8px; font-size:18px; font-weight:600;");

    QHBoxLayout *bottomInfoLayout = new QHBoxLayout();
    bottomInfoLayout->addWidget(feelsLabel);
    bottomInfoLayout->addWidget(humidityLabel);
    bottomInfoLayout->addWidget(windLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->addWidget(iconWeather);
    infoLayout->addWidget(tempLabel);
    infoLayout->addWidget(descriptionLabel);
    infoLayout->addLayout(bottomInfoLayout);
    infoLayout->setAlignment(Qt::AlignCenter);

    refresh = new QPushButton("Refresh weather", this);
    refresh->setFixedSize(180, 45);
    refresh->setCursor(Qt::PointingHandCursor);
    refresh->setStyleSheet("QPushButton{"
                           "background-color: #e2e8f0;"
                           "color: #1e293b;"
                           "font-weight:600;"
                           "border-radius: 10px;"
                           "padding: 14px 18px;"
                           "font-size: 18px;"
                           "}"
                           "QPushButton:hover{"
                           "background-color: #cbd5e1;"
                           "}");

    statusLabel = new QLabel("Погода загружена", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 18px; color: #94a3b8; padding-top: 8px; font-weight:600;");

    connect(searchBtn, &QPushButton::clicked, this, &WeatherApp::fetchWeather);
    connect(refresh, &QPushButton::clicked, this, &WeatherApp::fetchWeather);
    connect(cityEdit, &QLineEdit::returnPressed, this, &WeatherApp::fetchWeather);

    mainLayout->addWidget(title);
    mainLayout->addLayout(enterLayout);
    mainLayout->addWidget(cityLabel);
    mainLayout->addLayout(infoLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(refresh);
    mainLayout->addWidget(statusLabel);
}

void WeatherApp::fetchWeather(){

    QString city = cityEdit->text().trimmed();
    if(city.isEmpty()){
        statusLabel->setText("Введите название города");
        return;
    }

    QString apikey = "b8906c4b987a983bfc1d552dc4e13367";
    QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=ru")
            .arg(city)
            .arg(apikey);

    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=](){

        if(reply->error() != QNetworkReply::NoError){
            statusLabel->setText("город не найден или нет интернета");
            reply->deleteLater();
            return;
        }
        QByteArray arr = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(arr);
        QJsonObject obj = doc.object();

        if(!obj.contains("main")){
            statusLabel->setText("Ошибка: неверный ответ от сервера");
            reply->deleteLater();
            return;
        }

        QString cityName = obj["name"].toString();
        QString country = obj["sys"].toObject()["country"].toString();
        cityLabel->setText(cityName + ", " + country);

        QJsonObject mainObj = obj["main"].toObject();
        double temp = mainObj["temp"].toDouble();
        double feels = mainObj["feels_like"].toDouble();
        int humidity = mainObj["humidity"].toInt();

        double speedWind = obj["wind"].toObject()["speed"].toDouble();

        QJsonArray weatherArray = obj["weather"].toArray();
        if(!weatherArray.isEmpty()){
            QString description = weatherArray[0].toObject()["description"].toString();
            descriptionLabel->setText(description);
            QString icon = obj["weather"].toArray()[0].toObject()["icon"].toString();
            QString iconUrl = "http://openweathermap.org/img/wn/" + icon + "@4x.png";
            QNetworkRequest iconRequest(iconUrl);
            QNetworkReply *iconReply = manager->get(iconRequest);

                QObject::connect(iconReply, &QNetworkReply::finished, [=](){

                    if(iconReply->error() == QNetworkReply::NoError){
                    QByteArray iconDoc = iconReply->readAll();
                    QPixmap pixmap;
                    pixmap.loadFromData(iconDoc);
                    iconWeather->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    iconReply->deleteLater();
                    }

                });
        }

        tempLabel->setText(QString::number((int)temp) + "°C");
        feelsLabel->setText("Ощущается как " + QString::number((int)feels) + "°C");
        humidityLabel->setText("Влажность " + QString::number(humidity) + "%");
        windLabel->setText("Ветер " + QString::number(speedWind) + " м/с");

        statusLabel->setText("Погода обновлена");
        reply->deleteLater();

    });

}

void WeatherApp::fetchCitySuggestion(){}


