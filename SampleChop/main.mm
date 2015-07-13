//
//  main.m
//  SampleChop
//
//  Created by Hari Karam Singh on 23/06/2015.
//  Copyright (c) 2015 Air Craft. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BellDefs.h"
#import "BellUtils.h"
#import "BellAudioFile.h"
#import "AC_Gate.h"

#import "FFTHelper.h"

FFTHelperRef *fftConverter = NULL;

//=======================================


//==========================Window Buffer
Float32 *windowBuffer= NULL;
//=======================================

const Float32 NyquistMaxFreq = 44100/2.0;

/// max value from vector with value index (using Accelerate Framework)
static Float32 vectorMaxValueACC32_index(Float32 *vector, unsigned long size, long step, unsigned long *outIndex) {
    Float32 maxVal;
    vDSP_maxvi(vector, step, &maxVal, outIndex, size);
    return maxVal;
}
Float32 frequencyHerzValue(long frequencyIndex, long fftVectorSize, Float32 nyquistFrequency ) {
    return ((Float32)frequencyIndex/(Float32)fftVectorSize) * nyquistFrequency;
}

static Float32 strongestFrequencyHZ(Float32 *buffer, FFTHelperRef *fftHelper, UInt32 frameSize, Float32 *freqValue) {
    Float32 *fftData = computeFFT(fftHelper, buffer, frameSize);
    fftData[0] = 0.0;
    unsigned long length = frameSize/2.0;
    Float32 max = 0;
    unsigned long maxIndex = 0;
    max = vectorMaxValueACC32_index(fftData, length, 1, &maxIndex);
    if (freqValue!=NULL) { *freqValue = max; }
    Float32 HZ = frequencyHerzValue(maxIndex, length, NyquistMaxFreq);
    return HZ;
}


int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        if (argc < 2)
        {
            NSLog(@"ERROR1: No file");
            exit(1);
        }
   
        NSString *currentPath = [[NSFileManager defaultManager] currentDirectoryPath];
        NSString *arg1 = [NSString stringWithUTF8String:argv[1]];
        NSString *path = [currentPath stringByAppendingPathComponent:arg1];

        
        if (![[NSFileManager defaultManager] fileExistsAtPath:path])
        {
            NSLog(@"ERROR: File %@ does not exist!", path);
            exit(1);
        }
        
        BellAudioFile *audio = [BellAudioFile openAudioFileWithURL:[NSURL URLWithString:path]];
        BellAudioFile *outFile;
        
        CGFloat minFileLength = 1.0; //Seconds
        
        CGFloat openThreshold = 0.025;
        CGFloat closeThreshold = 0.01;
        CGFloat attackMs = 3.0;
        CGFloat releaseMs = 3.0;
        
        air_craft::dsp::Gate *Lgate = new air_craft::dsp::Gate(openThreshold, closeThreshold, attackMs, releaseMs);
        air_craft::dsp::Gate *Rgate = new air_craft::dsp::Gate(openThreshold, closeThreshold, attackMs, releaseMs);
        
        NSArray *noteStrings = @[@"C", @"C#", @"D", @"D#", @"E", @"F", @"F#", @"G", @"G#", @"A", @"A#", @"B"];



        NSLog(@"******* %@ *******", audio);
        
        bool writing = false;
        NSInteger numberOfFiles = 0;
        
        Float32 L, R, A, B;
        Float32 maxHZ = 0.0;
        void *buffers[2] = {&A, &B};
        for (int i=0; i< [audio lengthInFrames]; i++)
        {
            
            [audio readFrames:1 intoBufferL:&L bufferR:&R];
            A = Lgate->process(L);
            B = Rgate->process(R);

            if (!Lgate->open() && writing) { // End of sample

                Float32 seconds = [outFile lengthInSeconds];
                UInt32 frames = [outFile lengthInFrames];//MIN(131072,[outFile lengthInFrames]);
                NSLog(@"Frames: %@ Seconds: %@", @(frames), @(seconds));
                if ([outFile lengthInSeconds] >= minFileLength) {
                    numberOfFiles++;
                }
                [outFile close];
                [outFile reopenForReading];
                Float32 X[frames];
                Float32 Y[frames];

                [outFile readFrames:frames fromFrame:0 intoBufferL:X bufferR:Y];


                Float32 maxHZValue = 0;
                fftConverter = FFTHelperCreate(frames);
                maxHZ = strongestFrequencyHZ(X, fftConverter, frames, &maxHZValue);
                writing = false;

                Float32 reference = 440.0;
                int midiNote = (12*(log10(maxHZ/reference)/log10(2)) + 57) + 0.5;


                
                NSLog(@"Closing file HZ: %@ Note: %@%@", @(maxHZ), [noteStrings objectAtIndex:midiNote%12], @(midiNote%12));
                [outFile close];
                
            } else if (Lgate->open() && !writing) { //Start a new file
                writing = true;
                NSString *fileName = [NSString stringWithFormat: @"%0.2ld-out-%@", (long)numberOfFiles, arg1];
                NSLog(@"Creating new file: %@", fileName);
                NSString *outPath = [currentPath stringByAppendingPathComponent:fileName];
                outFile = [BellAudioFile createNewAudioFileWithURL:[NSURL URLWithString:outPath] fileFormat:kBellFileFormat_CAF_LPCM_Stereo_44_1_16bit_Packed_SignedInt_BigEndian];

            }
            if (Lgate->open() && writing) { //If there is data coming from gate and file is open continue writing
                AudioBufferList *abl = Bell_CreateAudioBufferListUsingExistingBuffers(audio.clientStreamFormat.mChannelsPerFrame, buffers, audio.clientStreamFormat.mBytesPerFrame);
            
                [outFile writeFrames:1 fromAudioBufferList:abl];
                //take only data from 1 channel
                
            }
            

        }

        [outFile close]; //Last file length not validated

        NSLog(@"Doneit %@", @([audio lengthInFrames]));
    }
    
    return 0;
}
