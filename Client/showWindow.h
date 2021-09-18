#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QMainWindow>
#include <QLocalSocket>
#include <QMessageBox>
#include "class.h"
#include "addEditWindow.h"

/*!
 * \file
 * \brief Заголовочный файл, содержащий класс окна вывода информации о контакте
 *
 * В этом окне отображается полная информация о контакте. Доступна кнопка редактирования контакта.
 */

namespace Ui {
class ShowWindow;
}

/*!
 * \brief Окно вывода информации о контакте
 *
 * В этом окне отображается полная информация о контакте. Доступна кнопка редактирования контакта.
 */

class ShowWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowWindow(QPair <KeyClass, DataClass> parentPair, QLocalSocket *parentSocket, QWidget *parent = nullptr);
    ~ShowWindow();

private:
    //! Указатель на сокет
    QLocalSocket* socket;

    //! Указатель на окно и все его виджеты
    Ui::ShowWindow *ui;

    /*!
     * \brief Контейнер, содержащий все записи из телефонной книги
     *
     * Основная структура, используемая для хранения контактов. В качестве клююча используется экземпляр класса KeyClass,
     * хранящий в себе оснвную информацию (ФИО+номер), уникальную для каждой записи. В качестве значения используется
     * экземпляр класса DataClass, хранящий дополнительную информацию о контакте.
     */
    QPair <KeyClass, DataClass> pair;

private slots:
    /*!
     * \brief Слот обработки нажатия на кнопку
     *
     * Вызывается при нажатии на кнопку удаления контакта
     *
     * Функция вызывает диалоговое окно, что бы проверить, уверен ли пользователь в своём решении,
     * после чего отправляет запрос на сервер. Окно после этого закрывается.
     */
    void on_deleteButton_pressed();

    /*!
     * \brief Слот обработки нажатия на кнопку
     *
     * Вызывается при нажатии на кнопку редактирования контакта
     *
     * Функция открывает окно редактирования конта. Текущее окно закрывается.
     */
    void on_editButton_clicked();
};

#endif // SHOWWINDOW_H
