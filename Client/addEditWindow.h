#ifndef ADDEDITWINDOW_H
#define ADDEDITWINDOW_H

#include <QMainWindow>
#include <QLocalSocket>
#include "class.h"
#include "showWindow.h"

/*!
 * \file
 * \brief Заголовочный файл, содержащий класс окна добавления/редактирования контакта
 *
 * Окно с полями для заполнения/изменения информации о контакте. В зависимости от класса, в котором было созданно
 * это окно, оно будет выполнять функцию добавления/редактирвания.
 */


namespace Ui {
class AddEditWindow;
}

/*!
 * \brief Окно добавления/редактирования контакта
 *
 * Окно с полями для заполнения/изменения информации о контакте. В зависимости от класса, в котором было созданно
 * это окно, оно будет выполнять функцию добавления/редактирвания.
 */

class AddEditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddEditWindow(QLocalSocket *parentSocket, QWidget *parent = nullptr, QPair <KeyClass, DataClass>* editPair = nullptr);
    ~AddEditWindow();

private slots:
    /*!
     * \brief Слот обработки нажатия на кнопку
     *
     * Вызывается при нажатии на кнопку добавления/изменения контакта
     *
     * Функция проверяет корректность введенных данных, после чего формирует экземпляры классов
     * KeyClass и DataClass, которые потом отправляет серверу. Текущее окно закрывается. Если
     * оно было создано в ShowWindow, формируется новое окно ShowWindow с обновленными данными по контакту
     */
    void on_pushButton_clicked();

private:

    //! Указатель на родительское окно
    QWidget *parentWidget;

    //! Пара, содержащая одну запись из справочника
    QPair <KeyClass, DataClass>* editPair;

    //! Переменная для хранения размера получаемого от клиента блока
    quint16 nextBlockSize;

    //! Указатель на сокет
    QLocalSocket* socket;

    //! Указатель на окно и все его виджеты
    Ui::AddEditWindow *ui;

};

#endif // ADDEDITWINDOW_H
