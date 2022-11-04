#pragma once
#include "Hypersen_Client.h"
/*************************************************************************************************************************************************
**Function:面阵雷达遥控父类定义
*************************************************************************************************************************************************/
class c_Hypersen_Remote : public QObject
{
	Q_OBJECT
public:
	explicit c_Hypersen_Remote(QObject *parent = nullptr);
	virtual ~c_Hypersen_Remote();
	QJsonObject m_Hypersen_Remote_State;
	quint8 m_device_id = 99;
	public slots:
	//初始化接口
	void Init();
	//虚函数
	virtual void Connect();
	void Run_Single_Shot();
	void Run_Continuous();
	void Run_Idle();
signals:
	void Connect_Device(QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write_Hypersen_Remote_State(QJsonObject db);//写状态
	void Set_Working();//工作状态
	void Set_Default();//非工作状态
	void setEnabled(bool ready);//写消息状态
	void Read_Ready(quint8 id);//读同步
	void Write(quint8 value);//写
	void Status(QString status);//监视器状态
private:
	c_Hypersen_Client *m_Hypersen_Remote;
	private slots :
	void Connect_Done(quint8 id);
	void Disconnect_Done();
	void Read_Json_Done(QJsonObject json);
};
