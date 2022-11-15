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
	int m_Pen_Count = 0;//计数
	QPen m_Hypersen_30_Pen_0_aver_distance;//左面阵雷达ROI_0平均距离画笔
	QPen m_Hypersen_30_Pen_0_max_distance;//左面阵雷达ROI_0最大距离画笔
	QPen m_Hypersen_30_Pen_0_min_distance;//左面阵雷达ROI_0最小距离画笔
	QPen m_Hypersen_30_Pen_0_valid_aver_amp;//左面阵雷达ROI_0有效幅值画笔
	QPen m_Hypersen_30_Pen_0_all_aver_amp;//左面阵雷达ROI_0平均幅值画笔
	QPen m_Hypersen_30_Pen_1_aver_distance;//左面阵雷达ROI_1最平均距离画笔
	QPen m_Hypersen_30_Pen_1_max_distance;//左面阵雷达ROI_1最大距离画笔
	QPen m_Hypersen_30_Pen_1_min_distance;//左面阵雷达ROI_1最小距离画笔
	QPen m_Hypersen_30_Pen_1_valid_aver_amp;//左面阵雷达ROI_1有效幅值画笔
	QPen m_Hypersen_30_Pen_1_all_aver_amp;//左面阵雷达ROI_1平均幅值画笔
	QPen m_Hypersen_31_Pen_0_aver_distance;//右面阵雷达ROI_0最平均距离画笔
	QPen m_Hypersen_31_Pen_0_max_distance;//右面阵雷达ROI_0最大距离画笔
	QPen m_Hypersen_31_Pen_0_min_distance;//右面阵雷达ROI_0最小距离画笔
	QPen m_Hypersen_31_Pen_0_valid_aver_amp;//右面阵雷达ROI_0有效幅值画笔
	QPen m_Hypersen_31_Pen_0_all_aver_amp;//右面阵雷达ROI_0平均幅值画笔
	QPen m_Hypersen_31_Pen_1_aver_distance;//右面阵雷达ROI_1最平均距离画笔
	QPen m_Hypersen_31_Pen_1_max_distance;//右面阵雷达ROI_1最大距离画笔
	QPen m_Hypersen_31_Pen_1_min_distance;//右面阵雷达ROI_1最小距离画笔
	QPen m_Hypersen_31_Pen_1_valid_aver_amp;//右面阵雷达ROI_1有效幅值画笔
	QPen m_Hypersen_31_Pen_1_all_aver_amp;//右面阵雷达ROI_1平均幅值画笔
	bool m_System_Scan = true;
	int m_Current_FPS = 0;//当前帧率
	int m_FPS = 0;//帧率计数

	private slots:
	void System_Scan(QJsonObject db);//系统扫描
};
