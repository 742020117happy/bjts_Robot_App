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
	//虚函数的多态
	virtual void Connect();
};
