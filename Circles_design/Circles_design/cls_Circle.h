#ifndef CLS_CIRCLE_H
#define CLS_CIRCLE_H

class QPainter;

class cls_Circle
{
public:
    cls_Circle();
    /*cls_Circle(int p_cx, int p_cy, int p_a, int p_b, double p_angle);*/
    ~cls_Circle();

    void SetParams(int p_cx, int p_cy, int p_a, int p_b, double p_angle);

    void Draw(QPainter* p_painter);

    double GetDistanceWrtCursor(int p_curX, int p_curY);
    double GetAngleWrtCursor(int p_curX, int p_curY);

    void SetAngle(double p_angle) { mAngle = p_angle; }
    void Shift(int p_curX, int p_curY);

private:
    int mCenterX;
    int mCenterY;
    int mA;         // first semi-axis
    int mB;         // second semi-axis
    double mAngle;  // in deg

    // temporary shift of the center point
    int mShiftX;
    int mShiftY;

};

#endif // CLS_CIRCLE_H
