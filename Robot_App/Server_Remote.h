#pragma once
#include "Robot_Server.h"
/*************************************************************************************************************************************************
**Function:对外接口父类定义
*************************************************************************************************************************************************/
class c_Server_Remote : public QObject
{
	Q_OBJECT
public:
	explicit c_Server_Remote(QObject *parent = nullptr);
	virtual ~c_Server_Remote();
	c_Robot_Server *m_Robot_Server;
	QString m_Ip;
	int m_Port;
	QJsonObject m_Server_State;
	public slots:
	void Init();
	void System_Scan(QJsonObject db);
signals:
	void Write_Server_State(QJsonObject db);//写状态
	void Set_Working();//工作状态
	void Set_Default();//非工作状态
	void setEnabled(bool ready);//写消息状态
	void Status(QString status);//监视器状态
	void Start_Cmd(QJsonObject cmd);//开始巡检
	void Fuction_0_Set();//通信控制（线圈）
	void Fuction_0_Reset();//通信控制（线圈）
	void Fuction_7();//主动力位置清零（线圈，工控机立即复位）
	void Fuction_8();//主动力故障复位（线圈，工控机立即复位）
	void Fuction_9_Set();//主动力手动左行（线圈，点控）
	void Fuction_9_Reset();//主动力手动左行（线圈，点控）
	void Fuction_10_Set();//主动力手动右行（线圈，点控） 
	void Fuction_10_Reset();//主动力手动右行（线圈，点控） 
	void Fuction_11();//正向连续运行（线圈，工控机立即复位）
	void Fuction_12();//返向连续运行（线圈，工控机立即复位）
	void Fuction_13();//返向连续运行再启动（线圈，工控机立即复位）
	void Fuction_14();//急停（线圈，工控机立即复位）（离散）
	void Fuction_15();//左机器人开机（线圈，工控机立即复位）
	void Fuction_16();//左机器人关机（线圈，工控机立即复位）
	void Fuction_17();//右机器人开机（线圈，工控机立即复位）
	void Fuction_18();//右机器人关机（线圈，工控机立即复位）
	void Fuction_19_Set();//上电（线圈，点控）
	void Fuction_19_Reset();//上电（线圈，点控）
	void Fuction_20();//充电回原点
	void Fuction_21_Set();//清扫电机
	void Fuction_21_Reset();//清扫电机
	void Fuction_22_Set();//风刀电机
	void Fuction_22_Reset();//风刀电机
	void Fuction_24(int Position);//轴1移动（线圈，工控机立即复位）
	void Fuction_25(int Position);//轴2移动（线圈，工控机立即复位）
	void Fuction_26();//开始充电（线圈，工控机立即复位）
	void Fuction_27();//结束充电（线圈，工控机立即复位）
	void Date_0(int Position, int Speed);//左升降台
	void Date_4(int Position, int Speed);//右升降台
	void Jaka_120_power_on();//左机械臂打开电源
	void Jaka_120_power_off();//左机械臂关闭电源
	void Jaka_120_disable();//左机器人下使能
	void Jaka_120_enable();//左机器人上使能
	void Jaka_120_play_program(QString programName);//左机械臂运行轨迹文件
	void Jaka_120_stop_program();//左机械臂运行轨迹文件
	void Jaka_121_power_on();//左机械臂打开电源
	void Jaka_121_power_off();//左机械臂关闭电源
	void Jaka_121_disable();//左机器人下使能
	void Jaka_121_enable();//左机器人上使能
	void Jaka_121_play_program(QString programName);//左机械臂运行轨迹文件
	void Jaka_121_stop_program();//左机械臂运行轨迹文件 
private:
	QJsonObject m_State_DB;
	QString m_Cmd_Name;
	bool m_Value;
	private slots :
	void Connect_Done();
	void Disconnect_Done();
	void Read_Json_Done(QJsonObject json);
};