#include <iostream>
#include <fstream>
#include <vector>
#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QString>
#include "class.h"

using namespace std;

KeyClass::KeyClass(QStringList elems)
{
    this->Number = elems[0];
    this->FirstName = elems[1];
    this->MiddleName = elems[2];
    this->LastName = elems[3];
}

KeyClass::KeyClass()
{
    this->Number = "";
    this->FirstName = "";
    this->MiddleName = "";
    this->LastName = "";
}

QDataStream& operator>> (QDataStream &in, KeyClass &key)
{
    in >> key.Number >> key.FirstName >>  key.MiddleName >>  key.LastName;
    return in;
}

QDataStream& operator<< (QDataStream &out, const KeyClass &key)
{
    out << key.Number << key.FirstName << key.MiddleName << key.LastName;
    return out;
}

QTextStream& operator<< (QTextStream &out, const KeyClass &key)
{
    out << key.Number << "," << key.FirstName << ","
        << key.MiddleName << "," << key.LastName << ',';
    return out;
}

DataClass::DataClass(QStringList elems)
{
    this->Email = elems[4];
    this->City = elems[5];
    this->Street = elems[6];
    this->House = elems[7];
    this->Flat = elems[8];
    this->birthDate= elems[9];
    if (elems.size() == 10)
        this->Company = "";
    else
        this->Company = elems[10];;
}

DataClass::DataClass()
{
    this->Email = "";
    this->City = "";
    this->Street = "";
    this->House = "";
    this->Flat = "";
    this->birthDate = "";
    this->Company = "";
}

QDataStream& operator>> (QDataStream &in, DataClass &data)
{
    in >> data.Email >> data.City >> data.Street >> data.House
            >> data.Flat >> data.birthDate >> data.Company;
    return in;
}

QDataStream& operator<< (QDataStream &out, const DataClass &data)
{
    out << data.Email << data.City << data.Street << data.House
        << data.Flat << data.birthDate << data.Company;
    return out;
}

QTextStream& operator<< (QTextStream &out, const DataClass &data)
{
    out << data.Email << "," << data.City << "," << data.Street << "," << data.House << ","
        << data.Flat << "," << data.birthDate << "," << data.Company << "\n";
    return out;
}

bool readCSV(QString path, QMap <KeyClass, DataClass> &phones)
{
    if (!phones.empty())
        phones.clear();
    QFile* file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return false;

    QTextStream in(file);
    DataClass newData;
    while (!in.atEnd())
    {
        QStringList elems = in.readLine().split(",");
        KeyClass* key = new KeyClass(elems);
        DataClass* data = new DataClass(elems);
        phones.insert(*key, *data);
    }
    file->close();
    return true;
}

bool writeCSV(QString path, QMap <KeyClass, DataClass> &phones)
{
    QFile* file = new QFile(path);
    if (!file->open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(file);
    QMapIterator<KeyClass, DataClass> it(phones);
    while (it.hasNext())
    {
        it.next();
        stream << it.key() << it.value();
    }

    file->close();
    return true;
}
