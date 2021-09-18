#include "phonesList.h"
#include "ui_phonesList.h"

#include <QErrorMessage>

PhonesList::PhonesList(QWidget *parent) : QMainWindow(parent), nextBlockSize(0), ui(new Ui::PhonesList)
{
    ui->setupUi(this);

    socket = new QLocalSocket(this);
    addEditWindow = nullptr;
    showWindow = nullptr;

    connect(socket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error), this, &::PhonesList::slotError);
    connect(socket, SIGNAL(readyRead()), SLOT(recievePhoneBook()));

    socket->connectToServer("PhoneBook");
}

PhonesList::~PhonesList()
{
    delete ui;
    delete socket;
}

void PhonesList::recievePhoneBook()
{
    QDataStream in(socket);
    QString message = "";
    ui->phonesList->clear();
    in.setVersion(QDataStream::Qt_5_3);
    for(;;)
    {
        if(!nextBlockSize)
        {
            if(socket->bytesAvailable() < (int)sizeof(quint16))
                break;
        }
        in >> nextBlockSize;

        if(socket->bytesAvailable() < nextBlockSize)
            break;

        in >> phones >> message;

        QMapIterator<KeyClass, DataClass> it(phones);
        while (it.hasNext())
        {
            it.next();
            QString numberStr = it.key().Number;
            ui->phonesList->addItem(it.key().FirstName + " " + it.key().MiddleName + " " + it.key().LastName + "\n      "
                                    + numberStr.insert(1,'(').insert(5, ')').insert(9, '-').insert(12, '-'));
        }

        nextBlockSize = 0;
    }
    if (message != "")
    {
        QErrorMessage* msg = new QErrorMessage(this);
        msg->setStyleSheet("background-color: rgb(33, 57, 84);color: rgb(255, 255, 255);");
        msg->showMessage(message);
    }
}

void PhonesList::slotError(QLocalSocket::LocalSocketError error)
{
    QString strError =
        "Error: " + (error == QLocalSocket::ServerNotFoundError ?
                     "Сервер не нейден" :
                     error == QLocalSocket::PeerClosedError ?
                     "Сервер закрыт" :
                     error == QLocalSocket::ConnectionRefusedError ?
                     "Отказано в соединении" :
                     QString(socket->errorString()));
    {
        QErrorMessage* msg = new QErrorMessage(this);
        msg->setStyleSheet("background-color: rgb(33, 57, 84);color: rgb(255, 255, 255);");
        msg->showMessage(strError);
    }
}

void PhonesList::on_searchBar_textChanged(const QString &txt)
{
    ui->phonesList->clear();

    if (!ui->searchBar->text().isEmpty())
    {
        if (ui->searchBar->validator() == nullptr)
        {
            bool isDigit = false;
            txt.toInt(&isDigit, 10);
            if (isDigit)
            {
                QRegExp numbersRX("[1-9]\\d{0,10}");
                QValidator *numbersValidatior = new QRegExpValidator(numbersRX, this);
                ui->searchBar->setValidator(numbersValidatior);
            }
            else
            {
                QRegExp charactersRX(QString("[а-яА-Яa-zA-Z ]+"));
                QValidator *charactersValidator = new QRegExpValidator(charactersRX, this);
                ui->searchBar->setValidator(charactersValidator);
                if (!ui->searchBar->hasAcceptableInput())
                {
                    ui->searchBar->clear();
                    ui->searchBar->setValidator(nullptr);
                }
            }
        }
        if (ui->searchBar->validator() != nullptr)
        {
            QMapIterator<KeyClass, DataClass> it(phones);
            while (it.hasNext())
            {
                it.next();
                QString numberStr = it.key().Number;
                numberStr.insert(1,'(').insert(5, ')').insert(9, '-').insert(12, '-');
                if (it.key().Number.contains(txt) || it.key().Number.contains(txt) || it.key().MiddleName.contains(txt) || it.key().LastName.contains(txt))
                    ui->phonesList->addItem(it.key().FirstName + " " + it.key().MiddleName + " " + it.key().LastName + "\n      " + numberStr);
            }
        }
    }
    else
    {
        ui->searchBar->clear();
        ui->searchBar->setValidator(nullptr);
        QMapIterator<KeyClass, DataClass> it(phones);
        while (it.hasNext())
        {
            it.next();
            QString numberStr = it.key().Number;
            ui->phonesList->addItem(it.key().FirstName + " " + it.key().MiddleName + " " + it.key().LastName + "\n      "
                                    + numberStr.insert(1,'(').insert(5, ')').insert(9, '-').insert(12, '-'));
        }
    }
}

void PhonesList::on_phonesList_itemClicked(QListWidgetItem *item)
{
    QString fullText = item->text();

    QStringRef numStringRef(&fullText, fullText.indexOf(QRegExp("[0-9]")), 15);
    QString numStr = numStringRef.toString().remove(12, 1).remove(9, 1).remove(5, 1).remove(1, 1);

    QStringRef firstNameStr(&fullText, 0, fullText.indexOf(QRegExp(" ")));
    int firstNameEnd = firstNameStr.length() + 1;
    QStringRef middleNameStr(&fullText,firstNameEnd, fullText.indexOf(QRegExp(" "),firstNameEnd) - firstNameEnd);
    int middleNameEnd = middleNameStr.length() + firstNameEnd + 1;
    QStringRef lastNameStr(&fullText, middleNameEnd, fullText.indexOf(QRegExp("\n")) - middleNameEnd);


    QPair <KeyClass, DataClass> pair;

    QMapIterator<KeyClass, DataClass> it(phones);
    while (it.hasNext())
    {
        it.next();
        if (numStr == it.key().Number && firstNameStr.toString() == it.key().FirstName
                && middleNameStr.toString() == it.key().MiddleName && lastNameStr.toString() == it.key().LastName)
        {
            pair.first = it.key();
            pair.second = it.value();
            break;
        }
    }

    if (showWindow == nullptr)
    {
        showWindow = new ShowWindow(pair, socket);
        showWindow->show();
    }
    else
    {
        delete showWindow;
        showWindow = new ShowWindow(pair, socket);
        showWindow->show();
    }
}

void PhonesList::on_addNumberButton_clicked()
{
    if (addEditWindow == nullptr)
    {
        addEditWindow = new AddEditWindow(socket, this);
        addEditWindow->show();
    }
    else
    {
        delete addEditWindow;
        addEditWindow = new AddEditWindow(socket, this);
        addEditWindow->show();
    }
}
