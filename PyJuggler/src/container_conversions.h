// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _PYJ_CONTAINER_CONVERSIONS_H_
#define _PYJ_CONTAINER_CONVERSIONS_H_

#include <vector>
#include <boost/python/list.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/to_python_converter.hpp>
#include <boost/type_traits.hpp>

namespace pyj
{

// This is adapted from scitbx::boost_python::container_conversions::to_tuple,
// the source for which can be found at http://cctbx.sourceforge.net/
template<typename ContainerType, bool hasCopy = true>
struct to_tuple
{
   static PyObject* convert(ContainerType const& a)
   {
      boost::python::list result;
      typedef typename ContainerType::const_iterator const_iter;
      for ( const_iter p = a.begin(); p != a.end(); ++p )
      {
         if ( hasCopy )
         {
            result.append(boost::python::object(*p));
         }
         else
         {
            result.append(boost::python::object(boost::python::ptr(*p)));
         }
      }
      return boost::python::incref(boost::python::tuple(result).ptr());
   }
};

template<typename T>
struct std_vector_to_tuple
{
   // Holy functional programming, Batman!  This uses the Boost Metaprogramming
   // Library.
   std_vector_to_tuple()
   {
      boost::python::to_python_converter<
         std::vector<T>,
         to_tuple<
            std::vector<T>,
            boost::has_trivial_copy<
               typename boost::remove_pointer<T>::type
            >::value
         >
      >();
   }
};

} // namespace pyj

#endif /* _PYJ_CONTAINER_CONVERSIONS_H_ */
