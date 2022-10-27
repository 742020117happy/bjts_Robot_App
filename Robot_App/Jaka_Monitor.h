#pragma once
#include "Jaka_Client.h"
/*************************************************************************************************************************************************
**Function:机械臂监视父类定义
*************************************************************************************************************************************************/
class c_Jaka_Monitor : public QObject
{
	Q_OBJECT
public:
	explicit c_Jaka_Monitor(QObject *parent = nullptr);
	virtual ~c_Jaka_Monitor();
	QJsonObject m_Jaka_Monitor_State;
	//共有状态参数
	QJsonArray joint_actual_position = {};
	QJsonArray actual_position = {};
	QJsonArray homed = {};
	QJsonArray drag_near_limit = {};
	int task_state = 0;
	int task_mode = 0;
	int interp_state = 0;
	int current_tool_id = 0;
	int protective_stop = 0;
	int on_soft_limit = 0;
	int emergency_stop = 0;
	public slots:
	//初始化接口
	void Init();
	//虚函数
	virtual void Connect();
	//进入状态监控，判断是否到达既定关节角位置
	void Power_Monitor();//上电监视
	void Enable_Monitor();//使能监视
	void Origin_Monitor();//回原点
signals:
	void Connect_Device(QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write_Jaka_Monitor_State(QJsonObject db);//写状态
	void Set_Working();//工作状态
	void Set_Default();//非工作状态
	void setEnabled(bool ready);//写消息状态
	void Status(QString status);//监视器状态
	void Read_Ready();//读同步
	void Power_On();//上电
	void Power_Off();//下电
	void Enable_Robot();//上使能
	void Disable_Robot();//下使能
	void Origin_Moved();//回原点
private:
	QThread *m_Jaka_Monitor_Thread;
	c_Jaka_Client *m_Jaka_Monitor;
	bool m_Power_Monitor = false;
	bool m_Able_Monitor = false;
	bool m_Origin_Monitor = false;
	private slots :
	void Connect_Done();
	void Disconnect_Done();
	void Read_Json_Done(QJsonObject json);
	bool Origin_Position(QJsonArray joint);
};
