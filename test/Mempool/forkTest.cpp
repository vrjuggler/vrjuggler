#include <iostream.h>
#include <sys/types.h>
#include <unistd.h>
#include <SharedMem/C2Memory.h>
#include <SharedMem/SharedType.h>

void main(void)
{
    MemPool* anSgiPool = new MemPoolSGI(1024*1024);
    
    SharedType<int> a(2);
    SharedType<int> b(10);
    SharedType<int>* c = new(anSgiPool) SharedType<int>(2);
    SharedType<int>* d = new(anSgiPool) SharedType<int>(10);
    
    cout << endl << endl;
    cout << "a:" << a << "  b:" << b << "\t\tShould be: a:2  b:10\n";
    cout << "c:" << *c << "  d:" << *d << "\t\tShould be: c:2  d:10\n";

    if(fork())
    {	// Parent
	sginap(100);
	
	cout << "\n\n---- Parent ----\n";
	cout << "a:" << a << "  b:" << b << "\t\tShould be: a:3  b:12 (If it was shared)\n";
	cout << "c:" << *c << "  d:" << *d << "\t\tShould be: c:3  d:12\n";
    } else 
    {	// Child
	a = a + 1;
	b = b + 2;
	*c = *c + 1;
	*d = *d + 2;
	cout << "\n\n---- Child ----\n";
	cout << "a:" << a << "  b:" << b << "\t\tShould be: a:3  b:12 (If it was shared)\n";
	cout << "c:" << *c << "  d:" << *d << "\t\tShould be: c:3  d:12\n";
    }
    
    delete anSgiPool;
}
