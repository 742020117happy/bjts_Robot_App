#pragma once
#include "Scan_Remote.h"

class c_Fast_Scan_Remote : public c_Scan_Remote
{
	Q_OBJECT
public:
	explicit c_Fast_Scan_Remote(c_Scan_Remote *parent = nullptr);
	virtual ~c_Fast_Scan_Remote();

	public slots:
	void Init();
	//外部操作接口
	virtual void Connect();
	virtual void Collection();
	virtual void Stop();
};

