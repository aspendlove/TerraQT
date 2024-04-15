//
// Created by aspendlove on 3/31/24.
//

#ifndef TERRAQT_NETFACE_H
#define TERRAQT_NETFACE_H


#include <qstring.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <QObject>
#include "DataTypes.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

using std::cout;
using std::endl;

class NetFace : public QObject {
Q_OBJECT

public:
    NetFace(int);

    ~NetFace();

signals:

    void dataReceived(const QVector<ShapeTime> &);

private:
    [[noreturn]] void handleConnection(int);

    static QString recvAll(int);

    [[noreturn]] void listenForConnections();

    int serverSocket = 0;
};


#endif //TERRAQT_NETFACE_H
