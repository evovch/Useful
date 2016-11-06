#include "cls_Circle.h"

#include <QPainter>

const double RADtoDEG = 180./3.14159265358979;
const double DEGtoRAG = 3.14159265358979/180.;

cls_Circle::cls_Circle() :
    mCenterX(100),
    mCenterY(100),
    mA(50),
    mB(50),
    mAngle(0.),
    mShiftX(0),
    mShiftY(0)
{
}

/*
cls_Circle::cls_Circle(int p_cx, int p_cy, int p_a, int p_b, double p_angle) :
    mCenterX(p_cx),
    mCenterY(p_cy),
    mA(p_a),
    mB(p_b),
    mAngle(p_angle),
    mShiftX(0),
    mShiftY(0)
{
}
*/

cls_Circle::~cls_Circle()
{
}

void cls_Circle::SetParams(int p_cx, int p_cy, int p_a, int p_b, double p_angle)
{
    mCenterX = p_cx;
    mCenterY = p_cy;
    mA = p_a;
    mB = p_b;
    mAngle = p_angle;
    mShiftX = 0;
    mShiftY = 0;
}

void cls_Circle::Draw(QPainter* p_painter)
{
    p_painter->translate(mCenterX+mShiftX, mCenterY+mShiftY);
    p_painter->rotate(-mAngle);
    p_painter->drawEllipse(-mA, -mB, 2*mA, 2*mB);

    p_painter->drawEllipse(-mA*0.4, -mB*0.9, 2*mA*0.4, 2*mB*0.9);

    p_painter->rotate(mAngle);
    p_painter->translate(-(mCenterX+mShiftX), -(mCenterY+mShiftY));
}

double cls_Circle::GetDistanceWrtCursor(int p_curX, int p_curY)
{
    int tmp = (p_curX-mCenterX)*(p_curX-mCenterX) + (p_curY-mCenterY)*(p_curY-mCenterY);
    return sqrt((double)tmp);
}

double cls_Circle::GetAngleWrtCursor(int p_curX, int p_curY)
{
    // Special cases
    if (mCenterX == p_curX && mCenterY == p_curY) return 0.;
    if (mCenterX > p_curX && mCenterY == p_curY) return 0.;
    if (mCenterX < p_curX && mCenterY == p_curY) return 180.;
    if (mCenterX == p_curX && mCenterY > p_curY) return 270.;
    if (mCenterX == p_curX && mCenterY < p_curY) return 90.;

    double dx, dy;
    double v_angle;

    if (mCenterX > p_curX) {
        dx = (double)(mCenterX-p_curX);

        if (mCenterY > p_curY) {                // 4 quarter
            dy = (double)(mCenterY-p_curY);
            v_angle = 360. - RADtoDEG*atan(dy/dx);
        } else if (mCenterY < p_curY) {         // 1 quarter
            dy = (double)(-mCenterY+p_curY);
            v_angle = RADtoDEG*atan(dy/dx);
        }
    } else if (mCenterX < p_curX) {
        dx = (double)(-mCenterX+p_curX);

        if (mCenterY > p_curY) {                // 3 quarter
            dy = (double)(mCenterY-p_curY);
            v_angle = 180. + RADtoDEG*atan(dy/dx);
        } else if (mCenterY < p_curY) {         // 2 quarter
            dy = (double)(-mCenterY+p_curY);
            v_angle = 180. - RADtoDEG*atan(dy/dx);
        }
    }

    return v_angle;
}

void cls_Circle::Shift(int p_curX, int p_curY)
{
    double v_tilt = GetAngleWrtCursor(p_curX, p_curY);
    double v_dist = GetDistanceWrtCursor(p_curX, p_curY);

    // Special cases
    //TODO implement


    double areaSize = 1200.;
    double magn = 18.;

    if (v_dist > areaSize) {
        mShiftX = 0;
        mShiftY = 0;
        return;
    }

    mShiftX = (magn - (magn/areaSize)*v_dist)*cos(v_tilt*DEGtoRAG);
    mShiftY = -(magn - (magn/areaSize)*v_dist)*sin(v_tilt*DEGtoRAG);
/*
    if (v_tilt > 0. && v_tilt < 90.) {              // 1 quarter

    }
    else if (v_tilt > 90. && v_tilt < 180.) {        // 2 quarter

    }
    else if (v_tilt > 180. && v_tilt < 270.) {       // 3 quarter

    }
    else if (v_tilt > 270. && v_tilt < 360.) {       // 4 quarter

    }
*/
}
