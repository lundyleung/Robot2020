/*
 * @Descripttion: 
 * @version: 
 * @Author: xingzhang.Wu
 * @Date: 2019-10-23 17:09:23
 * @LastEditors  : Qingmao Wei
 * @LastEditTime : 2020-01-15 15:21:04
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connectdialog.h"
#include "plots.h"
#include "jointplot.h"
#include "jointspeedmeas.h"
#include "jointcontrol.h"
#include "remotecontrol.h"
#include "singlejointcontrol.h"
#include "offlinecontrol.h"
#include "receiveworkerthread.h"
#include "offlinesequencecontrol.h"
#include "terminal.h"
#include "receiveerror.h"
#include "globaldata.h"

#include <QTimer>

#include "debug.h"

/**
 * @brief Construct a new Main Window:: Main Window object
 * 
 * @param parent 
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Initialize the connection window
    m_connect_dialog = new ConnectDialog;
#if SIMULATE_CONNECT == NONE_CONNECT
    QTimer::singleShot(50, m_connect_dialog, &ConnectDialog::show);
#else
    canConnectEvent();
#endif

    //Initialize joint window
    m_joint_widget = new JointPlot;

    //Initialize joint speed measure window
    // m_joint_speed_meas_widget = new JointSpeedMeas;

    //Initialize terminal window
    m_terminal = new Terminal;

    //Initialize joint control window
    m_joint_control = new JointControl;

    //Initialize joint control window
    m_single_joint_control = new SingleJointControl;

    //Initialize offline control window
    m_offline_control = new OfflineControl;

    //Initialize test window
    m_receive_error = new ReceiveError;

    //Initialize remote control window
    m_remote_control = new RemoteControl;

    m_offline_sequence_control = new OfflineSequenceControl;

    connectInit();
}

/**
 * @brief 启动CAN接收线程
 * 
 */
void MainWindow::canConnectEvent()
{
    //start receive thread
    workerThread = new ReceiveWorkerThread();
    // After the end of the thread, automatically destroy
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
    workerThread->start();
    //set status
    if (globalData->connectType)
        ui->statusBar->showMessage(tr("Connected"));
    else if (SIMULATE_CONNECT != NONE_CONNECT)
        ui->statusBar->showMessage(tr("Simulate"));
}

/**
 * @brief 对connect的初始化
 * 
 */
void MainWindow::connectInit()
{
    connect(ui->actionConnect_CAN, &QAction::triggered, m_connect_dialog, &ConnectDialog::show);
    connect(ui->actionDisconnect_CAN, &QAction::triggered, m_connect_dialog, &ConnectDialog::disconnect);
    connect(ui->actionTerminal, &QAction::triggered, m_terminal, &Terminal::show);
    connect(ui->actionJoint, &QAction::triggered, m_joint_widget, &JointPlot::show);
    // connect(ui->actionJoint_Speed_Meas, &QAction::triggered, m_joint_speed_meas_widget, &JointSpeedMeas::show);
    connect(ui->actionJoint_Control, &QAction::triggered, m_joint_control, &JointControl::show);
    connect(ui->actionSingle_Joint_Control, &QAction::triggered, m_single_joint_control, &SingleJointControl::show);
    connect(ui->actionOffline_Control, &QAction::triggered, m_offline_control, &OfflineControl::show);
    connect(ui->actionSequence_Control, &QAction::triggered, m_offline_sequence_control, &OfflineSequenceControl::show);
    connect(ui->actionRemote_Control, &QAction::triggered, m_remote_control, &RemoteControl::show);
    connect(m_connect_dialog, &ConnectDialog::accepted, this, &MainWindow::canConnectEvent);
    connect(ui->actionReceive_Error, &QAction::triggered, m_receive_error, &ReceiveError::show);
    connect(m_receive_error, &ReceiveError::jointError, m_offline_control, &OfflineControl::pausedWhenError);
    connect(m_receive_error, &ReceiveError::jointError, m_offline_sequence_control, &OfflineSequenceControl::pausedWhenError);
}

/**
 * @brief Destroy the Main Window:: Main Window object
 * 
 */
MainWindow::~MainWindow()
{
    delete m_connect_dialog;
    delete m_joint_widget;
    // delete m_joint_speed_meas_widget;
    delete m_terminal;
    delete m_joint_control;
    delete m_single_joint_control;
    delete m_offline_control;
    delete m_receive_error;
    delete m_remote_control;
    delete m_offline_sequence_control;
    delete workerThread;
    delete ui;
}
