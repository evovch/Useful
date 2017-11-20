#ifndef SUPPORT_H
#define SUPPORT_H

#define PI 3.14159265358979323846

#include <cmath>

// return radians - same way as asin() of <cmath>
double myAsin(double sinAlpha, double cosAlpha)
{
  if (sinAlpha > 0.) {
    if (cosAlpha > 0.) {
      return asin(sinAlpha);
    } else {
      return PI - asin(sinAlpha);
    }
  } else {
    if (cosAlpha > 0.) {
      return 2.*PI + asin(sinAlpha);
    } else {
      return PI - asin(sinAlpha);
    }
  }
}

#endif // SUPPORT_H
