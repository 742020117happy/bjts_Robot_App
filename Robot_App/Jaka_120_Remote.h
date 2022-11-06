#pragma once
#include "Jaka_Remote.h"

class c_Jaka_120_Remote : public c_Jaka_Remote
{
	Q_OBJECT
public:
	explicit c_Jaka_120_Remote(c_Jaka_Remote *parent = nullptr);
	virtual ~c_Jaka_120_Remote();
	public slots:
	//虚函数的多态
	void Init();
	virtual void Connect();
	virtual void Connect_Loop();//循环检测连接状态
};
