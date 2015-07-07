//
//  AC_EnvolopeFollower.hpp
//  SampleChop
//
//  Created by Chris Mitchelmore on 07/07/2015.
//  Copyright © 2015 Air Craft. All rights reserved.
//

#ifndef AC_EnvolopeFollower_cpp
#define AC_EnvolopeFollower_cpp


#include <math.h>

namespace air_craft
{
    namespace dsp
    {
        class EnvelopeFollower
        {
            /////////////////////////////////////////////////////////////////////////
#pragma mark - IVARs
            /////////////////////////////////////////////////////////////////////////
            
        private:
            float_t _value;
            float_t _kAtt;  // Constants for calculations
            float_t _kRel;

            
            
            /////////////////////////////////////////////////////////////////////////
#pragma mark - Life Cycle
            /////////////////////////////////////////////////////////////////////////
        public:
            
            EnvelopeFollower(float_t attackMs, float_t releaseMs, float_t sampleRate) : _value(0)
            {
                 float_t sampleLength = 1.0/sampleRate;
                _kAtt = pow( pow( 0.01, 1.0 / ( attackMs * 0.001 ) ), sampleLength);
                _kRel = pow( pow( 0.01, 1.0 / ( releaseMs * 0.001 ) ), sampleLength);
            }
            
            /////////////////////////////////////////////////////////////////////////
#pragma mark - Public Methods
            /////////////////////////////////////////////////////////////////////////
        public:
            
            inline float_t value() { return _value; }
            
            //---------------------------------------------------------------------
            
            inline float_t process(float_t x)
            {
                // Attack or release?  Get the constant
                float_t k;
                x = fabs(x);
                if (x > _value) {
                    k = _kAtt;
                } else {
                    k = _kRel;
                }
                
                _value = k * ( _value - x ) + x;
                
                return _value;
            }
            
            //---------------------------------------------------------------------
            
            /** Specify a value for reseting to a non-zero value */
            inline void reset(float_t value = 0)
            {
                _value = value;
            }
            
        };
    }
}

#endif /* AC_EnvolopeFollower_cpp */
