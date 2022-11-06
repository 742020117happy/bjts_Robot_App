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
	QThread *m_Jaka_Monitor_Thread;
	c_Jaka_Client *m_Jaka_Monitor;
	QJsonObject m_Jaka_Monitor_State;
	bool m_Origin_Monitor = false;
	QString m_Ip;
	int m_Port;
	public slots:
	//初始化接口
	void Init();
	//虚函数
	virtual void Connect();
	virtual void Connect_Loop();//循环检测连接状态
	//进入状态监控，判断是否到达既定关节角位置
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
	void Origin_Moved();//回原点
private:
	private slots :
	void Connect_Done();
	void Disconnect_Done();
	void Read_Json_Done(QJsonObject json);
	bool Origin_Position(QJsonArray joint);
};
