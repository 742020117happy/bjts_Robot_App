#pragma once
/*************************************************************************************************************************************************
**Function:RGV功能映射
*************************************************************************************************************************************************/
#define RGV_Fuction_0      0//通信控制（线圈）                （离散：为1时远程控制，PLC端无控制））
#define RGV_Fuction_7      7//主动力位置清零（线圈，工控机立即复位）（离散）
#define RGV_Fuction_8      8//主动力故障复位（线圈，工控机立即复位）（离散）
#define RGV_Fuction_9      9//主动力手动左行（线圈，点控）          （离散：为1运行中）
#define RGV_Fuction_10     10//主动力手动右行（线圈，点控）         （离散：为1运行中）
#define RGV_Fuction_11     11//正向连续运行（线圈，工控机立即复位） （离散：为1运行中）
#define RGV_Fuction_12     12//返向连续运行（线圈，工控机立即复位） （离散：为1运行中）
#define RGV_Fuction_13     13//返向连续运行再启动（线圈，工控机立即复位）（离散：为1运行中）
#define RGV_Fuction_14     14//急停（线圈，工控机立即复位）（离散）
#define RGV_Fuction_15     15//左机器人开机（线圈，工控机立即复位）（离散：为1完成开机）
#define RGV_Fuction_16     16//左机器人关机（线圈，工控机立即复位）（离散：为1完成关机）
#define RGV_Fuction_17     17//右机器人开机（线圈，工控机立即复位）（离散：为1完成开机）
#define RGV_Fuction_18     18//右机器人关机（线圈，工控机立即复位）（离散：为1完成关机）
#define RGV_Fuction_19     19//上电（线圈，工控机立即复位）        （离散：为1所有设备上电处于待机状态）
#define RGV_Fuction_20     20//充电回原点（线圈，工控机立即复位）  
#define RGV_Fuction_21     21//风刀（线圈，工控机立即复位）  
#define RGV_Fuction_22     22//清扫电机（线圈，工控机立即复位）  
#define RGV_Fuction_24     24//轴1移动（线圈，工控机立即复位）（离散：为为1移动中）
#define RGV_Fuction_25     25//轴2移动（线圈，工控机立即复位）（离散：为1移动中）
#define RGV_Fuction_26     26//开始充电（线圈，工控机立即复位）（离散：为1时远程控制，PLC端无控制）
#define RGV_Fuction_27     27//结束充电（线圈，工控机立即复位）（离散：为1时远程控制，PLC端无控制）
#define RGV_Date_0     0//左升降台速度
#define RGV_Date_1     2//左升降台位置
#define RGV_Date_2     4//右升降台速度
#define RGV_Date_3     6//右升降台位置
#define RGV_Date_4     8//轴1移动距离
#define RGV_Date_5     9//轴2移动距离
/*************************************************************************************************************************************************
**Function:线阵雷达IO映射
*************************************************************************************************************************************************/
#define IO_OUTNUM 3
#define IO_INNUM 2
#define PI 3.1415926
/*************************************************************************************************************************************************
**Function:包含目录
*************************************************************************************************************************************************/
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <SDKDDKVer.h>
#include <Windows.h>
using namespace std;

#include <QMap>
#include <QDir>
#include <QFile>
#include <Qbrush>
#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QObject>
#include <QThread>
#include <QVector>
#include <Qpainter>
#include <QtCharts>
#include <QVariant>
#include <QDateTime>
#include <QKeyEvent>
#include <QJsonValue>
#include <QByteArray>
#include <QJsonArray>
#include <QEventLoop>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QMainWindow>
#include <QJsonObject>
#include <QPolarChart>
#include <QMessageBox>
#include <QPainterPath>
#include <QHostAddress>
#include <QApplication>
#include <QJsonDocument>
#include <QstackedWidget>
#include <QScatterSeries>
#include <QAbstractSocket>
#include <QModbusDataUnit>
#include <QConicalGradient>
#include <QModbusTcpClient>


