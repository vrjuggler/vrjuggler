
#ifndef USERTESTCASE_H
#define USERTESTCASE_H

#include <vrj/Test/TestCase.h>

class UserTestCase : public vrj::test::TestCase
{
public:
    virtual void setUp()
    {;}

    virtual void tearDown()
    {;}

    void processTest()
    {;}

    bool isDone()
    {;}

};
#endif //USERTESTCASE_H