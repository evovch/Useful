#include "cls_cylinder.h"

#include "cls_axis2_placement_3d.h"

cls_cylinder::cls_cylinder()
    : mMatrixBuilt(false)
{
}

cls_cylinder::~cls_cylinder()
{
}

void cls_cylinder::BuildMatrix(void)
{
    // Transformation matrix
    mM.data[0]  = mPosition->mXaxis.fPx;
    mM.data[4]  = mPosition->mXaxis.fPy;
    mM.data[8]  = mPosition->mXaxis.fPz;
    mM.data[1]  = mPosition->mYaxis.fPx;
    mM.data[5]  = mPosition->mYaxis.fPy;
    mM.data[9]  = mPosition->mYaxis.fPz;
    mM.data[2]  = mPosition->mAxis.fPx;
    mM.data[6]  = mPosition->mAxis.fPy;
    mM.data[10] = mPosition->mAxis.fPz;
    mM.data[3]  = mPosition->mLocation.fX;
    mM.data[7]  = mPosition->mLocation.fY;
    mM.data[11] = mPosition->mLocation.fZ;

    mMinv = mM;
    mMinv.Inverse();

    mMatrixBuilt = true;
}

unsigned int cls_cylinder::Intersect(point_t p_start, direction_t p_ray, point_t& inter1, point_t& inter2)
{
    if (!mMatrixBuilt) this->BuildMatrix();

    point_t pTrackStart2(0., 0., 0.); // in local CS
    direction_t dTrackDir2; // in local CS

    // Transform track start point into local CS
    pTrackStart2 = p_start;
    pTrackStart2.MultiplyLeft(mM);
    // Transform track direction into local CS
    dTrackDir2 = p_ray;
    dTrackDir2.MultiplyLeft(mM);

    double l = dTrackDir2.fPx;
    double m = dTrackDir2.fPy;
    double n = dTrackDir2.fPz;
    double x0 = pTrackStart2.fX;
    double y0 = pTrackStart2.fY;
    double z0 = pTrackStart2.fZ;

    double a = l*l + m*m;
    double b = 2.*(x0*l+y0*m);
    double c = x0*x0 + y0*y0 - mR*mR;
    double D = b*b - 4.*a*c;
    double sqrtD = sqrt(D);

    ////unsigned int nRoots = 0;
    unsigned int nInters = 0;

    double t1 = -DBL_MAX; // solution 1
    double t2 = -DBL_MAX; // solution 2

    if (D == 0.) { //TODO check tolerance
        ////nRoots = 1;
        t1 = -b / (2.*a);

        if (t1>=0. || t1<=1.) {
            inter1.fX = l*t1 + x0;
            inter1.fY = m*t1 + y0;
            inter1.fZ = n*t1 + z0;

            // Transform
            inter1.MultiplyLeft(mMinv);

            inter1 += pTrackStart2;

            nInters++;
        }
    } else if (D > 0.) {
        ////nRoots = 2;
        t1 = (-b + sqrtD) / (2.*a);
        t2 = (-b - sqrtD) / (2.*a);

        if (t1>=0. || t1<=1.) {
            inter1.fX = l*t1 + x0;
            inter1.fY = m*t1 + y0;
            inter1.fZ = n*t1 + z0;

            // Transform
            inter1.MultiplyLeft(mMinv);

            inter1 += pTrackStart2;

            nInters++;
        }

        if (t2>=0. || t2<=1.) {
            inter2.fX = l*t2 + x0;
            inter2.fY = m*t2 + y0;
            inter2.fZ = n*t2 + z0;

            // Transform
            inter2.MultiplyLeft(mMinv);

            inter2 += pTrackStart2;

            nInters++;
        }
    }

	return nInters;
}
