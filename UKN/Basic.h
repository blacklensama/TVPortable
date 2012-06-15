#ifndef Project_Unknown_Basic_H_
#define Project_Unknown_Basic_H_

#include "UKN/Uncopyable.h"
#include "UKN/Platform.h"

namespace ukn {

	class Interface: private Uncopyable {
	public:
		virtual ~Interface() { }
	};
	
	class Object {
	public:
		virtual ~Object() { }
	};
	
	template<typename T, size_t minSize>
	union BinaryRetriever {
		T 		data;
		uint8 	binary[sizeof(T)>minSize?sizeof(T):minSize]; 
	};
   

} // namespace ukn

#endif