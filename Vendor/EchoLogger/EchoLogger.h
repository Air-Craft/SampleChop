//
//  AC_Logger.h
//  Shareight
//
//  Created by Hari Karam Singh on 25/02/2015.
//  Copyright (c) 2015 Shareight. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 ECHOLOGGER_FRIENDLY_ENUMS (def=on)
 ECHOLOGGER_USE_FRIENDLY_FUNCTION_NAMES (def=off)
 ECHOLOGGER_REPLACE_NSLOG (def=off)
 ECHOLOGGER_DISABLE_ALL_IF_NOT_DEBUG (default on)
 
 ECHOLOGGER_CHANNEL channel override
 */


/////////////////////////////////////////////////////////////////////////
#pragma mark - Core Logging Macros
/////////////////////////////////////////////////////////////////////////

/** Override in your .m file to have custom channels. Useful for grouping several files together into one log */
#define ECHOLOGGER_CHANNEL nil

#define _STR2CHANNEL(str) [NSString string

/** Basic level based logger. Channel is the file name capitalised without the extension or the page override */
#undef echo
#define echo(level, fmt, ...) \
    [[EchoLogger sharedInstance] \
                 logWithLevel:level \
                 channel:(ECHOLOGGER_CHANNEL ?: [[[[NSString stringWithUTF8String:__FILE__] lastPathComponent] componentsSeparatedByString:@"."][0] uppercaseString]) \
                 line:__LINE__ \
                 message:@"" fmt, ##__VA_ARGS__]



/////////////////////////////////////////////////////////////////////////
#pragma mark - EZ Enums (Defaults to On)
/////////////////////////////////////////////////////////////////////////

#if !defined(ECHOLOGGER_FRIENDLY_ENUMS) || ECHOLOGGER_FRIENDLY_ENUMS

#undef kTRACE
#undef kDEBUG
#undef kINFO
#undef kWARN
#undef kERROR
#undef kALL
#define kTRACE kEchoLogLevelTrace
#define kDEBUG kEchoLogLevelDebug
#define kINFO  kEchoLogLevelInfo
#define kWARN  kEchoLogLevelWarn
#define kERROR kEchoLogLevelError
#define kALL   kEchoLogLevelAll
#endif


/////////////////////////////////////////////////////////////////////////
#pragma mark - Friendly Functions
/////////////////////////////////////////////////////////////////////////

#if ECHOLOGGER_USE_FRIENDLY_FUNCTION_NAMES

#undef trace
#undef dbg
#undef info
#undef warn
#undef error
#define trace(fmt, ...)    echo(kEchoLogLevelTrace, fmt, ##__VA_ARGS__)
#define dbg(fmt, ...)      echo(kEchoLogLevelDebug, fmt, ##__VA_ARGS__)
#define info(fmt, ...)     echo(kEchoLogLevelInfo,  fmt, ##__VA_ARGS__)
#define warn(fmt, ...)     echo(kEchoLogLevelWarn,  fmt, ##__VA_ARGS__)
#define error(fmt, ...)    echo(kEchoLogLevelError, fmt, ##__VA_ARGS__)

#endif


/////////////////////////////////////////////////////////////////////////
#pragma mark - NSLog Replacer
/////////////////////////////////////////////////////////////////////////

#if ECHOLOGGER_REPLACE_NSLOG

#undef NSLog
#define NSLog(fmt, ...) echo(kEchoLogLevelDebug, fmt, ##__VA_ARGS__)

#endif


/////////////////////////////////////////////////////////////////////////
#pragma mark - DEBUG Disabler
/////////////////////////////////////////////////////////////////////////
// default = on
#if !DEBUG && (!defined(ECHOLOGGER_DISABLE_ALL_IF_NOT_DEBUG) || ECHOLOGGER_DISABLE_ALL_IF_NOT_DEBUG)

#undef echo
#define echo(...)

#if ECHOLOGGER_USE_FRIENDLY_FUNCTION_NAMES
#undef trace
#undef dbg
#undef info
#undef warn
#undef error
#define trace(...)
#define dbg(...)
#define info(...)
#define warn(...)
#define error(...)
#endif

#endif


/////////////////////////////////////////////////////////////////////////
#pragma mark -
/////////////////////////////////////////////////////////////////////////

typedef NS_ENUM(NSUInteger, EchoLogLevel) {
    kEchoLogLevelTrace=1<<0,
    kEchoLogLevelDebug=1<<1,
    kEchoLogLevelInfo=1<<2,
    kEchoLogLevelWarn=1<<3,
    kEchoLogLevelError=1<<4,
    kEchoLogLevelAll = 0xFFFF
};


/////////////////////////////////////////////////////////////////////////
#pragma mark -
/////////////////////////////////////////////////////////////////////////

@interface EchoLogger : NSObject


+ (instancetype)sharedInstance;

/** Bitflag of levels to show. Default = All */
@property (atomic) NSUInteger logLevels;

/** List of "channels" to exclude. Usually NSString of the file/class names in all caps å*/
@property (atomic, strong) NSArray *excludedChannels;

/** The levels to check along with the exclusions. If the level is not in here then it will be shown. Default = TRACE|DEBUG|INFO|WARN */
@property (atomic) EchoLogLevel logLevelsForExcluded;

/** Assign excluded from dictionary of channel name => BOOL */
- (void)setExcludedChannelsWithDictionary:(NSDictionary *)dictionary;

/** 
 Usually you'll want to use the macros above but it's fine to call manually or to make your own macros
 @param     channel     A label and a grouping. Used in the output and can used to disable groups of logs via the properties above
 @param     line        The line number of the file - __LINE__ from a macro usually
 */
- (void)logWithLevel:(EchoLogLevel)level
             channel:(NSString *)channel
                line:(int)line
             message:(NSString *)msg, ...;


@end
