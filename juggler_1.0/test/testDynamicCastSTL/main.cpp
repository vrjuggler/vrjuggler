#include <vector>
#include <iostream.h>

class base
{
public:
    virtual void bar() 
	{ cerr << "base\n"; }
};

class derived : public base
{
public:
    virtual void bar()
	{ cerr << "derived\n"; }
};

main(void)
{
    std::vector<base*> bVec;
    
    base* b = new derived;
    derived* d = new derived;
    
    bVec.push_back((base*)d);
    
    
    b->bar();
    if(dynamic_cast<derived*>(b) == NULL)
	cerr << "Failed to cast to derived.\n";
    else
	cerr << "Success casting to derived.\n";
	
    base* b2 = bVec[0];
    b2->bar();
    if(dynamic_cast<derived*>(b2) == NULL)
	cerr << "Failed to cast to derived.\n";
    else
	cerr << "Success casting to derived.\n";
}
