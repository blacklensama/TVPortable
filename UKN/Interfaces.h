#ifndef Project_Unknown_Interfaces_H_
#define Project_Unknown_Interfaces_H_

#include "UKN/Basic.h"
#include "UKN/StringUtil.h"

namespace ukn {

	template<typename T>
	class IEnumerator: public virtual Interface {
	public:
		virtual IEnumerator<T>* clone() const = 0;
		virtual const T& 		current() const = 0;
		virtual intPtr			index() const = 0;
		virtual bool			next() = 0;
		virtual bool			available() const = 0;
		virtual void			reset() = 0;
	};
	
	template<typename T>
	class IEnumerable: public virtual Interface {
	public:
		virtual IEnumerator<T>*	createEnumerator() const = 0;
	};
	
    class IResource: public virtual Interface {
    public:
        virtual const String& getName() const = 0;
    };
    
    template<typename T>
    class ISerializable: public virtual Interface {
        
    };
    
} // namespace ukn

#endif