#pragma once
#include "Hypersen_Remote.h"

class c_Hypersen_30_Remote : public c_Hypersen_Remote
{
	Q_OBJECT
public:
	explicit c_Hypersen_30_Remote(c_Hypersen_Remote *parent = nullptr);
	virtual ~c_Hypersen_30_Remote();
	public slots:
	//虚函数的多态
	virtual void Connect();
	void Hypersen_30_Read_Ready();
};

