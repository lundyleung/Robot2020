/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:05:06 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:05:06 
 */
#include "singlejointcontrol.h"
#include "ui_singlejointcontrol.h"

#include <QKeyEvent>

#include "drivers.h"
#include "qdebug.h"


#include "mycustomslider.h"
#include <QMessageBox>

inline double degree2rad(double x){
    return (x > 0 ? x : (360.0 + x)) * PI / 180.0;
}

inline double model2entity(double x, double cal){
    return fmod(x+cal, 360.0);
}

const double cal[6] = {182.141, 164.137, 90.896, 332.53, 349.84, 315.49};



/**
 * @brief Construct a new Single Joint Control:: Single Joint Control object
 * 
 */
SingleJointControl::SingleJointControl() : ui(new Ui::SingleJointControl)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Single Joint Control"));

    initObject();
    if (initUiObject())
    {
        initConnection();
        controlWidgetInit();
    }
    setCurrentNode(0);
}



/**
 * @brief 初始化界面信息，字符串代表界面控件的object名称
 * 
 */
void SingleJointControl::initObject()
{
    setSpeedSlider(QList<QString>({"speedHorizontalSlider"}));
    setPositionSlider(QList<QString>({"positionHorizontalSlider"}));
    setSpeedSpinBox(QList<QString>({"speedDoubleSpinBox"}));
    setPositionSpinBox(QList<QString>({"positionDoubleSpinBox"}));
    setPositionSetButton(QList<QString>({"positionSetButton"}));
    setSpeedSetButton(QList<QString>({"speedSetButton"}));
    setForwardReversePushButton(QList<QString>({"posForwardPushButton", "posReversalPushButton"}));
    setRelativeSpinBox(QList<QString>({"relativePosDoubleSpinBox"}));
}

/**
 * @brief 初始化电机
 * 
 */
void SingleJointControl::on_initDriverPushButton_clicked()
{
    init:
    Drivers::initJoint(ui->comboBox->currentIndex());

    bool normal_status = 1;
    QString rtn_msg = QString("");
    double position;

    // 等待500ms再检查电机状态
   QEventLoop loop;                              //定义一个新的事件循环
   QTimer::singleShot(500, &loop, SLOT(quit())); //创建单次定时器，槽函数为事件循环的退出函数
   loop.exec();

   int i = ui->comboBox->currentIndex();

    position = globalData->currentCanAnalyticalData[i].position;
    if ((359.98 < position && 360.0 > position) )
    {
        rtn_msg.append(tr("角度异常:%1！%2\n").arg(i+1).arg(position));
        normal_status = 0;
        //break;
    }
    else if(globalData->statusId[i] != 0x06)
    {
        normal_status = 0;
        rtn_msg.append(tr("[x] 电机%1 返回码异常: 0x0%2 \n").arg(i+1).arg(globalData->statusId[i]));
    }
    else
    {
        rtn_msg.append(tr("[-] 电机%1 正常。%2 \n").arg(i+1).arg(position));
    }

   if(normal_status)
   {
       QMessageBox:: StandardButton result = QMessageBox::warning(this, tr("电机状态提示"),//information(this, tr("电机状态提示"),
                                //QString("所有电机正常。"),
                                rtn_msg,
                                QMessageBox::Retry|QMessageBox::Ok);
       if (result ==  QMessageBox::Retry)
           goto init;
   }
   else{
       QMessageBox:: StandardButton result = QMessageBox::warning(this, tr("电机状态提示"),
                                rtn_msg,
                                QMessageBox::Retry|QMessageBox::Close);
       if (result ==  QMessageBox::Retry)
           goto init;
   }

//     重设角度为0

   //armAngle[i] = 0.0f;

   //findChild<QDoubleSpinBox *>(positionSpinBox[i])->setValue(model2entity(armAngle[i],cal[i]));
  // findChild<MyCustomSlider *>(positionSlider[i])->doubleSetValue(model2entity(armAngle[i],cal[i]));

   //updateModel(1);

}


/**
 * @brief 紧急停止
 * 
 */
void SingleJointControl::on_emergencyStopPushButton_clicked()
{
    Drivers::stopJoint(ui->comboBox->currentIndex());
}

/**
 * @brief 电机校准
 * 
 */
void SingleJointControl::on_caliPushButton_clicked()
{
    Drivers::calJoint(ui->comboBox->currentIndex());
}

/**
 * @brief 电机状态位清除
 *
 */
void SingleJointControl::on_faultPushButton_clicked()
{
    Drivers::faultJoint(ui->comboBox->currentIndex());
}


/**
 * @brief 打开关节电磁阀
 *
 */
void SingleJointControl::on_OpenValvePushButton_clicked()
{
    Drivers::initValve(ui->comboBox->currentIndex());
}


/**
 * @brief 关闭关节电磁阀
 *
 */
void SingleJointControl::on_CloseValvePushButton_clicked()
{
    Drivers::CloseValve(ui->comboBox->currentIndex());
}





/**
 * @brief 紧急停止快捷键ALT+A
 * 
 * @param e 
 */
void SingleJointControl::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_A:
    {
        if (QApplication::keyboardModifiers() == Qt::AltModifier)
        {
            on_emergencyStopPushButton_clicked();
        }
        break;
    }
    }
}

/**
 * @brief 选择需要进行操控的CAN节点
 * 
 * @param index 节点ID
 */
void SingleJointControl::on_comboBox_currentIndexChanged(int index)
{
    setCurrentNode(index);
}


/**
 * @brief 使能电机
 *
 *
 */
void SingleJointControl::on_MotorEnableButton_clicked()
{
    Drivers::EnableMotor(ui->comboBox->currentIndex());
}
/**
 * @brief 关闭电机
 *
 *
 */
void SingleJointControl::on_MotorDisableButton_clicked()
{
    Drivers::DisableMotor(ui->comboBox->currentIndex());
}
