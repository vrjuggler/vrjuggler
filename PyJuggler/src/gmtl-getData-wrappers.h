#ifndef _PYJUGGLER_GMTL_GETDATA_WRAPPERS_H_
#define _PYJUGGLER_GMTL_GETDATA_WRAPPERS_H_

#include <boost/python/tuple.hpp>
#include <gmtl/Matrix.h>
#include <gmtl/VecBase.h>
#include <gmtl/Quat.h>

namespace gmtlWrappers
{
   template<typename T>
   boost::python::tuple Matrix_3_3_getData(gmtl::Matrix<T, 3, 3>* m)
   {
      return boost::python::make_tuple(m->mData[0], m->mData[1], m->mData[2],
                                       m->mData[3], m->mData[4], m->mData[5],
                                       m->mData[6], m->mData[7], m->mData[8]);
   }

   template boost::python::tuple Matrix_3_3_getData(gmtl::Matrix<float, 3, 3>*);

//   template<typename T>
//   boost::python::tuple Matrix_float_4_4_getData(gmtl::Matrix<T, 4, 4>* m)
//   {
//      return make_tuple(m->mData[0],  m->mData[1],  m->mData[2],  m->mData[3],
//                        m->mData[4],  m->mData[5],  m->mData[6],  m->mData[7],
//                        m->mData[8],  m->mData[9],  m->mData[10], m->mData[11]);
//   }

   template<typename T>
   boost::python::tuple VecBase_2_getData(gmtl::VecBase<T, 2>* v)
   {
      return boost::python::make_tuple(v->mData[0], v->mData[1]);
   }

   template boost::python::tuple VecBase_2_getData(gmtl::VecBase<double, 2>*);
   template boost::python::tuple VecBase_2_getData(gmtl::VecBase<float, 2>*);
   template boost::python::tuple VecBase_2_getData(gmtl::VecBase<int, 2>*);

   template<typename T>
   boost::python::tuple VecBase_3_getData(gmtl::VecBase<T, 3>* v)
   {
      return boost::python::make_tuple(v->mData[0], v->mData[1], v->mData[2]);
   }

   template boost::python::tuple VecBase_3_getData(gmtl::VecBase<double, 3>*);
   template boost::python::tuple VecBase_3_getData(gmtl::VecBase<float, 3>*);
   template boost::python::tuple VecBase_3_getData(gmtl::VecBase<int, 3>*);

   template<typename T>
   boost::python::tuple VecBase_4_getData(gmtl::VecBase<T, 4>* v)
   {
      return boost::python::make_tuple(v->mData[0], v->mData[1], v->mData[2],
                                       v->mData[3]);
   }

   template boost::python::tuple VecBase_4_getData(gmtl::VecBase<double, 4>*);
   template boost::python::tuple VecBase_4_getData(gmtl::VecBase<float, 4>*);
   template boost::python::tuple VecBase_4_getData(gmtl::VecBase<int, 4>*);

   template<typename T>
   boost::python::tuple Quat_getData(gmtl::Quat<T>* q)
   {
      return boost::python::make_tuple(q->mData[0], q->mData[1], q->mData[2],
                                       q->mData[3]);
   }

   template boost::python::tuple Quat_getData(gmtl::Quatd*);
   template boost::python::tuple Quat_getData(gmtl::Quatf*);

   template<typename TYPE, typename ROT>
   boost::python::tuple EulerAngle_getData(gmtl::EulerAngle<TYPE, ROT>* ang)
   {
      return boost::python::make_tuple(ang->getData()[0], ang->getData()[1],
                                       ang->getData()[2]);
   }

   template boost::python::tuple EulerAngle_getData(gmtl::EulerAngleXYZf*);
   template boost::python::tuple EulerAngle_getData(gmtl::EulerAngleZYXf*);
   template boost::python::tuple EulerAngle_getData(gmtl::EulerAngleZXYf*);
}

#endif /* _GMTL_GETDATA_WRAPPERS_H_ */
