#pragma once
#include "Scan_Remote.h"
#include "Robot_Server.h"

class c_Prec_Scan_121_Remote : public c_Scan_Remote
{
	Q_OBJECT
public:
	explicit c_Prec_Scan_121_Remote(c_Scan_Remote *parent = nullptr);
	virtual ~c_Prec_Scan_121_Remote();
	public slots:
	void Init();
	//外部操作接口
	void Connect_Server();
	virtual void Connect();
	virtual void Tran(QString value);
	virtual void Connect_Loop();
private:
	c_Robot_Server *m_Prec_Scan_Tran;
};




