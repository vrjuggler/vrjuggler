#include <iostream.h>
#include <vector>
#include <SharedMem/MemPool_alloc.h>

template<class T, class A>
void printVector(std::vector<T, A>& list);

int main(void)
{
    std::vector<int, mempool_alloc> intList;
    
    for(int i=0;i<20;i++)
	intList.push_back(i);
	
    printVector(intList);
	
    if(fork())
    {	// Parent
	sginap(100);
	
	cout << "\n\n---- Parent ----\n";
	printVector(intList);
    }
    else
    {	// Child
    
	cout << "\n\n---- Child ----\n";
	
	intList[5] = 69;
	intList[10] = 71;
	
	//for(i=0;i<15;i++)
	//    intList.pop_back();
	    
	printVector(intList);
    }
}


template<class T, class A>
void printVector(std::vector<T, A>& list)
{
    for(int i=0;i<list.size();i++)
	cout << list[i] << ", "; 
    cout << endl << flush;
}
