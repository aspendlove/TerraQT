//
// Created by aspendlove on 3/31/24.
//

#include <future>
#include <QList>
#include "NetFace.h"
#include "DataTypes.h"
#include <QDebug>

using std::byte;

NetFace::NetFace(int port) : QObject(nullptr) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // binding socket.
    bind(serverSocket, (struct sockaddr *) &serverAddress,
         sizeof(serverAddress));
    std::thread listenThread(&NetFace::listenForConnections, this);
    listenThread.detach();
}

[[noreturn]] void NetFace::listenForConnections() {
    while (true) {
        listen(serverSocket, 5);
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        std::thread acceptThread(&NetFace::handleConnection, this, clientSocket);
        acceptThread.detach();
    }
}

NetFace::~NetFace() {
    close(serverSocket);
}

[[noreturn]] void NetFace::handleConnection(int clientSocket) {
    while (true) {
        QString message = recvAll(clientSocket);
        qDebug() << "received message " + message.toStdString();
        QStringList lines = message.split("\r\n");
        QVector<ShapeTime> mouthShapes{};
        for (const QString &line: lines) {
            QStringList splitLine = line.split(' ');
            if (splitLine.size() != 2) continue;
            QString timeLine = splitLine[0];
            int time = timeLine.toInt();
            QString shapeLine = splitLine[1];
            MouthShape shape = MouthShape::A; // A is the default and matches A or X
            if (shapeLine == "B") {
                shape = MouthShape::B;
            } else if (shapeLine == "C") {
                shape = MouthShape::C;
            } else if (shapeLine == "D") {
                shape = MouthShape::D;
            } else if (shapeLine == "E") {
                shape = MouthShape::E;
            } else if (shapeLine == "F") {
                shape = MouthShape::F;
            }

            mouthShapes.append(ShapeTime({time, shape}));
        }
        emit dataReceived(mouthShapes);
        char byte = 1; // signifies success to the sender
        send(clientSocket, &byte, sizeof(byte), 0);
    }
}

QString NetFace::recvAll(int clientSocket) {
    QString message = "";
    bool finished = false;
    while (!finished) {
        char buffer[1024] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);
        message.append(buffer);
        if(message.contains("\r\n\r\n")) {
            QStringList split = message.split("\r\n\r\n");
            message = split[0];
            finished = true;
        }
    }
    return message;
}
