#pragma once
#include "Hikvision_Remote.h"

class c_Hikvision_21_Remote : public c_Hikvision_Remote {
	Q_OBJECT

public:
	explicit c_Hikvision_21_Remote(c_Hikvision_Remote * parent = nullptr);
	virtual ~c_Hikvision_21_Remote();

	public slots:
	void Init();
	virtual void Connect();
	virtual void Connect_Loop();//循环连接
};
