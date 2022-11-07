#pragma once
#include "Jaka_Client.h"
/*************************************************************************************************************************************************
**Function:机械臂遥控父类定义
*************************************************************************************************************************************************/
class c_Jaka_Remote : public QObject
{
	Q_OBJECT
public:
	explicit c_Jaka_Remote(QObject *parent = nullptr);
	virtual ~c_Jaka_Remote();
	QThread *m_Jaka_Remote_Thread;
	c_Jaka_Client *m_Jaka_Remote;
	QJsonObject m_Jaka_Remote_State;//监控状态
	QString m_Name;
	QString m_Ip;
	int m_Port;
	public slots:
	//初始化接口
	void Init();
	//虚函数
	virtual void Connect();
	virtual void Connect_Loop();//循环检测连接状态
	//遥控接口
	void Jaka_power_on();
	void Jaka_power_off();
	void Jaka_enable_robot();
	void Jaka_disable_robot();
	void Jaka_play_program();
	void Jaka_pause_program();
	void Jaka_resume_program();
	void Jaka_stop_program();
	//获取程序名
	void Jaka_program_name(QString name);
signals:
	void Connect_Device(QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Write_Jaka_Remote_State(QJsonObject db);//写状态
	void Write_Json(QJsonObject json);//写Json消息
	void Write_Json_Done(QString cmdName);//写Json消息完成
	void Write_Json_Error(QString cmdName);//写Json消息错误
	void setEnabled(bool ready);//写消息状态
	void Set_Working();//工作状态
	void Set_Default();//非工作状态
	void Status(QString status);//遥控器状态
	void Origin_Monitor();//远点监控
	void Origin_Moved();//回原点
private:
	bool m_Writing = false;
	QJsonObject power_on;//上电
	QJsonObject power_off;//下电
	QJsonObject enable_robot;//上使能
	QJsonObject disable_robot;//下使能
	QJsonObject joint_move;//关节角运动
	QJsonObject load_program;//加载程序
	QJsonObject play_program;//运行程序
	QJsonObject pause_program;//暂停程序
	QJsonObject resume_program;//恢复程序
	QJsonObject stop_program;//停止程序
	private slots:
	void Connect_Done();
	void Disconnect_Done();
	void Write(QJsonObject json);
	void Read_Json_Done(QJsonObject json);
	void Jaka_joint_move(double J1, double J2, double J3, double J4, double J5, double J6);
	void Jaka_origin_move();
	void Jaka_load_program();
};
