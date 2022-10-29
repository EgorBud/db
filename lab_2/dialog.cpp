#include <QDebug>

#include "dialog.h"
#include "ui_dialog.h"
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{

        ui->setupUi(this);
    QSettings settings ("settings.ini",QSettings::IniFormat);
     settings.beginGroup("Settings");

     ui->adress        ->setText(settings.value("adress").toString());
     ui->database_name ->setText(settings.value("database_name").toString());
     ui->port          ->setText(settings.value("port").toString());
     ui->user_name     ->setText(settings.value("username").toString());
     ui->password      ->setText(settings.value("password").toString());

             settings.endGroup();
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

    QSettings settings("settings.ini",QSettings::IniFormat);
      settings.beginGroup("Settings");
   settings.setValue("adress"       , ui->adress          ->text());
   settings.setValue("database_name", ui->database_name   ->text());
   settings.setValue("port"         , ui->port            ->text());
   settings.setValue("user_name"    , ui->user_name       ->text());
   settings.setValue("password"     , ui->password        ->text());

    settings.endGroup();

}
