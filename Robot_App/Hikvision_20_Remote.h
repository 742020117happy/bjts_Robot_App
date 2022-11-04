#pragma once
#include "Hikvision_Remote.h"

class c_Hikvision_20_Remote : public c_Hikvision_Remote {
	Q_OBJECT

public:
	explicit c_Hikvision_20_Remote(c_Hikvision_Remote * parent = nullptr);
	virtual ~c_Hikvision_20_Remote();

	public slots:
	virtual void Connect();
	
};
