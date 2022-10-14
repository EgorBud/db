#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog = new Dialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_input_db_clicked()
{
    dialog->show();
}

