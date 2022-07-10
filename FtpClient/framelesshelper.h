#ifndef FRAMELESS_HELPER_H
#define FRAMELESS_HELPER_H
#include <QObject>
#include <QMouseEvent>
#include <QHoverEvent>

class QWidget;
class FramelessHelperPrivate;
class WidgetAction;

class FramelessHelper : public QObject
{
    Q_OBJECT

public:
    explicit FramelessHelper(QObject *parent = 0);
    ~FramelessHelper();
    // 激活窗体
    void activateOn(QWidget *topLevelWidget);
    // 移除窗体
    void removeFrom(QWidget *topLevelWidget);
    // 设置窗体移动
    void setWidgetMovable(bool movable);
    // 设置窗体缩放
    void setWidgetResizable(bool resizable);
    // 设置橡皮筋移动
   
    // 设置边框的宽度
    void setBorderWidth(uint width);
    // 设置标题栏高度
    void setTitleHeight(uint height);
    bool widgetResizable();
    bool widgetMovable();
  
    uint borderWidth();
    uint titleHeight();

protected:
    // 事件过滤，进行移动、缩放等
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    FramelessHelperPrivate *ptrHelperPriv;
};

/*****
 * FramelessHelperPrivate
 * 存储界面对应的数据集合，以及是否可移动、可缩放属性
*****/
class FramelessHelperPrivate
{
public:
    QHash<QWidget*, WidgetAction*> WidgetActionHash;
    bool isWidgetMovable        : true;
    bool isWidgetResizable      : true;
};

/*****
 * CursorPos
 * 计算鼠标是否位于左、上、右、下、左上角、左下角、右上角、右下角
*****/

class CursorPos
{
public:
    explicit CursorPos();
    void reset();
    void recalculate(const QPoint &globalMousePos, const QRect &frameRect);

public:
    bool isOnEdge                 : true;
    bool isOnLeftEdge             : true;
    bool isOnRightEdge            : true;
    bool isOnTopEdge              : true;
    bool isOnBottomEdge           : true;
    bool isOnLeftTopCorner        : true;
    bool isOnLeftBottomCorner     : true;
    bool isOnRightTopCorner       : true;
    bool isOnRightBottomCorner    : true;

    static int m_nBorderWidth;
    static int m_nTitleHeight;    
};

/*****
 * WidgetAction
 * 更新鼠标样式、移动窗体、缩放窗体
*****/
class WidgetAction
{
public:
    explicit WidgetAction(FramelessHelperPrivate *_ptrHelperPriv, QWidget *ptrTopLevelWidget);
    ~WidgetAction();
    QWidget* widget();
    // 处理鼠标事件-划过、厉害、按下、释放、移动
    void handleWidgetEvent(QEvent *event);

private:
    // 更新鼠标样式
    void updateCursorShape(const QPoint &gMousePos);
    // 重置窗体大小
    void resizeWidget(const QPoint &gMousePos);
    // 移动窗体
    void moveWidget(const QPoint &gMousePos);
    // 处理鼠标按下
    void handleMousePressEvent(QMouseEvent *event);
    // 处理鼠标释放
    void handleMouseReleaseEvent(QMouseEvent *event);
    // 处理鼠标移动
    void handleMouseMoveEvent(QMouseEvent *event);
    // 处理鼠标离开
    void handleLeaveEvent(QEvent *event);
    // 处理鼠标进入
    void handleHoverMoveEvent(QHoverEvent *event);
    void handleMouseDblClickEvent(QMouseEvent *event);
    void showMaxRestore();
private:
    FramelessHelperPrivate *ptrHelperPriv;
 
    QWidget *ptrWidget;
    QPoint   ptInWidgetPos;
    CursorPos MousePressedPos;
    CursorPos MouseMovePos;
    bool isLeftButtonPressed;
    bool isCursorShapeChanged;
    bool isLeftButtonPressedTitle;
    Qt::WindowFlags windowFlags;

    double xfactor;
    double yfactor;
};



#endif //FRAMELESS_HELPER_H
