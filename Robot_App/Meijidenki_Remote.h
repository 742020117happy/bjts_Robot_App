#pragma once
#include "Meijidenki_Client.h"
/*************************************************************************************************************************************************
**Function:线阵阵雷达遥控父类定义
*************************************************************************************************************************************************/
class c_Meijidenki_Remote : public QObject
{
	Q_OBJECT
public:
	explicit c_Meijidenki_Remote(QObject *parent = nullptr);
	virtual ~c_Meijidenki_Remote();
	QJsonObject m_Meijidenki_Remote_State;
	public slots:
	//初始化接口
	void Init();
	//虚函数
	virtual void Connect();
	//外部操作接口
	void START_LMD();
	void STOP_LMD();
signals:
	void Connect_Device(int id, QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write_Meijidenki_Remote_State(QJsonObject db);//写状态
	void Set_Working();//工作状态
	void Set_Default();//非工作状态
	void setEnabled(bool ready);//写消息状态
	void Read_Ready(qint32 id);//读同步
	void Write(quint32 value);//写
	void Status(QString status);//监视器状态
private:
	c_Meijidenki_Client *m_Meijidenki_Remote;
	private slots :
	void Connect_Done();
	void Disconnect_Done();
	void Read_Json_Done(QJsonObject json);
};
