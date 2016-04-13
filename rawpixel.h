#ifndef RAW_PIXEL_H
#define RAW_PIXEL_H

#include "EMConverter.h"

class RawPixel
{
public:
  RawPixel(double raw_pixel = 0);

  int CP(EMConverter::Ptr const&) const;
  int CP(double scale) const;

  operator int() const;

private:
  double raw_pixel_;
};

// User-Defined Literals (ex: 10_em, 10.0_em)
RawPixel operator"" _em(long double pixel);
RawPixel operator"" _em(unsigned long long pixel);

#endif // RAW_PIXEL_H
