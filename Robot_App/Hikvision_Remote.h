#pragma once
#include "Scan_Remote.h"

class c_Hikvision_Remote : public c_Scan_Remote
{
	Q_OBJECT
public:
	explicit c_Hikvision_Remote(c_Scan_Remote *parent = nullptr);
	virtual ~c_Hikvision_Remote();
	public slots:
	//虚函数的多态
	virtual void Connect();
	virtual void Start();
	virtual void Stop();
};

