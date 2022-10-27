#pragma once
#include "Server_Remote.h"

class c_App_Control : public c_Server_Remote
{
	Q_OBJECT
public:
	explicit c_App_Control(c_Server_Remote *parent = nullptr);
	virtual ~c_App_Control();
	public slots:
	void Run();
};