#ifndef RNG_H
#define RNG_H

#include <random>

class RNG{
private:
    static inline std::default_random_engine engine= std::default_random_engine(time(0));
    static inline  std::uniform_int_distribution<uint8_t> udd {};
public:
    
    uint8_t inline  roll(){
        return udd(engine, std::uniform_int_distribution<uint8_t>::param_type{0,255});
    }

    void inline seed_statically(){
        // gaurentee that RNG has fixed value
        engine= std::default_random_engine(engine.default_seed);
    }
};


#endif