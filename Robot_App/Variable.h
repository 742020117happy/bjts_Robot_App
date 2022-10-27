﻿#pragma once
#include "Public_Header.h"

/*************************************************************************************************************************************************
**Function:系统全局变量与接口
*************************************************************************************************************************************************/
class c_Variable : public QObject
{
	Q_OBJECT
public:
	explicit c_Variable(QObject *parent = nullptr);
	virtual ~c_Variable();
	static QJsonObject g_Communicate_DB;
	public slots:
	static float Short_To_Float(quint16 high, quint16 low);
	static QString TCP_Status(int State);
	static QString Modbus_Status(int State);
	static bool msleep(const int mSec);
private:
	bool exist;
	QDir System_Dir;
	QFile Communicate_DB;
};
