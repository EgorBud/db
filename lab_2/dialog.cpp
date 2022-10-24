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
    save_db_options();
    emit update_db();
    hide();
}


void Dialog::on_cancel_clicked()
{
    hide();
}

void Dialog::save_db_options()
{
    QFile fout("file.txt");
    if (!fout.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "ERROR";
        return;
    }
    QTextStream out(&fout);
    out << ui->adress        ->text() << "\n"
        << ui->database_name ->text() << "\n"
        << ui->port          ->text() << "\n"
        << ui->user_name     ->text() << "\n"
        << ui->password      ->text() << "\n";
    fout.close();

}
