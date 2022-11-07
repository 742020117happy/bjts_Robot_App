#pragma once
#include "Scan_Client.h"
/*************************************************************************************************************************************************
**Function:立创相机父类定义
*************************************************************************************************************************************************/
class c_Scan_Remote : public QObject
{
	Q_OBJECT
public:
	explicit c_Scan_Remote(QObject *parent = nullptr);
	virtual ~c_Scan_Remote();
	QThread *m_Scan_Remote_Thread;
	c_Scan_Client *m_Scan_Remote;
	QString m_Track_Rank = "D98";//股道号
	QString m_Begin_Time = "202209201622";//开始时间
	QString m_Car_Type = "CHR380AL";//任务车型
	QString m_Car_Num = "2411";//任务车号
	QString m_Work_Num = "202209201622CHR380AL2411";//任务编号
	QString m_Carbox_Num = "1";
	QString m_Bogie_Num = "A";
	QString m_Axis_Num = "1";
	QJsonObject m_Scan_Remote_State;
	public slots:
	//外部操作接口
	void Init();
	void Start_Cmd(QJsonObject object);
	virtual void Connect();
	virtual void Connect_Loop();
	virtual void Start();
	virtual void Collection();
	virtual void Stop();
	virtual void Tran(QString value);
signals:
	void Connect_Device(QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write_Scan_Remote_State(QJsonObject db);//写状态
	void Set_Working();//工作状态
	void Set_Default();//非工作状态
	void setEnabled(bool ready);//写消息状态
	void Write_String(QString ip, int port, QString value);
	void Read_String_Done(QString str);
	void Status(QString status);//监视器状态
	void Collect_Done();
private:
	private slots :
    void Connect_Done();
	void Disconnect_Done();
};
