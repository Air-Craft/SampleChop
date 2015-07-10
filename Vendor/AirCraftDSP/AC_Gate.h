//
//  AC_Gate.h
//  SampleChop
//
//  Created by Hari Karam Singh on 23/06/2015.
//  Copyright (c) 2015 Air Craft. All rights reserved.
//

#ifndef __SampleChop__AC_Gate__
#define __SampleChop__AC_Gate__

#include <iostream>
#include <math.h>
#include "AC_EnvolopeFollower.h"

namespace air_craft
{
    namespace dsp
    {
        class Gate {
            /////////////////////////////////////////////////////////////////////////
            #pragma mark - Private ivars
            /////////////////////////////////////////////////////////////////////////
            EnvelopeFollower *_magnitudeEnvelope;
            EnvelopeFollower *_fadeEnvelope;
            float_t _openThreshold;
            float_t _closeThreshold;
            float_t _attackMs;
            float_t _releaseMs;
            bool _open;
            
            
            
        public:
            
            Gate(float_t openThreshold,
                 float_t closeThreshold,
                 float_t attackMs,
                 float_t releaseMs) : _openThreshold(openThreshold), _closeThreshold(closeThreshold), _attackMs(attackMs), _releaseMs(releaseMs)
            {
                _magnitudeEnvelope = new EnvelopeFollower(20, 20, 44000);
                _fadeEnvelope = new EnvelopeFollower(1, 1, 44000);
                
                _open = false;
                
            }
            
            /////////////////////////////////////////////////////////////////////////
#pragma mark - Public ivars
            /////////////////////////////////////////////////////////////////////////
            
            /////////////////////////////////////////////////////////////////////////
#pragma mark - Life Cycle
            /////////////////////////////////////////////////////////////////////////
            
            /////////////////////////////////////////////////////////////////////////
#pragma mark - Operators
            /////////////////////////////////////////////////////////////////////////
            
            /////////////////////////////////////////////////////////////////////////
#pragma mark - Public API
            /////////////////////////////////////////////////////////////////////////
            inline float_t process(float_t x)
            {
                float_t magnitude = _magnitudeEnvelope->process(x);
                if (magnitude >= _openThreshold && !_open) {
                    _open = true;
                } else if (magnitude <= _closeThreshold && _open) {
                    _open = false;
                }
//                std::cout << "Mag: " << magnitude << std::endl;
                float_t fadeMultiplier = _fadeEnvelope->process(_open ? 1 : 0);
                
                return fadeMultiplier * x;
            }
            
            
            inline bool open() // Not same as internally open. Stays open during fade out
            {
                return _fadeEnvelope->value() > 0;
            }
            /////////////////////////////////////////////////////////////////////////
#pragma mark - Private API
            /////////////////////////////////////////////////////////////////////////
        protected:
            
        };
        
        
        /////////////////////////////////////////////////////////////////////////
#pragma mark - Inlines: Life Cycle
        /////////////////////////////////////////////////////////////////////////
        
        /////////////////////////////////////////////////////////////////////////
#pragma mark - Inlines: Operators
        /////////////////////////////////////////////////////////////////////////
        
        /////////////////////////////////////////////////////////////////////////
#pragma mark - Inlines: Public API
        /////////////////////////////////////////////////////////////////////////
        
        /////////////////////////////////////////////////////////////////////////
#pragma mark - Inlines: Private API
        /////////////////////////////////////////////////////////////////////////
        
        

    }
}

#endif /* defined(__SampleChop__AC_Gate__) */
