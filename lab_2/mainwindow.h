#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QShortcut>
#include <QFile>
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_input_db_clicked();
    void on_build_clicked();
    void on_show_table_list_doubleClicked(const QModelIndex &index);
    void update_db_options();

private:
    Ui::MainWindow *ui;
    Dialog         *dialog;
    QSqlDatabase    db;

    QString         HostName;
    QString         DatabaseName;
    int             Port;
    QString         UserName;
    QString         Password;
    QShortcut      *shortcut;

    void update_table_list();
    void update_error(QString string);
    void update_log();
    void load_dp_options();
    void init_db_options();
    void reconnect();
};

#endif
