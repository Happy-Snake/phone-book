#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <QMap>
#include <QTextStream>


/*!
 * \file
 * \brief Заголовочный файл, содержащий классы для хранения информации о контакте
 *
 * Данный файл содержит в себе определения двух основных
 * классов KeyClass и DataClass, используемых для хранения информации о контакте
 */


/*!
 * \brief Класс основной информации о контакте
 *
 * Класс содержит основную информацию о контакте: номер и ФИО. В дальнейшем экземпляры класса будут
 * использоваться в качестве ключей для QMap, таким образом обеспечивается уникальность пар ФИО+номер
 */

class KeyClass
{
private:
    QString FirstName;
    QString MiddleName;
    QString LastName;
    QString Number;

public:
    KeyClass(QStringList elems);
    KeyClass();
    /*!
     * \brief Перегрузка оператора сравнения для класса KeyClass
     */
    friend bool operator<(const KeyClass&k1, const KeyClass&k2)
    {
        if (QString::compare(k1.FirstName, k2.FirstName))
            return QString::compare(k1.FirstName, k2.FirstName);
        if (QString::compare(k1.MiddleName, k2.MiddleName))
            return QString::compare(k1.MiddleName, k2.MiddleName);
        if (QString::compare(k1.LastName, k2.LastName))
            return QString::compare(k1.LastName, k2.LastName);
        return QString::compare(k1.Number, k2.Number);
    }

    /*!
     * \brief Перегрузка оператора записи экземпляра класса DataClass в поток QDataStream
     * \param out ссылка на поток QDataStream, который осуществляет передачу информации между сервером и клтиентом
     * \param key ссылка на экземпляр класса KeyClass, содержащий дополнительную информацию о передаваемом контакте
     * \return ссылка на используемый поток QDataStream
     */
    friend QDataStream& operator<< (QDataStream &out, const KeyClass &key);

    /*!
     * \brief Перегрузка оператора чтения экземпляра класса DataClass из потока QDataStream
     * \param out ссылка на поток QDataStream, который осуществляет передачу информации между сервером и клтиентом
     * \param key ссылка на экземпляр класса KeyClass, содержащий дополнительную информацию о передаваемом контакте
     * \return ссылка на используемый поток QDataStream
     */
    friend QDataStream& operator>> (QDataStream &out, KeyClass &key);

    /*!
     * \brief Перегрузка оператора записи экземпляра класса DataClass в поток QTextStream
     * \param out ссылка на поток QTextStream, который осуществляет запись информации в QFile
     * \param key ссылка на экземпляр класса KeyClass, содержащий дополнительную информацию о передаваемом контакте
     * \return ссылка на используемый поток QTextStream
     */
    friend QTextStream& operator<< (QTextStream &out, const KeyClass &key);
};

/*!
 * \brief Класс дополнительной информации о контакте
 *
 * Класс содержит дополнительную информацию о контакте: адрес электронной почты, место работы, адрес проживания
 * и дату рождения. В дальнейшем экземпляры класса будут использоваться в качестве значений для QMap
 */

class DataClass
{
private:
    QString Email;
    QString City;
    QString Street;
    QString House;
    QString Flat;
    QString birthDate;
    QString Company;

public:
    DataClass(QStringList elems);
    DataClass();
    QString getEmail(){return this->Email;}
    QString getCity(){return this->City;}
    QString getStreet(){return this->Street;}
    QString getHouse(){return this->House;}
    QString getFlat(){return this->Flat;}
    QString getBirthDate(){return this->birthDate;}
    QString getCompany(){return this->Company;}

    /*!
     * \brief Перегрузка оператора записи экземпляра класса DataClass в поток QDataStream
     * \param out ссылка на поток QDataStream, который осуществляет передачу информации между сервером и клтиентом
     * \param data ссылка на экземпляр класса DataClass, содержащий дополнительную информацию о передаваемом контакте
     * \return ссылка на используемый поток QDataStream
     */
    friend QDataStream& operator<< (QDataStream &out, const DataClass &data);

    /*!
     * \brief Перегрузка оператора чтения экземпляра класса DataClass из потока QDataStream
     * \param out ссылка на поток QDataStream, который осуществляет передачу информации между сервером и клтиентом
     * \param data ссылка на экземпляр класса DataClass, содержащий дополнительную информацию о передаваемом контакте
     * \return ссылка на используемый поток QDataStream
     */
    friend QDataStream& operator>> (QDataStream &out, DataClass &data);

    /*!
     * \brief Перегрузка оператора записи экземпляра класса DataClass в поток QTextStream
     * \param out ссылка на поток QTextStream, который осуществляет запись информации в QFile
     * \param data ссылка на экземпляр класса DataClass, содержащий дополнительную информацию о передаваемом контакте
     * \return ссылка на используемый поток QTextStream
     */
    friend QTextStream& operator<< (QTextStream &out, const DataClass &data);
};

/*!
 * \brief Функция чтения csv файла и записи контактов
 * \param строка, содержащая путь до файла с контактами
 * \param ссылка на конейнер QMap номеров телефонов, в который будет осуществлена запись контаков
 * \return 1, если файл был успешно открыт, 0, если произошла ошибка при открытии файла
 */
bool readCSV(QString path, QMap <KeyClass, DataClass> &phones);

/*!
 * \brief Функция записи контаков в файл
 * \param строка, содержащая путь до файла
 * \param ссылка на конейнер QMap номеров телефонов, которые будут записаны в файл
 * \return 1, если файл был успешно открыт, 0, если произошла ошибка при открытии файла
 */
bool writeCSV(QString path, QMap <KeyClass, DataClass> &phones);

