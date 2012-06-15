//
//  Singleton.h
//  Project Unknown
//
//  Created by Robert Bu on 12/16/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Singleton_h
#define Project_Unknown_Singleton_h


namespace ukn {
    
    template<typename T>
    struct ClassInstantiator {
        static T* instantiate() {
            return new T();
        }
        
        template<typename R>
        static T* instantiate(R arg) {
            return new T(arg);
        }
        
        static void release(T* obj) {
            delete obj;
        }
    };
    
    // class for allocating singleton on the heap
    template<typename T, typename R>
    class SingletonHolder {
    public:
        SingletonHolder():
        mInstance(0) {
            
        }
        
        ~SingletonHolder() {
            if(this->mInstance)
                ClassInstantiator<T>::release(this->mInstance);
        }
        
        T* get(R arg) {
            if(!mInstance) mInstance = ClassInstantiator<T>::template instantiate<R>(arg);
            return mInstance;
        }
        
    private:
        T* mInstance;
    };
    
    template<typename T>
    class SingletonHolder<T, void> {
    public:
        SingletonHolder():
        mInstance(0) {
            
        }
        
        ~SingletonHolder() {
            if(this->mInstance)
                ClassInstantiator<T>::release(this->mInstance);
        }
        
        T* get(void) {
            if(!mInstance) mInstance = ClassInstantiator<T>::instantiate();
            return mInstance;
        }
        
    private:
        T* mInstance;
    };
    
    template<typename T>
    class Singleton {
    public:
        static T& Instance() {
            return *InstancePtr();
        }
        
        static T* InstancePtr() {
            if(!mInstance) mInstance = ClassInstantiator<T>::instantiate();
            return mInstance;
        }
        
        static void Destroy() {
            if(mInstance) 
                ClassInstantiator<T>::release(mInstance);
            mInstance = 0;
        }
        
    private:
        static T* mInstance;
        
    protected:
        Singleton() { }
        virtual ~Singleton() { }
    };
    
    template<typename T>
    T* Singleton<T>::mInstance = 0;
    
} // namespace ukn


#endif
