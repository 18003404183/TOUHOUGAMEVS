#include "EasyxWindow.h"

EasyxWindow::EasyxWindow()
{

}

EasyxWindow::~EasyxWindow()
{

}

bool EasyxWindow::init(int w,int h)
{
	initgraph(w,h);
	return true;
}


