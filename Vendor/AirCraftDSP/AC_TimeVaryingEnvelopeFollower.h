//
//  AC_TimeVaryingEnvelopeFollower.h
//  AC-Sabre
//
//  Created by Hari Karam Singh on 12/06/2014.
//
//

#ifndef AC_TimeVaryingEnvelopeFollower__
#define AC_TimeVaryingEnvelopeFollower__


#include <math.h>

namespace air_craft
{
    namespace dsp
    {
        class TimeVaryingEnvelopeFollower
        {
        /////////////////////////////////////////////////////////////////////////
        #pragma mark - IVARs
        /////////////////////////////////////////////////////////////////////////
            
        private:
            float_t _value;
            float_t _prevTimestamp;
            float_t _kAtt;  // Constants for calculations
            float_t _kRel;
            
            
        /////////////////////////////////////////////////////////////////////////
        #pragma mark - Life Cycle
        /////////////////////////////////////////////////////////////////////////
        public:
            
            TimeVaryingEnvelopeFollower(float_t attackMs, float_t releaseMs) : _value(0), _prevTimestamp(-1)
            {
                _refreshConstants(attackMs, releaseMs);
            }
            
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
                _prevTimestamp = timestamp; // DNF!!!@@!
                
                // Attack or release?  Get the constant
                float_t k;
                x = fabs(x);
                if (x > _value) {
                    k = pow(_kAtt, dt);
                } else {
                    k = pow(_kRel, dt);
                }
                
                _value = k * ( _value - x ) + x;
                
                return _value;
            }
            
            //---------------------------------------------------------------------
            
            /** Specify a value for reseting to a non-zero value */
            inline void reset(float_t value = 0)
            {
                _prevTimestamp = -1;
                _value = value;
            }

            
        /////////////////////////////////////////////////////////////////////////
        #pragma mark - Additional Privates
        /////////////////////////////////////////////////////////////////////////
        private:
            
            inline void _refreshConstants(float_t attackMs, float_t releaseMs)
            {
                _kAtt = pow( 0.01, 1.0 / ( attackMs * 0.001 ) );
                _kRel = pow( 0.01, 1.0 / ( releaseMs * 0.001 ) );
            }
            
        };
    }
}

#endif /* defined(AC_TimeVaryingEnvelopeFollower__) */
