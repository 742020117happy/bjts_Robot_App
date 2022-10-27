#pragma once
#include "Jaka_Remote.h"

class c_Jaka_121_Remote : public c_Jaka_Remote
{
	Q_OBJECT
public:
	explicit c_Jaka_121_Remote(c_Jaka_Remote *parent = nullptr);
	virtual ~c_Jaka_121_Remote();

	public slots:
	//虚函数的多态
	virtual void Connect();
};
