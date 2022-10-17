#include <QStringList>
#include <QTableView>
#include <QMessageBox>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QMainWindow *win = this;
    win->setFixedSize(810, 610);
    ui->setupUi(win);
    dialog = new Dialog(this);

    // shortcut ctrl + d
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(on_build_clicked()));

    init_db_options();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_input_db_clicked()
{
    dialog->show();
}

void MainWindow::on_show_table_list_doubleClicked(const QModelIndex &index)
{
    QString name_table = (index.model()->data(index.model()->index(index.row(),index.column())).toString());
    QSqlQueryModel *query_model = new QSqlQueryModel;
    query_model->setQuery("SELECT * FROM " + name_table);
    ui->show_table->setModel(query_model);
}

void MainWindow::on_build_clicked()
{
    QSqlQuery query(db);
    query.exec(ui->input_field->toPlainText());

    if(!query.isActive()) {
        update_error(query);
        return;
    }

    QSqlQueryModel *query_model = new QSqlQueryModel;
    query_model->setQuery(query);
    ui->show_table->setModel(query_model);

    update_log();
    update_table_list();

}

void MainWindow::update_table_list()
{
    QSqlQueryModel *setquery = new QSqlQueryModel;
    setquery->setQuery("SELECT table_name AS \"список таблиц\" FROM information_schema.tables\
                        WHERE table_schema = 'public'");
    ui->show_table_list->setModel(setquery);
}

void MainWindow::update_error(QSqlQuery &query)
{
    ui->show_errors->append(query.lastError().text());
}

void MainWindow::update_log()
{
    ui->show_logs->append(ui->input_field->toPlainText());
}

// открыть файл и заполнить поля класса
void MainWindow::load_dp_options()
{

}

void MainWindow::init_db_options()
{  
    db = QSqlDatabase::addDatabase("QPSQL");

    // база даных препода
    db.setHostName("195.19.32.74");
    db.setDatabaseName("fn1132_2022");
    db.setPort(5432);
    db.setUserName("student");
    db.setPassword("bmstu");

    load_dp_options();
    update_table_list();
}

void MainWindow::setHostName     (QString &HostName)     { this->HostName = HostName; }
void MainWindow::setDatabaseName (QString &DatabaseName) { this->DatabaseName = DatabaseName; }
void MainWindow::setPort         (int      Port)         { this->Port = Port; }
void MainWindow::setUserName     (QString &UserName)     { this->UserName = UserName; }
void MainWindow::setPassword     (QString &Password)     { this->Password = Password; }







