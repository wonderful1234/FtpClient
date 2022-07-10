#pragma once
#include <QDialog>
class  PopWidgetBase : public QDialog
{
	Q_OBJECT
public:
	PopWidgetBase(bool moveable, QWidget* parent = nullptr);
	PopWidgetBase(QWidget* parent = nullptr);
protected:
	virtual void keyPressEvent(QKeyEvent *);
};