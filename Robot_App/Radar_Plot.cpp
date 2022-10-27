#pragma execution_character_set("utf-8")
#include "Radar_Plot.h"

c_Radar_Plot::c_Radar_Plot(QWidget * parent) : QWidget(parent) {
	m_cylindernum = 10;  //圈数
	m_lineangle = 0.333; //分辨率
	m_Axis_max = 2000;//量程
}

c_Radar_Plot::~c_Radar_Plot() {
	
}

void c_Radar_Plot::set_Date(QJsonArray len)
{
	m_length = len;
	update();
}
void c_Radar_Plot::paintEvent(QPaintEvent *)
{
	QPainter *painter = new QPainter(this);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	painter->setRenderHint(QPainter::TextAntialiasing);
	drawRadar(painter);
	drawScatterPoints(painter);
	painter->end();
}

void c_Radar_Plot::drawRadar(QPainter *painter)
{
	painter->save();
	QPen pen;
	QColor color(Qt::lightGray);
	pen.setWidthF(0.4);
	painter->setPen(pen);

	QPainterPath looppath;
	int w = width();//图款宽
	int h = height(); //图款长
	int count = m_cylindernum;//圈数
	int diameter = qMin(w, h);//选较小参数作为半径
	int step = diameter / count ;  //步长，加1是为了四周留出空间，写标签
	int x = 0;
	int y = 0;//矩形顶点

	QRectF outrect;
	for (int i = 1; i < count + 1; ++i)
	{
		diameter = step * i;
		x = w / 2 - diameter / 2;
		y = h / 2 - diameter / 2;
		QRectF rect(x, y, diameter, diameter);
		looppath.addEllipse(rect);
		outrect = rect;
	}
	painter->drawPath(looppath);

	if (m_lineangle == 0)
	{
		m_lineangle = 90;
	}
	int linecount = 360 / m_lineangle;
	int x0 = w / 2;
	int y0 = h / 2;
	int newdiameter = outrect.height() / 2;
	for (int i = 0; i < linecount; ++i)
	{
		int x1 = x0 + newdiameter*qSin(PI * 2 / linecount*i);
		int y1 = y0 + newdiameter*qCos(PI * 2 / linecount*i);
		painter->drawLine(x0, y0, x1, y1);
	}
	painter->restore();
}

void c_Radar_Plot::drawScatterPoints(QPainter *painter)
{
	painter->save();
	int w = width();
	int h = height();
	int diameter = qMin(w, h);
	double R = diameter / m_Axis_max;

	QColor color = QColor("red");
	painter->setPen(color.darker(200));
	painter->setBrush(color.darker(200));

	for (int i = 0; i < m_length.size(); i++)
	{
		int length = m_length.at(i).toInt();
		double angle = i * m_lineangle;
		double x_length = w / 2 + R * (length * qCos(angle * PI / 180));
		double y_length = h / 2 - R * (length * qSin(angle * PI / 180));
		QPointF certp(x_length, y_length);
		painter->drawEllipse(certp, 2, 2);
	}
	painter->restore();
}
