#include <iostream.h>
#include <stdio.h>

#include <SharedMem/Integer.h>
#include <SharedMem/SharedType.h>
#include <SharedMem/MemPool.h>

int main(void)
{       
    MemPool* aSharedPool = new C2SharedPool(1024*1024, 8, "/var/tmp/memPoolSGITestXXXXXX");
    
    int x, y;
    x = 5;
    y = 9;

	// ------- NORMAL ALLOCATION VIA operator new ------- //    
    cout << "\nTest normal allocation of C2Memory based objects.\n";
    SharedType<int>* normal1 = new SharedType<int>(5);
    SharedType<float>* normal2 = new SharedType<float>(10.45);
    
    cout << "\tnormal1: " << *normal1 << "\tShould be: 5" << endl;
    cout << "\tnormal2: " << *normal2 << "\tShould be: 10.45" << endl;
    cout << "\t Now we will try to delete them....." << flush;
    
    delete normal1;
    delete normal2;
    cout << "sucess!!!\n";

	// ----- More Normal Allocation ----- //
    SharedType<int> q(9);
	cerr << "\nx: " << x << "\ty: " << y << "\tq: " << q;
    q = q + 1;
    	cerr << "\nx: " << x << "\ty: " << y << "\tq: " << q;
    q = 1;
    	cerr << "\nx: " << x << "\ty: " << y << "\tq: " << q;
    q = x + y;
    	cerr << "\nx: " << x << "\ty: " << y << "\tq: " << q;
    
    SharedType<int> r(8);
    
    q = r;
    	cerr << "\nx: " << x << "\ty: " << y << "\tq: " << q;
    
    SharedType<int*> xPtr(&x);
    SharedType<int*> yPtr(&y);
    
    *xPtr = 7;
    	cerr << "\nx: " << x << "\ty: " << y << "\tq: " << q;
    *yPtr = 10;
    	cerr << "\nx: " << x << "\ty: " << y << "\tq: " << q;
    
	// ------ Shared allocation ------ //
    cout << "\nTest Shared Memory allocation of C2Memory based objects.\n";
    
    SharedType<int>* a = new(aSharedPool) SharedType<int>(9);
    SharedType<int>* b = new(aSharedPool) SharedType<int>(3);
	
	cout << "\na: " << *a << "\tb: " << *b << endl;
	cout << "Should be: a:9     b:3" << endl;
    *a = 12;
	cout << "\na: " << *a << "\tb: " << *b << endl;
	cout << "Should be: a:12     b:3" << endl;
    *b = 15;
	cout << "\na: " << *a << "\tb: " << *b << endl;
	cout << "Should be: a:12     b:15" << endl;
	
	cout << "\tNow we will delete them...." << flush;
    
    delete a;
	cout << "a deleted...." << flush;
    delete b;
	cout << "b deleted....Sucess\n" << flush;
	
    delete aSharedPool;
	     
    return 0;
}
