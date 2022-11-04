#pragma once
#include "Hikvision_Client.h"

class c_Hikvision_Remote : public QObject {
	Q_OBJECT

public:
	explicit c_Hikvision_Remote(QObject * parent = nullptr);
	virtual ~c_Hikvision_Remote();
	public slots:
	void Init();
	//虚函数
	virtual void Connect();

signals:
	void Connect_Device(QVariant Login, QVariant Client);
	void Disconnect_Device();
	void Connect_Done();//工作状态
	void Disconnect_Done();//非工作状态
	void Status(QString status);//监视器状态

private:
	c_Hikvision_Client *m_Hikvision;
	
	
};
