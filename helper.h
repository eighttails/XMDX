#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QFileInfo>

class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = 0);
    Q_INVOKABLE QString fileNameFromPath(const QString & filePath) const {
        return QFileInfo(filePath).fileName();
    }
signals:

public slots:

};

#endif // HELPER_H
