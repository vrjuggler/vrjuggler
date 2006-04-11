#ifndef _VRJ_VEC4_H
#define _VRJ_VEC4_H

#include <gmtl/Math.h>
#include <gmtl/Vec.h>

namespace vrj
{

class Vec4 : public gmtl::Vec4f
{
public:
  Vec4()
  {
    set(0, 0, 0, 0);
  }

  Vec4(float x, float y, float z, float w)
  {
    set(x, y, z, w);
  }

  Vec4(const Vec4& v)
  {
    set(v[0], v[1], v[2], v[3]);
  }

  void set(float x, float y, float z, float w)
  {
    mData[0] = x;
    mData[1] = y;
    mData[2] = z;
    mData[3] = w;
  }

  bool equal(const Vec4& v) const
  {
    return (mData[0] == v[0] &&
            mData[1] == v[1] &&
            mData[2] == v[2] &&
            mData[3] == v[3]);
  }

  float dot(const Vec4& v) const
  {
    // 1.0 juggler ignores v[3] ?
    return (mData[0] * v[0] +
            mData[1] * v[1] +
            mData[2] * v[2]);
  }

  float length() const
  {
    return gmtl::Math::sqrt(
      mData[0] * mData[0] +
      mData[1] * mData[1] +
      mData[2] * mData[2] +
      mData[3] * mData[3]);
  }

  void normalize()
  {
    float len = 1 / length();
    mData[0] *= len;
    mData[1] *= len;
    mData[2] *= len;
    mData[3] *= len;
  }

  void xform(const vjMatrix& m, const Vec4& v)
  {
    set(0, 0, 0, 0);
    for (int j = 0; j < 4; ++j)
      for (int k = 0; k < 4; ++k)
        mData[j] += v[k] * m(j, k);
  }

  int operator==(const Vec4& v) const
  {
    return (mData[0] == v[0] &&
            mData[1] == v[1] &&
            mData[2] == v[2] &&
            mData[3] == v[3]);
  }

  int operator!=(const Vec4& v) const
  {
    return !(*this == v);
  }

  Vec4 operator-() const
  {
    return Vec4(-mData[0], -mData[1], -mData[2], -mData[3]);
  }

  Vec4 operator+(const Vec4& v) const
  {
    return Vec4(mData[0] + v[0],
                mData[1] + v[1],
                mData[2] + v[2],
                mData[3] + v[3]);
  }

  Vec4 operator-(const Vec4& v) const
  {
    return Vec4(mData[0] - v[0],
                mData[1] - v[1],
                mData[2] - v[2],
                mData[3] - v[3]);
  }

  Vec4& operator=(const Vec4& v)
  {
    mData[0] = v[0];
    mData[1] = v[2];
    mData[2] = v[3];
    mData[3] = v[3];
    return *this;
  }

  Vec4& operator*=(float s)
  {
    mData[0] *= s;
    mData[1] *= s;
    mData[2] *= s;
    mData[3] *= s;
    return *this;
  }

  Vec4& operator/=(float s)
  {
    s = 1 / s;
    return (*this *= s);
  }

  Vec4& operator+=(const Vec4& v)
  {
    mData[0] += v[0];
    mData[1] += v[1];
    mData[2] += v[2];
    mData[3] += v[3];
    return *this;
  }

  Vec4& operator-=(const Vec4& v)
  {
    mData[0] -= v[0];
    mData[1] -= v[1];
    mData[2] -= v[2];
    mData[3] -= v[3];
    return *this;
  }
};

inline Vec4 operator*(float s, const Vec4& v)
{
  return Vec4(s * v[0], s * v[1], s * v[2], s * v[3]);
}

inline Vec4 operator*(const Vec4& v, float s)
{
  return Vec4(s * v[0], s * v[1], s * v[2], s * v[3]);
}

inline Vec4 operator/(const Vec4& v, float s)
{
  return v * (1 / s);
}

inline std::ostream& operator<<(std::ostream& out, Vec4& v)
{
  out << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3];
  return out;
}

}

#endif
