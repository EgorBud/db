#include <QCoreApplication>
#include <QtSql>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db");

    if (!db.open()) {
        std::cerr << "Error: Can't open " << db.databaseName().toStdString()
                  << " " << db.lastError().text().toStdString() << std::endl;
        return 1;
    }

    db.exec("PRAGMA foreign_keys = ON");
    std::string command;
    QSqlQuery query(db);
    for (;;) {
        getline(std::cin, command);
        if (command == "stop")
            break;

        query.exec(command.c_str());

        if (!query.isActive()) {
            std::cerr << "Error: " << query.lastError().text().toStdString() << std::endl;
            continue;
        }

        if (!query.isSelect())
            std::cout << "OK" << std::endl;
        else {

            QSqlRecord record = query.record();
            std::cout << "|";
            for (int i = 0; i < record.count(); i++) {
                std::cout.width(15);
                std::cout << record.fieldName(i).toStdString() << "|";
            }
            std::cout << std::endl;

            while (query.next()) {
                record = query.record();
                std::cout << "|";
                for (int i = 0; i < record.count(); i++) {
                    std::cout.width(15);
                    std::cout << record.value(i).toString().toStdString() << "|";
                }
                std::cout << std::endl;
            }

        }
    }

    std::cout << "OK" << std::endl;
    return QCoreApplication::exec();
}
