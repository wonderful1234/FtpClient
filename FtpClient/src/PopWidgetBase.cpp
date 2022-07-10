#include "PopWidgetBase.h"
#include "framelesshelper.h"

PopWidgetBase::PopWidgetBase(bool moveable, QWidget * parent)
	:QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setWindowModality(Qt::ApplicationModal);
	if (moveable)
	{
		FramelessHelper *pHelper = new FramelessHelper(this);
		pHelper->activateOn(this);
		pHelper->setWidgetResizable(false);
		pHelper->setBorderWidth(1);
		pHelper->setTitleHeight(45);
		pHelper->setWidgetMovable(true);
	}
}

PopWidgetBase::PopWidgetBase(QWidget* parent)
	:PopWidgetBase(true, parent)
{
}

void PopWidgetBase::keyPressEvent(QKeyEvent *)
{//不写这个按回车窗口会消失
	return;
}
