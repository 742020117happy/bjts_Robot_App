#pragma once
#include "Server_Remote.h"

class c_Local_Remote : public c_Server_Remote
{
	Q_OBJECT
public:
	explicit c_Local_Remote(c_Server_Remote *parent = nullptr);
	virtual ~c_Local_Remote();

	public slots:
	void Init();
	virtual void Connect();
	virtual void Connect_Loop();
};
