#ifndef _VPR_FACTORY_H
#define _VPR_FACTORY_H


namespace vpr
{

template <class IdentifierType, class AbstractProduct>
struct NullFactoryError
{
   static AbstractProduct* onUnknownType(IdentifierType)
   {
      return NULL;
   }
};

/** Implements a useful little template function usable as a Creator in factory */
template<class AbstractProduct, class ConcreteProduct>
AbstractProduct* CreateProduct()
{ return (new ConcreteProduct); }

/** Implements generic Factory pattern
*
* AbstractProduct: The base class for the hierarchy for the object factory
* IndentifierType: The id for indexing the creators (must be sortable)
* ProductCreator: The callable entity that creates objects.
*        Must support:
*           AbstractProduct* operator();
*        (ex: functions, functors, classes)
*        default: Simple function of type: AbstractProduct* func() { }
* FactoryErrorPolicy: The handler for failed lookups.
*        Must support:
*           FactoryErrorImpl<IdentifierType, AbstractProduct> fErrorImpl;
*           AbstractProduct* p = fErrorImpl.onUnknownType(id)
*/
template<
   class AbstractProduct,
   class IdentifierType,
   typename ProductCreator = AbstractProduct* (*)(),
      class FactoryErrorPolicy = NullFactoryError<IdentifierType, AbstractProduct>
>
class Factory : public FactoryErrorPolicy
{
public:
   bool registerCreator(const IdentifierType& id, ProductCreator creator)
   {
      return mCreatorMap.insert( CreatorMap::value_type(id,creator)).second;
   }

   bool unregisterCreator(const IdentifierType& id)
   {
      return (mCreatorMap.erase(id) == 1);   // return (num_erased == 1)
   }
   bool isRegistered(const IdentifierType& id)
   {
      return ( mCreatorMap.find(id) != mCreatorMap.end());
   }

   AbstractProduct* createObject(const IdentifierType& id)
   {
      typename CreatorMap::const_iterator i = mCreatorMap.find(id);
      if(i != mCreatorMap.end())
      {
         return (i->second)();
      }

      return onUnknownType(id);     // Calls template method from FactoryErrorPolicy<>
   }



protected:
   typedef std::map<IdentifierType, ProductCreator> CreatorMap;
   CreatorMap mCreatorMap;
};

}; // namespace vpr


#endif

