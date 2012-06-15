//
//  RandomUtil.h
//  Project Unknown
//
//  Created by Robert Bu on 12/7/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_RandomUtil_h
#define Project_Unknown_RandomUtil_h

#include "UKN/Platform.h"

namespace ukn {
    
    class Random {
    public:
        static void SetRandomSeed(uint32 seed);
        static uint32 GetRandomSeed();
        
        // 0 - 1
        static float NormalizedRandomFloat();
        
        static float RandomFloat(float min, float max);
        static int32 RandomInt(int32 min, int32 max);
    };
    
    /**
     * Global RandomGenerator
     * You can change the functions in it to make the random generator use your funcs instead of internal generator funcs
     **/
    static struct RandomGenerator {
        RandomGenerator();
        
        typedef void (*set_random_seed_func_ptr)(uint32 seed);
        typedef float (*normalized_random_func_ptr)();
        
        set_random_seed_func_ptr random_seed_func;
        normalized_random_func_ptr normalized_random_func;
        
    } random_generator;
    
    /**
     * Specialize this to support your types
     **/
    template<typename T>
    struct Randomizer;
 
    template<typename T>
    class RandomObject {
    public:
        RandomObject() { }
        
        RandomObject(const T& mint, const T& maxt):
        min(mint),
        max(maxt) { }
        
        ~RandomObject() { }
        
        const T& getMin() const { return this->min; }
        const T& getMax() const { return this->max; }
        
        T randomize() { 
            return Randomizer<T>::Randomize(min, max);
        }
        
        void set(const T& min, const T& max) {
            this->min = min;
            this->max = max;
        }
        
        T min;
        T max;
    };
    
    class Vector2;
    class Vector3;
    class Color;
    
    template<> struct Randomizer<float> {
        static float Randomize(float min, float max);
    };
    
    template<> struct Randomizer<Vector2> {
        static Vector2 Randomize(const Vector2& min, const Vector2& max);
    };
    
    template<> struct Randomizer<Vector3> {
        static Vector3 Randomize(const Vector3& min, const Vector3& max);
    };
    
    template<> struct Randomizer<Color> {
        static Color Randomize(const Color& min, const Color& max);
    };
    
    typedef RandomObject<float>     RandomRange;
    typedef RandomObject<Vector2>   RandomArea;
    typedef RandomObject<Vector3>   RandomVolume;
    typedef RandomObject<Color>     RandomColor;
    
} // namespace ukn

#endif
