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
    dialog = new Dialog(this);

    // shortcut ctrl + d
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(on_build_clicked()));
    connect(this->dialog, SIGNAL(update_db()), this, SLOT(update_db_options()));
    init_db_options();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_input_db_clicked()
{
    dialog->show();
  load_dp_options();

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
    QSqlQueryModel *setquery = new QSqlQueryModel;
    setquery->setQuery("SELECT table_name AS \"список таблиц\" FROM information_schema.tables\
                        WHERE table_schema = 'public'");
    ui->show_table_list->setModel(setquery);
}

void MainWindow::update_error(QString string)
{
    ui->show_errors->append(string);
}

void MainWindow::update_log()
{
    ui->show_logs->append(ui->input_field->toPlainText());
}

// открыть файл и заполнить поля класса
void MainWindow::load_dp_options()
{
QFile fin("file.txt");
if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)){
       db.setHostName("195.19.32.74");
       db.setDatabaseName("fn1132_2022");
       db.setPort(5432);
       db.setUserName("student");
       db.setPassword("bmstu");
      return;}
QTextStream in(&fin);
   QString line;


   line = in.readLine();
   db.setHostName          (line);
   line = in.readLine();
   db.setDatabaseName      (line);
   line = in.readLine();
   db.setPort              (line.toInt());
   line = in.readLine();
   db.setUserName          (line);
   line = in.readLine();
   db.setPassword          (line);




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

void MainWindow::setHostName     (QString &HostName)     { this->HostName = HostName; }
void MainWindow::setDatabaseName (QString &DatabaseName) { this->DatabaseName = DatabaseName; }
void MainWindow::setPort         (int      Port)         { this->Port = Port; }
void MainWindow::setUserName     (QString &UserName)     { this->UserName = UserName; }
void MainWindow::setPassword     (QString &Password)     { this->Password = Password; }







