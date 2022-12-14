#pragma once
#include "Hikvision_Client.h"

class c_Hikvision_Remote : public QObject {
	Q_OBJECT

public:
	explicit c_Hikvision_Remote(QObject * parent = nullptr);
	virtual ~c_Hikvision_Remote();
	c_Hikvision_Client *m_Hikvision;
	public slots:
	void Init();
	//虚函数
	virtual void Connect();
	virtual void Connect_Loop();//循环连接
signals:
	void Connect_Device(QVariant Login, QVariant Client);
	void Disconnect_Device();
	void Connect_Done();//工作状态
	void Disconnect_Done();//非工作状态
	void Status(QString status);//监视器状态
};
