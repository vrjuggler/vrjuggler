#ifndef CPPUNIT_GUARDS_H
#define CPPUNIT_GUARDS_H

// Prevent copy construction and assignment for a class

#define REFERENCEOBJECT(TYPE) private: TYPE (const TYPE& other); TYPE& operator= (const TYPE& other);

#endif

