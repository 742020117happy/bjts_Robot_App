#pragma once
#include "Public_Header.h"

class c_Radar_Plot : public QWidget {
	Q_OBJECT

public:
	explicit c_Radar_Plot(QWidget * parent = Q_NULLPTR);
	virtual ~c_Radar_Plot();
	public slots:
	void set_Date(QJsonArray len);
protected:
	void paintEvent(QPaintEvent *);
private:
	int m_cylindernum;  //圈数
	double m_lineangle;  //分度
	double m_Axis_max;//量程
	QJsonArray m_length;
	private slots:
	void drawRadar(QPainter *painter);
	void drawScatterPoints(QPainter *painter);

};
