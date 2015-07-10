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
        
        
        NSLog(@"******* %@ *******", audio);
        
        bool writing = false;
        NSInteger numberOfFiles = 0;
        
        Float32 L, R, A, B;
        void *buffers[2] = {&A, &B};
        for (int i=0; i< [audio lengthInFrames]; i++)
        {
            
            [audio readFrames:1 intoBufferL:&L bufferR:&R];
            A = Lgate->process(L);
            B = Rgate->process(R);

            if (!Lgate->open() && writing) { // End of sample
                NSLog(@"Closing file length: %@", @([outFile lengthInSeconds]));
                if ([outFile lengthInSeconds] >= minFileLength) {
                    numberOfFiles++;
                }
                [outFile close];
                writing = false;
                
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
            }
            

        }

        [outFile close]; //Last file length not validated

                NSLog(@"Doneit %@", @([audio lengthInFrames]));
    }
    
    return 0;
}

//BellAudioFile * createFile()
//{
//    
//}