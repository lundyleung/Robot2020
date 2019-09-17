#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "datatransmission.h"
#include "globaldata.h"

#include "debug.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_1000));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_800));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_666));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_500));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_400));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_250));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_200));
    ui->bauteComboBox->addItem(QString::number(CAN_BAUTE_125));

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowTitle(tr("Connect"));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::accept()
{
    int baud=ui->bauteComboBox->currentIndex();

    if(DataTransmission::connectToCan(GlobalData::connectType, baud)==-1)
        return;

    qDebug() << "Connect CAN OK!";
    return QDialog::accept();
}
