#include "dbdata.h"
#include "database.h"
#include "ui_dbdata.h"

DbData::DbData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbData)
{
    ui->setupUi(this);


    //Ресайзим вектор значений, по количеству полей необходимых для
    //подключения к БД
    data.resize(NUM_DATA_FOR_CONNECT_TO_DB);
}

DbData::~DbData()
{
    delete ui;
}

void DbData::setDefaulConnectiontData(const QVector<QString> cData)
{
    {
        auto currPos = static_cast<int>(fieldsForConnect::hostName);
        if (cData.size() <= currPos) { return; }
        ui->le_host->setText(cData[currPos]);
    }

    {
        auto currPos = static_cast<int>(fieldsForConnect::dbName);
        if (cData.size() <= currPos) { return; }
        ui->le_dbName->setText(cData[currPos]);
    }

    {
        auto currPos = static_cast<int>(fieldsForConnect::login);
        if (cData.size() <= currPos) { return; }
        ui->le_login->setText(cData[currPos]);
    }

    {
        auto currPos = static_cast<int>(fieldsForConnect::pass);
        if (cData.size() <= currPos) { return; }
        ui->le_pass->setText(cData[currPos]);
    }

    {
        auto currPos = static_cast<int>(fieldsForConnect::port);
        if (cData.size() <= currPos) { return; }
        bool isPort;
        auto port = cData[currPos].toInt(&isPort);
        if (isPort)
        {
            ui->spB_port->setValue(port);
        }
        else
        {
            ui->spB_port->setValue(5432);
        }
    }
}
/*!
 * \brief Обработчик кнопки "Ок"
 */
void DbData::on_buttonBox_accepted()
{

    //Добавляем данные в контейнер и передаем в главное окно
    data[hostName] = ui->le_host->text();
    data[dbName] = ui->le_dbName->text();
    data[login] = ui->le_login->text();
    data[pass] = ui->le_pass->text();
    data[port] = ui->spB_port->text();

    emit sig_sendData(data);

}

