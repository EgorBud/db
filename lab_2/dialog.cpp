#include <QDebug>

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_ok_clicked()
{
    qDebug() << "Pressed ok";
    save_db_options();
}


void Dialog::on_cancel_clicked()
{
    qDebug() << "Pressed cancel";
    hide();
}

// сохранить данные о бд в файл
void Dialog::save_db_options()
{

}
