#ifndef _SNX_SINGLETON_H_
#define _SNX_SINGLETON_H_

#include <stdlib.h> // for NULL, duh...

namespace snx
{
   /**
    * You can use this to make a singleton, just inherit like
    * so...
    *
    * class myClass : public snx::Singleton<myClass>
    */
   template< class singleClass >
   class Singleton
   {
   public:
      /**
       * Use the macro from above... kludgy yeah, but...
       * NOTE: it might not be good to have the imp inline 
       *       in the header???<br>
       * NOTE: currently, func is thread safe after first call to instance().
       * if first call to instance happens multiple times simultaneously
       * then don't be surprised when something dies because of a mutex..
       * this bug can be caused by spawning two threads immediately after 
       * entering main()
       */
      inline static singleClass* instance( void )        
      {                                                 
         // WARNING! race condition possibility, creation of static vars 
         // are not thread safe.  This is only an issue when creating
         // your first thread, since it uses a singleton thread manager,
         // the two threads might both try to call instance at the same time
         // which then the creation of the following mutex would not be certain.
         //static vpr::Mutex singleton_lock1;       
         static singleClass* the_instance1 = NULL;   
                                     
         if (NULL == the_instance1)                 
         {                                            
            //vpr::Guard<vpr::Mutex> guard( singleton_lock1 );
            if (NULL == the_instance1)        
            { the_instance1 = new singleClass; }         
         }                                      
         return the_instance1;                     
      }

   protected:
      /**
       * Don't create a singleton with new!  
       * use instance()
       */
      Singleton()
      {
      }

      /// don't delete a singleton!
      virtual ~Singleton()
      {
      }
   };
}; // end of namespace snx


#endif
