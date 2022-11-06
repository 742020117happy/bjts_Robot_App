#pragma once
#include "Server_Remote.h"

class c_Local_Monitor : public c_Server_Remote
{
	Q_OBJECT
public:
	explicit c_Local_Monitor(c_Server_Remote *parent = nullptr);
	virtual ~c_Local_Monitor();

	public slots:
	void Init();
	virtual void Connect_Loop();
};