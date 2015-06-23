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

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        if (argc < 2)
        {
            NSLog(@"ERROR: No file");
            exit(1);
        }
   
        NSString *arg1 = [NSString stringWithUTF8String:argv[1]];
        NSString *path = [[[NSFileManager defaultManager] currentDirectoryPath] stringByAppendingPathComponent:arg1];
        
        if (![[NSFileManager defaultManager] fileExistsAtPath:path])
        {
            NSLog(@"ERROR: File %@ does not exist!", path);
            exit(1);
        }
        
        BellAudioFile *audio = [BellAudioFile openAudioFileWithURL:[NSURL URLWithString:path]];
        
        NSLog(@"******* %@ *******", audio);
        Float32 L, R;
        for (int i=0; i<100; i++)
        {
            [audio seekToFrame:i*1000];
            [audio readFrames:1 intoBufferL:&L bufferR:&R];
            NSLog(@"******* %.2f, %.2f *******", L, R);
        }
    }
    
    return 0;
}