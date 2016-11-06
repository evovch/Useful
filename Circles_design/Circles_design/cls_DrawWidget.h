#ifndef CLS_DRAWWIDGET_H
#define CLS_DRAWWIDGET_H

#include <QWidget>
#include <QColor>

class cls_Circle;

class cls_DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit cls_DrawWidget(QWidget *parent = 0);
    ~cls_DrawWidget();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

signals:

public slots:

private:
    // Current cursor coordinates
    int mCurX;
    int mCurY;

    unsigned int mNx;     // size of the camera in horizontal direction
    unsigned int mNy;     // size of the camera in vertical direction

    cls_Circle* mCircles;

    QColor ValueToColor(double p_val);
};

#endif // CLS_DRAWWIDGET_H
