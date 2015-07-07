//
//  AC_Logger.m
//  Shareight
//
//  Created by Hari Karam Singh on 25/02/2015.
//  Copyright (c) 2015 Shareight. All rights reserved.
//

#import "EchoLogger.h"

@implementation EchoLogger
{
    NSTimeInterval _startTime;
}

+ (instancetype)sharedInstance
{
    static dispatch_once_t pred;
    static EchoLogger *shared = nil;
    dispatch_once(&pred, ^{
        shared = [self new];
        shared.logLevels = kEchoLogLevelAll;
        shared.logLevelsForExcluded = kEchoLogLevelAll & ~kEchoLogLevelError;
        shared->_startTime = CACurrentMediaTime();
    });
    return shared;
}

//---------------------------------------------------------------------

- (void)setExcludedChannelsWithDictionary:(NSDictionary *)dictionary
{
    NSMutableArray *excludedChannels = dictionary.allKeys.mutableCopy;
    for (id chan in excludedChannels.copy)
    {
        // Default to allow if not added. Otherwise YES means allow so remove from excluded list
        if ([dictionary[chan] boolValue])
        {
            [excludedChannels removeObject:chan];
        }
    }
    
    self.excludedChannels = excludedChannels;
}

//---------------------------------------------------------------------

- (void)logWithLevel:(EchoLogLevel)level
             channel:(NSString *)channel
                line:(int)line
             message:(NSString *)msg, ...
{
    // Skip excludeds if below thresh
    if ([self.excludedChannels containsObject:channel] &&
        (level & self.logLevelsForExcluded) )
        return;
    
    // Skip other if below threshold
    if (!(level & self.logLevels)) return;
    
    // Format the given message
    va_list ap;
    va_start (ap, msg);
    msg = [[NSString alloc] initWithFormat:msg arguments:ap];
    va_end (ap);
        
    // Create our log structure
    // Leave INFO blank as the kind of go-to one
    static char *levelNames[] = {":TRACE", "DEBUG: ", "", ":WARNING", ":ERROR"};
    char *levelName = levelNames[0];
    if (level == kEchoLogLevelTrace) levelName = levelNames[0];
    else if (level == kEchoLogLevelDebug) levelName = levelNames[1];
    else if (level == kEchoLogLevelInfo) levelName = levelNames[2];
    else if (level == kEchoLogLevelWarn) levelName = levelNames[3];
    else if (level == kEchoLogLevelError) levelName = levelNames[4];
    
    NSTimeInterval timeDelta = CACurrentMediaTime() - _startTime;
    msg = [NSString stringWithFormat:@"%3.4f L%4D [%@%s] %@", timeDelta, line, channel, levelName, msg];
    
    fprintf(stdout,"%s\n", [msg UTF8String]);
}

@end
