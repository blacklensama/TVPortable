#include "UKN/Basic.h"
#include "UKN/Ptr.h"

#include "UKN/detail/TypeTraits.h"

namespace ukn {
	
	template<typename T>
	class ForEachIterator: public Object {
	public:
		virtual bool      available() const = 0;
		virtual void      next() const = 0;
        virtual const T&  current() const = 0;
		
		operator bool() const {
			return true;
		}
	};
	
	template<typename T>
	class EnumerableForEachIterator: public ForEachIterator<T> {
	protected:
		SharedPtr<IEnumerator<T> > iterator;
	public:
		EnumerableForEachIterator(const IEnumerable<T>& enumerable):
		iterator(enumerable.createEnumerator()) {
		
		}
		
		EnumerableForEachIterator(const EnumerableForEachIterator<T>& enumerableIterator):
		iterator(enumerableIterator.iterator) {
		
		}
		
		bool available() const {
			return iterator->available();
		}
		
		void next() const {
			iterator->next();
		}
        
        const T& current() const {
            return iterator->current();
        }
	};
	
	template<typename T>
	EnumerableForEachIterator<T> CreateForEachIterator(const IEnumerable<T>& enumerable) {
		return enumerable;
	}
	
	#define UKN_SCOPE_VARIABLE(TYPE, VARIABLE, VALUE) \
		if(bool __scope_variable_flag__=true) \
			for(TYPE VARIABLE = VALUE; __scope_variable_flag__; __scope_variable_flag__=false)
			
	#define UKN_ENUMERABLE_FOREACH(TYPE, VARIABLE, COLLECTION) \
		UKN_SCOPE_VARIABLE(const ukn::ForEachIterator<ukn::traits::strip_qualifier<TYPE>::type >&, __foreach_iterator__, ukn::CreateForEachIterator<ukn::traits::strip_qualifier<TYPE>::type >(COLLECTION))\
		for(bool more=true; __foreach_iterator__.available(); __foreach_iterator__.next()) \
            if((more = false)) {} else \
            for(TYPE VARIABLE=__foreach_iterator__.current(); !more; more=true)
		
	#define UKN_ENUMERABLE_FOREACH_INDEXED(TYPE, VARIABLE, INDEXER, COLLECTION)\
		UKN_SCOPE_VARIABLE(const ukn::ForEachIterator<ukn::traits::strip_qualifier<TYPE>::type >&, __foreach_iterator__, CreateForEachIterator<ukn::traits::strip_qualifier<TYPE>::type >(COLLECTION))\
		UKN_SCOPE_VARIABLE(intPtr, indexer, 0) \
        for(bool more=true; __foreach_iterator__.available(); __foreach_iterator__.next(), INDEXER++) \
            if((more = false)) {} else \
            for(const VARIABLE=__foreach_iterator__.current(); !more; more=true)
    
} // namespace ukn