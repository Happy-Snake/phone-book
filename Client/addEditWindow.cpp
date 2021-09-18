#include "addEditWindow.h"
#include "ui_addEditWindow.h"

#include <QErrorMessage>

AddEditWindow::AddEditWindow(QLocalSocket *parentSocket, QWidget *parent,
                           QPair <KeyClass, DataClass>* pairEditted) : nextBlockSize(0), ui(new Ui::AddEditWindow)
{
    ui->setupUi(this);
    socket = parentSocket;
    editPair = pairEditted;
    parentWidget = parent;


    if(parentWidget->objectName() == "PhonesList")
    {
        ui->title->setText("Добавление контакта");
        ui->birthDateEdit->setDate(QDate::currentDate());
    }
    else
    {
        ui->title->setText("Изменение контакта");

        ui->firstNameLine->setText(editPair->first.FirstName);
        ui->middleNameLine->setText(editPair->first.MiddleName);
        ui->lastNameLine->setText(editPair->first.LastName);
        ui->cityLine->setText(editPair->second.getCity());
        ui->streetLine->setText(editPair->second.getStreet());
        ui->numberLine->setText(editPair->first.Number);
        ui->houseLine->setText(editPair->second.getHouse());
        ui->flatLine->setText(editPair->second.getFlat());
        ui->birthDateEdit->setDate(QDate::fromString(editPair->second.getBirthDate(), "dd.MM.yyyy"));
        ui->emailLine->setText(editPair->second.getEmail());
        ui->workLine->setText(editPair->second.getCompany());
        ui->pushButton->setText("Готово");
    }

    QRegExp lettersRX("[а-яА-Яa-zA-Z -]+");
    QValidator *lettersValidatior = new QRegExpValidator(lettersRX, this);
    ui->firstNameLine->setValidator(lettersValidatior);
    ui->middleNameLine->setValidator(lettersValidatior);
    ui->lastNameLine->setValidator(lettersValidatior);
    ui->cityLine->setValidator(lettersValidatior);
    ui->streetLine->setValidator(lettersValidatior);

    QRegExp numbersRX("[1-9]\\d{0,10}");
    QValidator *numbersValidatior = new QRegExpValidator(numbersRX, this);
    ui->numberLine->setValidator(numbersValidatior);

    QRegExp houseAndFlatRX("[1-9]\\d{0,3}");
    QValidator *houseAndFlatValidatior = new QRegExpValidator(houseAndFlatRX, this);
    ui->houseLine->setValidator(houseAndFlatValidatior);
    ui->flatLine->setValidator(houseAndFlatValidatior);

    QRegExp emailRX("[а-яА-Яa-zA-Z1-9/=?^_`{|}~.@!#$%&'*+-]+");
    QValidator *emailValidatior = new QRegExpValidator(emailRX, this);
    ui->emailLine->setValidator(emailValidatior);
    ui->workLine->setValidator(emailValidatior);
}

AddEditWindow::~AddEditWindow()
{
    delete ui;
}

void AddEditWindow::on_pushButton_clicked()
{
    if ((ui->firstNameLine->text().isEmpty() && ui->middleNameLine->text().isEmpty() && ui->lastNameLine->text().isEmpty())
            || ui->numberLine->text().isEmpty())
    {
        QErrorMessage* msg = new QErrorMessage(this);
        msg->setStyleSheet("background-color: rgb(33, 57, 84);color: rgb(255, 255, 255);");
        msg->showMessage("Имя и номер должны быть заполнены!");
    }
    else
        if (ui->numberLine->text().length() != 11)
        {
            QErrorMessage* msg = new QErrorMessage(this);
            msg->setStyleSheet("background-color: rgb(33, 57, 84);color: rgb(255, 255, 255);");
            msg->showMessage("Номер введён некорректно!");
        }
        else
        {
            QStringList elems;
            elems.append(ui->numberLine->text());
            elems.append(ui->firstNameLine->text());
            elems.append(ui->middleNameLine->text());
            elems.append(ui->lastNameLine->text());
            KeyClass* key = new KeyClass(elems);

            elems.append(ui->emailLine->text());
            elems.append(ui->cityLine->text());
            elems.append(ui->streetLine->text());
            elems.append(ui->houseLine->text());
            elems.append(ui->flatLine->text());

            if (ui->birthDateEdit->date() != QDate::currentDate())
                elems.append(ui->birthDateEdit->date().toString("dd.MM.yyyy"));
            else
               elems.append("");

            elems.append(ui->workLine->text());

            DataClass* data = new DataClass(elems);

            QByteArray array;
            QDataStream out(&array, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_3);

            if (parentWidget->objectName() == "PhonesList")
            {
                int status = 0;
                out << quint16(0) << status << *key << *data;
            }
            else
            {

                int status = 2;
                out << quint16(0) << status << *key << *data << editPair->first << editPair->second;
            }
            out.device()->seek(0);
            out << quint16(array.size() - sizeof(quint16));
            socket->write(array);
            if (parentWidget->objectName() == "ShowWindow")
            {
                QPair<KeyClass, DataClass> newPair;
                newPair.first = *key;
                newPair.second = *data;
                ShowWindow* showWindow = new ShowWindow(newPair, socket);
                showWindow->show();
            }
            this->close();
        }
}
