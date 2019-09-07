#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include <QObject>

class ObjectHandler : public QObject
{
    Q_OBJECT
public:
    explicit ObjectHandler(QObject *parent = nullptr);
    // Set image source displayed on QML element
    void setImageSource(QString src);
    // Return image source displayed on QML element
    QString getImageSource();

signals:

public slots:
private:
    QString imageSource;
};

#endif // OBJECTHANDLER_H
