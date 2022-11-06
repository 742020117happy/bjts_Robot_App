#pragma once
#include "Meijidenki_Remote.h"

class c_Meijidenki_20_Remote : public c_Meijidenki_Remote
{
	Q_OBJECT
public:
	explicit c_Meijidenki_20_Remote(c_Meijidenki_Remote *parent = nullptr);
	virtual ~c_Meijidenki_20_Remote();
	public slots:
	//虚函数的多态
	void Init();
	virtual void Connect();
	virtual void Connect_Loop();//循环连接
	void Meijidenki_20_Read_Ready();
private:
	qint32 m_device_id = 99;
};

