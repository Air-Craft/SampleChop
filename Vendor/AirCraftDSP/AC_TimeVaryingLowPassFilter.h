//
//  AC_TimeVaryingLowPassFilter.h
//  AC-Sabre
//
//  Created by Hari Karam Singh on 12/06/2014.
//
//

#ifndef __AC_TimeVaryingLowPassFilter__
#define __AC_TimeVaryingLowPassFilter__

#include <math.h>

namespace air_craft
{
    namespace dsp
    {
        class TimeVaryingLowPassFilter
        {
        /////////////////////////////////////////////////////////////////////////
        #pragma mark - IVARs
        /////////////////////////////////////////////////////////////////////////

        private:
            float_t _RC;
            float_t _prevTimestamp;
            float_t _value;
            
            
        /////////////////////////////////////////////////////////////////////////
        #pragma mark - Life Cycle
        /////////////////////////////////////////////////////////////////////////
        public:
            
            TimeVaryingLowPassFilter(float_t cutoffFrequency) : _RC(1.0 / (2.0 * M_PI * cutoffFrequency)), _prevTimestamp(-1), _value(0) {}
            
        
        /////////////////////////////////////////////////////////////////////////
        #pragma mark - Public Methods
        /////////////////////////////////////////////////////////////////////////
        public:
            
            inline float_t value() { return _value; }
            
            //---------------------------------------------------------------------

            inline float_t process(float_t x, float_t timestamp)
            {
                // Skip first round
                if (_prevTimestamp < 0) {
                    _prevTimestamp = timestamp;
                    return _value;
                }
                float_t dt = timestamp - _prevTimestamp;
                float_t a = dt / (_RC + dt);
                _value = a*x + (1-a)*_value;
                
                return _value;
            }
            
            //---------------------------------------------------------------------

            inline void reset(void) { _value = 0; }
        };
    }
}

#endif
