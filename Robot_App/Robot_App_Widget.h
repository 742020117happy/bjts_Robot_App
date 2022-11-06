#pragma once
#include "Ui_Robot_App_Widget.h"
#include "Public_Header.h"
#include "Thread.h"


class c_Robot_App_Widget : public QMainWindow
{
	Q_OBJECT
public:
	explicit c_Robot_App_Widget(QWidget * parent = nullptr);
	virtual ~c_Robot_App_Widget();
	c_Thread *m_Thread;//子类线程
	QElapsedTimer *m_Time;//计时器

	public slots:
	void keyPressEvent(QKeyEvent *event);

signals:
	void System_Scan_Done();

private:
	Ui_Robot_App_Widget *ui;
	QString m_Current_Time = "";//系统时间
	int m_Current_FPS = 0;//当前帧率
	int m_FPS = 0;//帧率计数

	private slots:
	void System_Scan(QJsonObject db);//系统扫描
};
