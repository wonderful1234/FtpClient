#include <QtWidgets>
#include "framelesshelper.h"


int CursorPos::m_nBorderWidth = 5;
int CursorPos::m_nTitleHeight = 30;

/***** CursorPos *****/
CursorPos::CursorPos()
{
    reset();
}

void CursorPos::reset()
{
    isOnEdge = false;
    isOnLeftEdge = false;
    isOnRightEdge = false;
    isOnTopEdge = false;
    isOnBottomEdge = false;
    isOnLeftTopCorner = false;
    isOnLeftBottomCorner = false;
    isOnRightTopCorner  = false;
    isOnRightBottomCorner = false;
}

void CursorPos::recalculate(const QPoint &gMousePos, const QRect &frameRect)
{
    int globalMouseX = gMousePos.x();
    int globalMouseY = gMousePos.y();

    int frameX = frameRect.x();
    int frameY = frameRect.y();

    int frameWidth = frameRect.width();
    int frameHeight = frameRect.height();

    isOnLeftEdge = (globalMouseX >= frameX &&
                  globalMouseX <= frameX + m_nBorderWidth );


    isOnRightEdge = (globalMouseX >= frameX + frameWidth - m_nBorderWidth &&
                   globalMouseX <= frameX + frameWidth);

    isOnTopEdge = (globalMouseY >= frameY &&
                 globalMouseY <= frameY + m_nBorderWidth );

    isOnBottomEdge = (globalMouseY >= frameY + frameHeight - m_nBorderWidth &&
                    globalMouseY <= frameY + frameHeight);

    isOnLeftTopCorner = isOnTopEdge && isOnLeftEdge;
    isOnLeftBottomCorner = isOnBottomEdge && isOnLeftEdge;
    isOnRightTopCorner = isOnTopEdge && isOnRightEdge;
    isOnRightBottomCorner = isOnBottomEdge && isOnRightEdge;

    isOnEdge = isOnLeftEdge || isOnRightEdge || isOnTopEdge || isOnBottomEdge;
}

/***** WidgetAction *****/
WidgetAction::WidgetAction(FramelessHelperPrivate *_ptrHelperPriv, QWidget *ptrTopLevelWidget)
{
    ptrHelperPriv = _ptrHelperPriv;
    ptrWidget = ptrTopLevelWidget;
    isLeftButtonPressed = false;
    isCursorShapeChanged = false;
    isLeftButtonPressedTitle = false;
    //ptrRubberBand = NULL;

    windowFlags = ptrWidget->windowFlags();
    ptrWidget->setMouseTracking(true);
    ptrWidget->setAttribute(Qt::WA_Hover, true);

    //updateRubberBandStatus();
}

WidgetAction::~WidgetAction()
{
    ptrWidget->setMouseTracking(false);
    ptrWidget->setWindowFlags(windowFlags);
    ptrWidget->setAttribute(Qt::WA_Hover, false);

    //delete ptrRubberBand;
    //ptrRubberBand = NULL;
}

QWidget* WidgetAction::widget()
{
    return ptrWidget;
}

void WidgetAction::handleWidgetEvent(QEvent *event)
{
    switch (event->type())
    {
    default:
        break;
     case QEvent::MouseButtonDblClick:
        handleMouseDblClickEvent(static_cast<QMouseEvent*>(event));
        break;
    case QEvent::MouseButtonPress:
        handleMousePressEvent(static_cast<QMouseEvent*>(event));
        break;
    case QEvent::MouseButtonRelease:
        handleMouseReleaseEvent(static_cast<QMouseEvent*>(event));
        break;
    case QEvent::MouseMove:
        handleMouseMoveEvent(static_cast<QMouseEvent*>(event));
        break;
    case QEvent::Leave:
        handleLeaveEvent(static_cast<QMouseEvent*>(event));
        break;
    case QEvent::HoverMove:
        handleHoverMoveEvent(static_cast<QHoverEvent*>(event));
        break;
    }
}


void WidgetAction::updateCursorShape(const QPoint &gMousePos)
{
    if (ptrWidget->isFullScreen() || ptrWidget->isMaximized())
    {
        if (isCursorShapeChanged)
        {
            ptrWidget->unsetCursor();
        }
        return;
    }

    MouseMovePos.recalculate(gMousePos, ptrWidget->frameGeometry());

    if(MouseMovePos.isOnLeftTopCorner || MouseMovePos.isOnRightBottomCorner)
    {
        ptrWidget->setCursor( Qt::SizeFDiagCursor );
        isCursorShapeChanged = true;
    }
    else if(MouseMovePos.isOnRightTopCorner || MouseMovePos.isOnLeftBottomCorner)
    {
        ptrWidget->setCursor( Qt::SizeBDiagCursor );
        isCursorShapeChanged = true;
    }
    else if(MouseMovePos.isOnLeftEdge || MouseMovePos.isOnRightEdge)
    {
        ptrWidget->setCursor( Qt::SizeHorCursor );
        isCursorShapeChanged = true;
    }
    else if(MouseMovePos.isOnTopEdge || MouseMovePos.isOnBottomEdge)
    {
        ptrWidget->setCursor( Qt::SizeVerCursor );
        isCursorShapeChanged = true;
    }
    else
    {
        if (isCursorShapeChanged)
        {
            ptrWidget->unsetCursor();
            isCursorShapeChanged = false;
        }
    }
}

void WidgetAction::resizeWidget(const QPoint &gMousePos)
{
    QRect origRect = ptrWidget->frameGeometry();

    int left,top,right,bottom;
    origRect.getCoords(&left, &top, &right, &bottom);

    int minWidth = ptrWidget->minimumWidth();
    int minHeight = ptrWidget->minimumHeight();

    if (MousePressedPos.isOnLeftTopCorner)
    {
        left = gMousePos.x();
        top = gMousePos.y();
    }
    else if (MousePressedPos.isOnLeftBottomCorner)
    {
        left = gMousePos.x();
        bottom = gMousePos.y();
    }
    else if (MousePressedPos.isOnRightTopCorner)
    {
        right = gMousePos.x();
        top = gMousePos.y();
    }
    else if (MousePressedPos.isOnRightBottomCorner)
    {
        right = gMousePos.x();
        bottom = gMousePos.y();
    }
    else if (MousePressedPos.isOnLeftEdge)
    {
        left = gMousePos.x();
    }
    else if (MousePressedPos.isOnRightEdge)
    {
        right = gMousePos.x();
    }
    else if (MousePressedPos.isOnTopEdge)
    {
        top = gMousePos.y();
    }
    else if (MousePressedPos.isOnBottomEdge)
    {
        bottom = gMousePos.y();
    }

    QRect newRect(QPoint(left, top), QPoint(right, bottom));

    if (newRect.isValid())
    {
        if (minWidth > newRect.width())
        {
            if (left != origRect.left())
                newRect.setLeft(origRect.left());
            else
                newRect.setRight(origRect.right());
        }
        if (minHeight > newRect.height())
        {
            if (top != origRect.top())
                newRect.setTop(origRect.top());
            else
                newRect.setBottom(origRect.bottom());
        }
        ptrWidget->setGeometry(newRect);
    }
}

void WidgetAction::moveWidget(const QPoint& gMousePos)
{
    ptrWidget->move(gMousePos - ptInWidgetPos);
 
}

// TBD: MAKE IT CORRECT
void WidgetAction::handleMouseDblClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->pos().y() < MouseMovePos.m_nTitleHeight)
    {
        showMaxRestore();
        xfactor = 1.0;
        yfactor = 1.0;
    }
 }

void WidgetAction::handleMousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isLeftButtonPressed = true;
        xfactor = 1.0;
        yfactor = 1.0;
        isLeftButtonPressedTitle = event->pos().y() < MouseMovePos.m_nTitleHeight;

        QRect frameRect = ptrWidget->frameGeometry();
        MousePressedPos.recalculate(event->globalPos(), frameRect);

        ptInWidgetPos = event->globalPos() - frameRect.topLeft();
        if(ptrWidget->isMaximized()||ptrWidget->isFullScreen())
        {
            xfactor = 1.0*ptInWidgetPos.x()/frameRect.width();
            yfactor = 1.0*ptInWidgetPos.y()/frameRect.height();
        }
    }
}

void WidgetAction::handleMouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isLeftButtonPressed = false;
        isLeftButtonPressedTitle = false;
        MousePressedPos.reset();
        xfactor = 1.0;
        yfactor = 1.0;
    }
}

void WidgetAction::handleMouseMoveEvent(QMouseEvent *event)
{
    if (isLeftButtonPressed)
    {
        if (ptrHelperPriv->isWidgetResizable && MousePressedPos.isOnEdge)
        {
            if(!(ptrWidget->isMaximized()||ptrWidget->isFullScreen()))resizeWidget(event->globalPos());
        }
        else if (ptrHelperPriv->isWidgetMovable && isLeftButtonPressedTitle)
        {
            if(ptrWidget->isMaximized()||ptrWidget->isFullScreen())
            {
                showMaxRestore();
            }
            else
            {
                if(1.0 == xfactor)
                {
                    moveWidget(event->globalPos());
                }
                else
                {
                    QRect frameRect = ptrWidget->frameGeometry();
                    ptInWidgetPos.setX(frameRect.width()*xfactor);
                    moveWidget(event->globalPos());
                    xfactor = 1.0;
                }
            }
        }
    }
    else if (ptrHelperPriv->isWidgetResizable)
    {
        updateCursorShape(event->globalPos());
    }
}

void WidgetAction::handleLeaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if (!isLeftButtonPressed)
    {
        ptrWidget->unsetCursor();
        isCursorShapeChanged = false;
    }
}

void WidgetAction::handleHoverMoveEvent(QHoverEvent *event)
{
    if (ptrHelperPriv->isWidgetResizable)
    {
        updateCursorShape(ptrWidget->mapToGlobal(event->pos()));
    }
}

void WidgetAction::showMaxRestore()
{
   if(ptrWidget->isMaximized()||ptrWidget->isFullScreen())
    {
        ptrWidget->showNormal();
    }
    else
    {
        ptrWidget->showMaximized();
    }    
}


/*****FramelessHelper*****/
FramelessHelper::FramelessHelper(QObject *parent)
    : QObject(parent),
      ptrHelperPriv(new FramelessHelperPrivate())
{
    ptrHelperPriv->isWidgetMovable = true;
    ptrHelperPriv->isWidgetResizable = true;
}

FramelessHelper::~FramelessHelper()
{
    QList<QWidget*> keys = ptrHelperPriv->WidgetActionHash.keys();
    int size = keys.size();
    for (int i = 0; i < size; ++i)
    {
        delete ptrHelperPriv->WidgetActionHash.take(keys[i]);
    }

    delete ptrHelperPriv;
}

bool FramelessHelper::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonDblClick:
		if (!ptrHelperPriv->isWidgetResizable)
		{
			break;
		}
    case QEvent::MouseMove:
    case QEvent::HoverMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::Leave:
    {
        WidgetAction *data = ptrHelperPriv->WidgetActionHash.value(static_cast<QWidget*>(obj));
        if (data)
        {
            data->handleWidgetEvent(event);
            return true;
        }
    }
    }
    return QObject::eventFilter(obj, event);
}

void FramelessHelper::activateOn(QWidget *topLevelWidget)
{
    if (!ptrHelperPriv->WidgetActionHash.contains(topLevelWidget))
    {
        WidgetAction *data = new WidgetAction(ptrHelperPriv, topLevelWidget);
        ptrHelperPriv->WidgetActionHash.insert(topLevelWidget, data);
        topLevelWidget->installEventFilter(this);
    }
}

void FramelessHelper::removeFrom(QWidget *topLevelWidget)
{
    WidgetAction *data = ptrHelperPriv->WidgetActionHash.take(topLevelWidget);
    if (data)
    {
        topLevelWidget->removeEventFilter(this);
        delete data;
    }
}


void FramelessHelper::setWidgetMovable(bool movable)
{
    ptrHelperPriv->isWidgetMovable = movable;
}

void FramelessHelper::setWidgetResizable(bool resizable)
{
    ptrHelperPriv->isWidgetResizable = resizable;
}



void FramelessHelper::setBorderWidth(uint width)
{
    if (width > 0)
    {
        CursorPos::m_nBorderWidth = width;
    }
}

void FramelessHelper::setTitleHeight(uint height)
{
    if (height > 0)
    {
        CursorPos::m_nTitleHeight = height;
    }
}

bool FramelessHelper::widgetMovable()
{
    return ptrHelperPriv->isWidgetMovable;
}

bool FramelessHelper::widgetResizable()
{
    return ptrHelperPriv->isWidgetResizable;
}



uint FramelessHelper::borderWidth()
{
    return CursorPos::m_nBorderWidth;
}

uint FramelessHelper::titleHeight()
{
    return CursorPos::m_nTitleHeight;
}
