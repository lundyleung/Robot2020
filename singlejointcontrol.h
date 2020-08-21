/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:05:10 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:05:10 
 */
#ifndef SINGLEJOINTCONTROL_H
#define SINGLEJOINTCONTROL_H

#include <QDialog>

#include <QTimer>

#include "globaldata.h"
#include "kinemetics.h"
#include "Eigen/Core"
#include "Eigen/Dense"
#include "armscene.h"
#include "controls.h"


namespace Ui
{
class SingleJointControl;
}

class SingleJointControl : public Controls
{
    Q_OBJECT

public:
    explicit SingleJointControl();

    QStringList positionSlider;
    QStringList positionSpinBox;

private slots:
    void on_initDriverPushButton_clicked();

    void on_emergencyStopPushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_caliPushButton_clicked();

    void on_faultPushButton_clicked();

    void on_OpenValvePushButton_clicked();

    void on_CloseValvePushButton_clicked();



    void on_MotorEnableButton_clicked();

    void on_MotorDisableButton_clicked();

private:
    Ui::SingleJointControl *ui;

    arm *ur;
    ArmScene *modifier;

    double armAngle[1];

    void initObject();

    void keyPressEvent(QKeyEvent *e) override;

};

#endif // SINGLEJOINTCONTROL_H
