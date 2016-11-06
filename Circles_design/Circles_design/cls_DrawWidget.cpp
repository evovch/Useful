#include "cls_DrawWidget.h"

#include <iostream>
using std::cout;
using std::endl;

#include <QMouseEvent>
#include <QPainter>

#include "cls_Circle.h"

cls_DrawWidget::cls_DrawWidget(QWidget *parent) : QWidget(parent),
    mCircles(nullptr)
{
    mCurX = 9999;
    mCurY = 9999;

    this->setMouseTracking(true);
}

cls_DrawWidget::~cls_DrawWidget()
{
    if (mCircles) { delete [] mCircles; mCircles = nullptr; }
}

void cls_DrawWidget::resizeEvent(QResizeEvent* /*event*/)
{
    //cout << "Resize" << endl;

    int typicalSize = 30;

    int w = this->width();
    int h = this->height();

    mNx = w/typicalSize;
    mNy = h/typicalSize;

    if (mCircles) { delete [] mCircles; mCircles = nullptr; }
    mCircles = new cls_Circle[mNx*mNy];

    int xStep = w/mNx;
    int yStep = h/mNy;

    for (unsigned int i=0; i<mNx; i++) {
        for (unsigned int j=0; j<mNy; j++) {
            mCircles[i*mNy+j].SetParams(xStep/2+i*xStep, yStep/2+j*yStep, 0.8*xStep/2, 0.8*yStep/2, 0);
//            mCircles[i*mNy+j].SetParams(xStep/2+i*xStep, yStep/2+j*yStep, 10, 50, 0);
        }
    }

    /*repaint();*/ // Called automatically?
}

void cls_DrawWidget::paintEvent(QPaintEvent* /*event*/)
{
    //cout << "Paint" << endl;

    QPainter v_painter(this);

    //v_painter.setPen(Qt::lightGray);
    //int v_PointerSize = 140;
    //v_painter.drawEllipse(mCurX-v_PointerSize/2, mCurY-v_PointerSize/2, v_PointerSize, v_PointerSize);

    for (unsigned int i=0; i<mNx; i++) {
        for (unsigned int j=0; j<mNy; j++) {

            /*double v_dist = mCircles[i*mNy+j].GetDistanceWrtCursor(mCurX, mCurY);
            int w = this->width();
            int h = this->height();
            int v_diag = (w*w+h*h);
            QColor v_color = ValueToColor(1. - v_dist/sqrt((double)v_diag));
            v_painter.setPen(v_color);*/

            mCircles[i*mNy+j].SetAngle(mCircles[i*mNy+j].GetAngleWrtCursor(mCurX, mCurY));
            mCircles[i*mNy+j].Shift(mCurX, mCurY);

            mCircles[i*mNy+j].Draw(&v_painter);
        }
    }
}

void cls_DrawWidget::mousePressEvent(QMouseEvent* /*event*/)
{
    //cout << "Press" << endl;
}

void cls_DrawWidget::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    //cout << "Release" << endl;
}

void cls_DrawWidget::mouseDoubleClickEvent(QMouseEvent* /*event*/)
{
    //cout << "Double click" << endl;
}

void cls_DrawWidget::mouseMoveEvent(QMouseEvent* event)
{
    //cout << "Move" << endl;
    int w = this->width();
    int h = this->height();

    if (event->x() >= 0 && event->x() <= w && event->y() >= 0 && event->y() <= h) {
        mCurX = event->x();
        mCurY = event->y();
        repaint();
    }
}

/*
 * 1.0 - red
 * 0.5 - green
 * 0.0 - blue
 */
QColor cls_DrawWidget::ValueToColor(double p_val)
{
    double r, g, b;

    if (p_val >= 0.0 && p_val < 0.5) {
        r = 0.;
        g = 2.*p_val;
        b = 1. - 2.*p_val;
    }
    if (p_val >= 0.5 && p_val <= 1.0) {
        r = -1. + 2.*p_val;
        g = 2. - 2.*p_val;
        b = 0.;
    }

    return QColor::fromRgb((int)(255.*r), (int)(255.*g), (int)(255.*b));
}
