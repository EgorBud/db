#include <QStringList>
#include <QTableView>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(on_build_clicked()));
    ui->show_table_list->setEditTriggers(QAbstractItemView::NoEditTriggers);

    init_db_options();
    dialog = new Dialog(this);
    connect(this->dialog, SIGNAL(update_db()), this, SLOT(update_db_options()));

    QFile fin("file.txt");
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR";
        return;
    }

    QTextStream in(&fin);
    ui->show_logs->setText(in.readAll());
    fin.close();
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
    reconnect();
    QString name_table = (index.model()->data(index.model()->index(index.row(),index.column())).toString());
    QSqlQueryModel *query_model = new QSqlQueryModel();
    query_model->setQuery("SELECT * FROM " + name_table);
    ui->show_table->setModel(query_model);
}

void MainWindow::on_build_clicked()
{
    reconnect();
    QString command = ui->input_field->toPlainText();
    if(command == "") {
        update_error("ERROR: empty query");
        return;
    }

    QSqlQuery query(db);
    query.exec(command);
    if(!query.isActive()) {
        update_error(query.lastError().text());
        return;
    }

    QSqlQueryModel *query_model = new QSqlQueryModel;
    query_model->setQuery(query);
    ui->show_table->setModel(query_model);

    update_log();
    update_table_list();
}


void MainWindow::update_db_options()
{
    load_dp_options();
    bool check = db.open();
    if (!check) {
        update_error(db.lastError().text());
        qDebug() << "ERROR: database not open";
        return;
    }
    update_table_list();
}


void MainWindow::update_table_list()
{
    QStringList tables = db.tables();
    QStringListModel *model = new QStringListModel();
    model->setStringList(tables);
    ui->show_table_list->setModel(model);
}

void MainWindow::update_error(QString string)
{
    ui->show_errors->setText(string);
}

void MainWindow::update_log()
{
    update_error("");
    ui->show_logs->append(ui->input_field->toPlainText());

    QFile fout("file.txt");
    if (!fout.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append))
    {
        qDebug() << "ERROR";
        return;
    }
    QTextStream out(&fout);
    out << ui->input_field->toPlainText() << "\n";
    fout.close();
}

void MainWindow::load_dp_options()
{
    QSettings settings ("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Settings");
    db.setHostName      (settings.value("adress"      , "195.19.32.74").toString());
    db.setDatabaseName  (settings.value("databasename", "fn1132_2022" ).toString());
    db.setPort          (settings.value("port"        ,  5432         ).toInt());
    db.setUserName      (settings.value("username"    , "student"     ).toString());
    db.setPassword      (settings.value("password"    , "bmstu"       ).toString());
    settings.endGroup();

    settings.beginGroup("Settings");
    settings.setValue("adress"      , db.hostName());
    settings.setValue("databasename", db.databaseName());
    settings.setValue("port"        , db.port());
    settings.setValue("username"    , db.userName());
    settings.setValue("password"    , db.password());
    settings.endGroup();
}

void MainWindow::init_db_options()
{  
    db = QSqlDatabase::addDatabase("QPSQL");
    load_dp_options();
    bool check = db.open();
    if (!check) {
        update_error(db.lastError().text());
        qDebug() << "ERROR: database not open";
        return;
    }
    update_table_list();
}

void MainWindow::reconnect()
{
    if (!db.open()) {
        db.setConnectOptions();
        db.open();
        qDebug() << "Reconnect";
    }
}


