#pragma once
#include "Jaka_Monitor.h"

//5120629
class c_Jaka_120_Monitor : public c_Jaka_Monitor
{
	Q_OBJECT
public:
	explicit c_Jaka_120_Monitor(c_Jaka_Monitor *parent = nullptr);
	virtual ~c_Jaka_120_Monitor();
	public slots:
	void Init();
	//虚函数的多态
	virtual void Connect();
	virtual void Connect_Loop();//循环检测连接状态
};
