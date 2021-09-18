#include "showWindow.h"
#include "ui_showWindow.h"

ShowWindow::ShowWindow(QPair <KeyClass, DataClass> parentPair, QLocalSocket *parentSocket, QWidget *parent) : QMainWindow(parent), ui(new Ui::ShowWindow)
{
    ui->setupUi(this);
    pair = parentPair;
    socket = parentSocket;

    if (pair.first.FirstName != "" && pair.first.MiddleName != "" && pair.first.LastName != "")
        ui->nameLine->setText(pair.first.LastName + '\n' + pair.first.FirstName + ' ' + pair.first.MiddleName);
    else
        ui->nameLine->setText(pair.first.FirstName + ' ' + pair.first.MiddleName +' '+ pair.first.LastName);

    QString numStr = pair.first.Number;
    ui->numberLine->setText(numStr.insert(1,'(').insert(5,')').insert(9,'-').insert(12,'-'));

    if (pair.second.getEmail() != "")
        ui->emailLine->setText(pair.second.getEmail());

    if (pair.second.getCity() != "" || pair.second.getStreet() != "" || pair.second.getHouse() != "" || pair.second.getFlat() != "")
    {
        ui->adressLine->setText("");
        if (pair.second.getCity() != "")
            ui->adressLine->setText("г." + pair.second.getCity() + ", ");
        if (pair.second.getStreet() != "")
            ui->adressLine->setText(ui->adressLine->text() + "ул." + pair.second.getStreet() + ", ");
        if (pair.second.getHouse() != "")
            ui->adressLine->setText(ui->adressLine->text() + "д." + pair.second.getHouse() + ", ");
        if (pair.second.getFlat() != "")
            ui->adressLine->setText(ui->adressLine->text() + "кв." +pair.second.getFlat());
    }

    if (pair.second.getBirthDate() != "")
        ui->birthDateLine->setText(pair.second.getBirthDate());

    if (pair.second.getCompany() != "")
        ui->workLine->setText(pair.second.getCompany());
}

ShowWindow::~ShowWindow()
{
    delete ui;
}

void ShowWindow::on_deleteButton_pressed()
{
    QMessageBox msgBox;
    msgBox.setText("Вы уверены?");
    msgBox.setStandardButtons(QMessageBox::StandardButton::Yes| QMessageBox::StandardButton::No);
    msgBox.setStyleSheet("background-color: rgb(33, 57, 84);color: rgb(255, 255, 255); font-size: 20px;");
    int reply = msgBox.exec();
    if (reply == QMessageBox::StandardButton::Yes)
    {
          QByteArray array;
          QDataStream out(&array, QIODevice::WriteOnly);
          out.setVersion(QDataStream::Qt_5_3);

          int status = 1;
          out << quint16(0) << status << pair.first << pair.second;
          out.device()->seek(0);
          out << quint16(array.size() - sizeof(quint16));
          socket->write(array);
          this->close();
    }
}

void ShowWindow::on_editButton_clicked()
{
    AddEditWindow* addEditWindow = new AddEditWindow(socket, this, &pair);
    addEditWindow->show();
    this->close();
}
