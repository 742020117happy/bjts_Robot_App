#pragma once
#include "Public_Header.h"

class c_Fr_Light : public QWidget {
	Q_OBJECT

public:
	enum State {
		Default,
		Error,
		Working
	};
	c_Fr_Light(QWidget * parent = nullptr);
	virtual ~c_Fr_Light();
	bool Current_State();

	public slots:
	void Set_Working();
	void Set_Default();
	void Set_Error();
	void Set_State(bool state);

protected:
	void paintEvent(QPaintEvent *);

private:
	State m_State;
	QColor m_Default_Color;
	QColor m_Error_Color;
	QColor m_Working_Color;
};
