#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include <opencv2/opencv.hpp>
class QtWidgetsApplication1 : public QMainWindow
{
	Q_OBJECT
public:
	QtWidgetsApplication1(QWidget* parent = nullptr);
	~QtWidgetsApplication1();
public:
	int m_tWindowWidth;
	int m_tWindowHeight;
	int m_tWindowX;
	int m_tWindowY;
	QString hv_font_1;
	int tWWindowID;
	int hv_WindowHandle;
	long Row;
	long Column;
	double Radius;
	double StartPhi;
	double EndPhi;
	double PointOrder;
	double Row1;
	double Column1;
	double Row2;
	double Column2;
	double Diameter;

private slots:
	void Read_Image();
	void inspect();
	void next_image();
	void setwindow();	
private:
	Ui::QtWidgetsApplication1Class ui;
};
