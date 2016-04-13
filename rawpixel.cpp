#include <cmath>
#include "rawpixel.h"

RawPixel operator"" _em(long double pixel)
{
  return RawPixel(pixel);
}

RawPixel operator"" _em(unsigned long long pixel)
{
  return RawPixel(pixel);
}

RawPixel::RawPixel(double raw_pixel)
  : raw_pixel_(raw_pixel)
{}

int RawPixel::CP(EMConverter::Ptr const& converter) const
{
  return converter->CP(raw_pixel_);
}

int RawPixel::CP(double scale) const
{
  return std::round(raw_pixel_ * scale);
}

RawPixel::operator int() const
{
  return std::round(raw_pixel_);
}
