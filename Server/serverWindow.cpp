#include <QtWidgets>
#include <QtNetwork>

#include "serverWindow.h"
#include "ui_serverWindow.h"

ServerWindow::ServerWindow(QWidget *parent) : QMainWindow(parent), nextBlockSize(0), ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    path = "..//phones.csv";

    server = new QLocalServer(this);
    if(!server->listen("PhoneBook"))
    {
        QMessageBox::critical(0, "Server error",
                              "Unable to start server:" + server->errorString());
        server->close();
        return;
    }

    readCSV(path, phones);

    ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] Server has been started");

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

ServerWindow::~ServerWindow()
{
    delete ui;
    delete server;
}

void ServerWindow::slotNewConnection()
{
    ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] Client has been connected");
    QLocalSocket* socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(clientReadSlot()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    sendPhoneBookToClient(socket, "");
}

void ServerWindow::sendPhoneBookToClient(QLocalSocket *clientConnection, QString message)
{
    readCSV(path, phones);
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << quint16(0) << phones << message;

    out.device()->seek(0);
    out << quint16(array.size() - sizeof(quint16));

    clientConnection->write(array);
}

void ServerWindow::clientReadSlot()
{
    readCSV(path, phones);
    QLocalSocket* socket = (QLocalSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_3);
    for(;;)
    {
        if(!nextBlockSize)
        {
            if(socket->bytesAvailable() < (int)sizeof(quint16))
                break;
            in >> nextBlockSize;
        }

        int action_code;
        KeyClass key;
        DataClass data;

        in >> action_code >> key >> data;

        ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] Data has been recieved");

        switch (action_code)
        {
        case 0:
            tryToAdd(key, data, socket);
            break;

        case 1:
            tryToDel(key, socket);
            break;

        case 2:
        {
            QPair <KeyClass, DataClass> originalPair;
            QPair <KeyClass, DataClass> editedPair;
            in >> originalPair;
            tryToEdit(key, data, originalPair, socket);
        }
            break;

        default:
            sendPhoneBookToClient(socket, "Error: unknown action code");
        }
        nextBlockSize = 0;
    }
}

void ServerWindow::tryToAdd(KeyClass& key, DataClass& data, QLocalSocket* socket)
{
    if (phones.contains(key))
    {
        ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] Number adding error:\n\tSuch number has alresdy been added");
        sendPhoneBookToClient(socket, "Такой контакт уже существует!");
        return;
    }

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream( &file );
        stream << key << data;
        file.close();
        ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] Number has been added");
        sendPhoneBookToClient(socket, "Контакт успешно добавлен!");
        return;
    }
    ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] File openning error");
    sendPhoneBookToClient(socket, "Ошибка при записи");
}

void ServerWindow::tryToDel(KeyClass& key, QLocalSocket* socket)
{
    if (!phones.contains(key))
    {
        sendPhoneBookToClient(socket, "Контакт уже был удалён!");
        return;
    }

    QMap<KeyClass, DataClass>::iterator it = phones.find(key);
    phones.erase(it);
    ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] Number has been deleted");
    if (writeCSV(path, phones))
        sendPhoneBookToClient(socket, "Контакт удалён!");
    else
    {
        ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] File openning error");
        sendPhoneBookToClient(socket, "Ошибка при записи");
        return;
    }
}

void ServerWindow::tryToEdit(KeyClass& key, DataClass& data,
               QPair <KeyClass, DataClass> originalPair, QLocalSocket* socket)
{
    if (!phones.contains(originalPair.first))
    {
        sendPhoneBookToClient(socket, "Контакт был удалён!");
        return;
    }

    QMap<KeyClass, DataClass>::iterator it = phones.find(originalPair.first);
    phones.erase(it);

    phones.insert(key, data);
    writeCSV(path, phones);
    ui->statusLog->insertItem(0, '[' + QTime::currentTime().toString()+ "] Number has been editted");
    sendPhoneBookToClient(socket, "");
}
