////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _VPR_LIFETIME_TRACKER_H_
#define _VPR_LIFETIME_TRACKER_H_

#include <vpr/vprConfig.h>

#include <typeinfo>

#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <new>

#if defined(__GNUC__)
#  include <cxxabi.h>
#endif

namespace vpr
{
    namespace detail
    {
////////////////////////////////////////////////////////////////////////////////
// class LifetimeTracker
// Helper class for SetLongevity
////////////////////////////////////////////////////////////////////////////////

        class LifetimeTracker
        {
        public:
            LifetimeTracker(unsigned int x) : longevity_(x) 
            {}
            
            virtual ~LifetimeTracker() = 0;
            
            static bool Compare(const LifetimeTracker* lhs,
                const LifetimeTracker* rhs)
            {
                return lhs->longevity_ > rhs->longevity_;
            }
            
        private:
            unsigned int longevity_;
        };
        
        // Definition required
        inline LifetimeTracker::~LifetimeTracker() {} 
        
        // Helper data
        typedef LifetimeTracker** TrackerArray;
        extern VPR_DATA_API(TrackerArray) pTrackerArray;
        extern VPR_DATA_API(unsigned int) elements;

        // Helper destroyer function
        template <typename T>
        struct Deleter
        {
            static void Delete(T* pObj)
            {
/*
#if defined(__GNUC__)
               const char* mangled_name(typeid(T).name());
               int status;
               char* type_name = abi::__cxa_demangle(mangled_name, NULL, NULL,
                                                     &status);

               if ( status == 0 )
               {
                  std::cout << "Deleter<T>::Delete() for " << type_name
                            << std::endl;
                  free(type_name);
               }
               else
               {
                  std::cout << "Deleter<T>::Delete() for " << mangled_name
                            << std::endl;
               }
#else
               std::cout << "Deleter<T>::Delete() for " << typeid(T).name()
                         << std::endl;
#endif
*/
               delete pObj;
            }
        };

        // Concrete lifetime tracker for objects of type T
        template <typename T, typename Destroyer>
        class ConcreteLifetimeTracker : public LifetimeTracker
        {
        public:
            ConcreteLifetimeTracker(T* p,unsigned int longevity, Destroyer d)
                : LifetimeTracker(longevity)
                , pTracked_(p)
                , destroyer_(d)
            {}
            
            ~ConcreteLifetimeTracker()
            { destroyer_(pTracked_); }
            
        private:
            T* pTracked_;
            Destroyer destroyer_;
        };

        VPR_API(void) AtExitFn(); // declaration needed below
    
    } // namespace detail

////////////////////////////////////////////////////////////////////////////////
// function template SetLongevity
// Assigns an object a longevity; ensures ordered destructions of objects 
//     registered thusly during the exit sequence of the application
////////////////////////////////////////////////////////////////////////////////

    template <typename T, typename Destroyer>
    void SetLongevity(T* pDynObject, unsigned int longevity,
        Destroyer d = detail::Deleter<T>::Delete)
    {
        using namespace detail;
        
        TrackerArray pNewArray = static_cast<TrackerArray>(
                std::realloc(pTrackerArray, 
                    sizeof(*pTrackerArray) * (elements + 1)));
        if (!pNewArray) throw std::bad_alloc();
        
        // Delayed assignment for exception safety
        pTrackerArray = pNewArray;
        
        LifetimeTracker* p = new ConcreteLifetimeTracker<T, Destroyer>(
            pDynObject, longevity, d);
        
        // Insert a pointer to the object into the queue
        TrackerArray pos = std::upper_bound(
            pTrackerArray, 
            pTrackerArray + elements, 
            p, 
            LifetimeTracker::Compare);
        std::copy_backward(
            pos, 
            pTrackerArray + elements,
            pTrackerArray + elements + 1);
        *pos = p;
        ++elements;
        
        // Register a call to AtExitFn
        std::atexit(detail::AtExitFn);
    }

} // namespace vpr

////////////////////////////////////////////////////////////////////////////////
// Change log:
// May 21, 2001: Correct the volatile qualifier - credit due to Darin Adler
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// January 08, 2002: Fixed bug in call to realloc - credit due to Nigel Gent and
//      Eike Petersen
// March 08, 2002: moved the assignment to pTrackerArray in SetLongevity to fix
//      exception safety issue. Credit due to Kari Hoijarvi
// May 09, 2002: Fixed bug in Compare that caused longevities to act backwards.
//      Credit due to Scott McDonald.
////////////////////////////////////////////////////////////////////////////////

#endif // _VPR_LIFETIME_TRACKER_H_
