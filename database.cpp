#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    currentQueryModel = new QSqlQueryModel(nullptr);
}

DataBase::~DataBase()
{
    delete currentQueryModel;
    if (currentTableModel)
    {
        delete currentTableModel;
    }

    delete dataBase;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());


    ///Тут должен быть код ДЗ


    bool status;
    status = dataBase->open( );
    if (status)
    {
        // табличную модель приходится пересоздавать при переподключении,
        // т.к. нет метода смены базы данных
        if (currentTableModel)
        {
            delete currentTableModel;
        }

        currentTableModel = new QSqlTableModel(nullptr, *dataBase);
    }

    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(int typeR)
{

    // Перенес формирование запросы в класс БД, чтобы изолировать основную форму от этих деталей
    QSqlQueryModel* model = nullptr;
    currentQueryModel->clear();
    currentTableModel->clear();

    switch (typeR)
    {
        case requestComedy:
            {
                auto queryStr = "SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id JOIN category c on c.category_id = fc.category_id WHERE c.name = 'Comedy' ORDER BY f.film_id";
                currentQueryModel->setQuery(queryStr, *dataBase);
                model = currentQueryModel;
            }
            break;
        case requestHorrors:
            {
                auto queryStr = "SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id JOIN category c on c.category_id = fc.category_id WHERE c.name = 'Horror' ORDER BY f.film_id";
                currentQueryModel->setQuery(queryStr, *dataBase);
                model = currentQueryModel;
            }
            break;
        default:
            currentTableModel->setTable("public.film");
            currentTableModel->select();
            // считаем, что в данном случае порядок полей в самой таблице не меняется
            // в противном случае можно было бы искать колонки по имени и перемещать их, как нужно
            currentTableModel->removeColumn(0);
            currentTableModel->removeColumns(2, currentTableModel->columnCount() - 2);
            model = currentTableModel;
            break;
    }

    model->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
    model->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
    emit sig_SendDataFromDB(model, typeR);
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
