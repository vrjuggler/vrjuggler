// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _VRJ_NET_GMTL_WRAPPERS_H_
#define _VRJ_NET_GMTL_WRAPPERS_H_

#include <gmtl/Generate.h>


namespace gmtlWrappers
{
   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 3, 3> makeRotMatrix33(const gmtl::Quat<DATA_TYPE>& q)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 3, 3> >(q);
   }

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeRotMatrix44(const gmtl::Quat<DATA_TYPE>& q)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 4, 4> >(q);
   }

   template<typename DATA_TYPE, typename ROT_ORDER>
   gmtl::Matrix<DATA_TYPE, 3, 3> makeRotMatrix33(const gmtl::EulerAngle<DATA_TYPE, ROT_ORDER>& e)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 3, 3> >(e);
   }

   template<typename DATA_TYPE, typename ROT_ORDER>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeRotMatrix44(const gmtl::EulerAngle<DATA_TYPE, ROT_ORDER>& e)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 4, 4> >(e);
   }

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeRotMatrix44(const gmtl::AxisAngle<DATA_TYPE>& a)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 4, 4> >(a);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::Quat<DATA_TYPE> makeRotQuat(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeRot< gmtl::Quat<DATA_TYPE> >(m);
   }

   template<typename DATA_TYPE>
   gmtl::Quat<DATA_TYPE> makeRotQuat(const gmtl::AxisAngle<DATA_TYPE>& a)
   {
      return gmtl::makeRot< gmtl::Quat<DATA_TYPE> >(a);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::EulerAngle<DATA_TYPE, gmtl::XYZ> makeRotEulerAngleXYZ(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeRot< gmtl::EulerAngle<DATA_TYPE, gmtl::XYZ> >(m);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::EulerAngle<DATA_TYPE, gmtl::ZYX> makeRotEulerAngleZYX(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeRot< gmtl::EulerAngle<DATA_TYPE, gmtl::ZYX> >(m);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::EulerAngle<DATA_TYPE, gmtl::ZXY> makeRotEulerAngleZXY(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeRot< gmtl::EulerAngle<DATA_TYPE, gmtl::ZXY> >(m);
   }

   template gmtl::Matrix33f      makeRotMatrix33(const gmtl::Quatf&);
   template gmtl::Matrix33d      makeRotMatrix33(const gmtl::Quatd&);
   template gmtl::Matrix33f      makeRotMatrix33(const gmtl::EulerAngleXYZf&);
   template gmtl::Matrix33f      makeRotMatrix33(const gmtl::EulerAngleZYXf&);
   template gmtl::Matrix33f      makeRotMatrix33(const gmtl::EulerAngleZXYf&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::Quatf&);
   template gmtl::Matrix44d      makeRotMatrix44(const gmtl::Quatd&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::EulerAngleXYZf&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::EulerAngleZYXf&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::EulerAngleZXYf&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::AxisAnglef&);
   template gmtl::Matrix44d      makeRotMatrix44(const gmtl::AxisAngled&);
   template gmtl::Quatf          makeRotQuat(const gmtl::Matrix44f&);
   template gmtl::Quatd          makeRotQuat(const gmtl::Matrix44d&);
   template gmtl::Quatf          makeRotQuat(const gmtl::AxisAnglef&);
   template gmtl::Quatd          makeRotQuat(const gmtl::AxisAngled&);
   template gmtl::EulerAngleXYZf makeRotEulerAngleXYZ(const gmtl::Matrix44f&);
   template gmtl::EulerAngleZYXf makeRotEulerAngleZYX(const gmtl::Matrix44f&);
   template gmtl::EulerAngleZXYf makeRotEulerAngleZXY(const gmtl::Matrix44f&);
   template gmtl::EulerAngleXYZd makeRotEulerAngleXYZ(const gmtl::Matrix44d&);
   template gmtl::EulerAngleZYXd makeRotEulerAngleZYX(const gmtl::Matrix44d&);
   template gmtl::EulerAngleZXYd makeRotEulerAngleZXY(const gmtl::Matrix44d&);

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeRotMatrix44(const gmtl::Vec<DATA_TYPE, 3>& from,
                                                 const gmtl::Vec<DATA_TYPE, 3>& to)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 4, 4> >(from, to);
   }

   template<typename DATA_TYPE>
   gmtl::Quat<DATA_TYPE> makeRotQuat(const gmtl::Vec<DATA_TYPE, 3>& from,
                                     const gmtl::Vec<DATA_TYPE, 3>& to)
   {
      return gmtl::makeRot< gmtl::Quat<DATA_TYPE> >(from, to);
   }

   template gmtl::Matrix44f makeRotMatrix44(const gmtl::Vec3f&,
                                            const gmtl::Vec3f&);
   template gmtl::Quatf     makeRotQuat(const gmtl::Vec3f&, const gmtl::Vec3f&);
   template gmtl::Quatd     makeRotQuat(const gmtl::Vec3d&, const gmtl::Vec3d&);

#ifndef _MSC_VER
   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 3, 3> makeDirCosMatrix33(const gmtl::Vec<DATA_TYPE, 3>& xDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& yDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& zDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& xSrcAxis = gmtl::Vec<DATA_TYPE, 3>(1, 0, 0),
                                                    const gmtl::Vec<DATA_TYPE, 3>& ySrcAxis = gmtl::Vec<DATA_TYPE, 3>(0, 1, 0),
                                                    const gmtl::Vec<DATA_TYPE, 3>& zSrcAxis = gmtl::Vec<DATA_TYPE, 3>(0, 0, 1))
   {
      return gmtl::makeDirCos< gmtl::Matrix<DATA_TYPE, 3, 3> >(xDestAxis,
                                                               yDestAxis,
                                                               zDestAxis,
                                                               xSrcAxis,
                                                               ySrcAxis,
                                                               zSrcAxis);
   }

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeDirCosMatrix44(const gmtl::Vec<DATA_TYPE, 3>& xDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& yDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& zDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& xSrcAxis = gmtl::Vec<DATA_TYPE, 3>(1, 0, 0),
                                                    const gmtl::Vec<DATA_TYPE, 3>& ySrcAxis = gmtl::Vec<DATA_TYPE, 3>(0, 1, 0),
                                                    const gmtl::Vec<DATA_TYPE, 3>& zSrcAxis = gmtl::Vec<DATA_TYPE, 3>(0, 0, 1))
   {
      return gmtl::makeDirCos< gmtl::Matrix<DATA_TYPE, 4, 4> >(xDestAxis,
                                                               yDestAxis,
                                                               zDestAxis,
                                                               xSrcAxis,
                                                               ySrcAxis,
                                                               zSrcAxis);
   }

   template gmtl::Matrix33f makeDirCosMatrix33(const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&);
   template gmtl::Matrix33d makeDirCosMatrix33(const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&);
   template gmtl::Matrix44f makeDirCosMatrix44(const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&);
   template gmtl::Matrix44d makeDirCosMatrix44(const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&);
#endif


   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeTransMatrix44(const gmtl::Vec<DATA_TYPE, 3>& v)
   {
      return gmtl::makeTrans< gmtl::Matrix<DATA_TYPE, 4, 4> >(v);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::Vec<DATA_TYPE, 3> makeTransVec3(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeTrans< gmtl::Vec<DATA_TYPE, 3> >(m);
   }

   template gmtl::Matrix44f makeTransMatrix44(const gmtl::Vec3f&);
   template gmtl::Matrix44d makeTransMatrix44(const gmtl::Vec3d&);
   template gmtl::Vec3f     makeTransVec3(const gmtl::Matrix44f&);
   template gmtl::Vec3d     makeTransVec3(const gmtl::Matrix44d&);

   template<typename DATA_TYPE, unsigned SIZE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeScaleMatrix44(const gmtl::Vec<DATA_TYPE, SIZE>& scale)
   {
      return gmtl::makeScale< gmtl::Matrix<DATA_TYPE, 4, 4> >(scale);
   }

   template gmtl::Matrix44f makeScaleMatrix44<float, 3>(const gmtl::Vec3f&);
   template gmtl::Matrix44d makeScaleMatrix44<double, 3>(const gmtl::Vec3d&);
   template gmtl::Matrix44f makeScaleMatrix44<float, 4>(const gmtl::Vec4f&);
   template gmtl::Matrix44d makeScaleMatrix44<double, 4>(const gmtl::Vec4d&);

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeScaleMatrix44(const DATA_TYPE& scale)
   {
      return gmtl::makeScale< gmtl::Matrix<DATA_TYPE, 4, 4> >(scale);
   }

   template gmtl::Matrix44f makeScaleMatrix44<float>(const float&);
   template gmtl::Matrix44d makeScaleMatrix44<double>(const double&);

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeAxesMatrix44(const gmtl::Vec<DATA_TYPE, 3>& xAxis,
                                                  const gmtl::Vec<DATA_TYPE, 3>& yAxis,
                                                  const gmtl::Vec<DATA_TYPE, 3>& zAxis)
   {
      return gmtl::makeAxes< gmtl::Matrix<DATA_TYPE, 4, 4> >(xAxis, yAxis,
                                                             zAxis);
   }

   template gmtl::Matrix44f makeAxesMatrix44(const gmtl::Vec3f&,
                                             const gmtl::Vec3f&,
                                             const gmtl::Vec3f&);
   template gmtl::Matrix44d makeAxesMatrix44(const gmtl::Vec3d&,
                                             const gmtl::Vec3d&,
                                             const gmtl::Vec3d&);

   // XXX: This is pretty annoying.  Visual C++ is not very good at deducing
   // types, I guess.
#ifdef _MSC_VER
   template<typename T, typename DATA_TYPE>
   void setArrayElement(T* obj, const unsigned i, typename DATA_TYPE value)
#else
   template<typename T>
   void setArrayElement(T* obj, const unsigned i, typename T::DataType value)
#endif
   {
      (*obj)[i] = value;
   }

   template void setArrayElement(gmtl::VecBase<int, 2>*, const unsigned, int);
   template void setArrayElement(gmtl::VecBase<float, 2>*, const unsigned, float);
   template void setArrayElement(gmtl::VecBase<double, 2>*, const unsigned, double);
   template void setArrayElement(gmtl::VecBase<int, 3>*, const unsigned, int);
   template void setArrayElement(gmtl::VecBase<float, 3>*, const unsigned, float);
   template void setArrayElement(gmtl::VecBase<double, 3>*, const unsigned, double);
   template void setArrayElement(gmtl::VecBase<int, 4>*, const unsigned, int);
   template void setArrayElement(gmtl::VecBase<float, 4>*, const unsigned, float);
   template void setArrayElement(gmtl::VecBase<double, 4>*, const unsigned, double);
   template void setArrayElement(gmtl::Vec3f*, const unsigned, float);
   template void setArrayElement(gmtl::Vec3d*, const unsigned, double);
   template void setArrayElement(gmtl::Vec4f*, const unsigned, float);
   template void setArrayElement(gmtl::Vec4d*, const unsigned, double);
   template void setArrayElement(gmtl::Quatf*, const unsigned, float);
   template void setArrayElement(gmtl::Quatd*, const unsigned, double);
   template void setArrayElement(gmtl::Point3f*, const unsigned, float);
   template void setArrayElement(gmtl::Point3d*, const unsigned, double);
   template void setArrayElement(gmtl::Matrix33f::RowAccessor*, const unsigned, float);
   template void setArrayElement(gmtl::Matrix44f::RowAccessor*, const unsigned, float);
   template void setArrayElement(gmtl::EulerAngleXYZf*, const unsigned, float);
   template void setArrayElement(gmtl::EulerAngleXYZd*, const unsigned, double);
   template void setArrayElement(gmtl::EulerAngleZYXf*, const unsigned, float);
   template void setArrayElement(gmtl::EulerAngleZYXd*, const unsigned, double);
   template void setArrayElement(gmtl::EulerAngleZXYf*, const unsigned, float);
   template void setArrayElement(gmtl::EulerAngleZXYd*, const unsigned, double);
}


#endif /* _VRJ_NET_GMTL_WRAPPERS_H_ */
