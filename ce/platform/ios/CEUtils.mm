//
//  Utils.m
//  ce
//
//  Created by kaixuan on 15-7-27.
//  Copyright (c) 2015年 1901. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "CEUtils.h"
#include "CEJsEngine.h"



@interface JSTimer : NSObject

@property(assign)JS_Function jsCallback;
@property(strong)NSTimer* timer;

+ (JSTimer*)createTimerWithInterval:(NSTimeInterval)aInterval repeat:(BOOL)aRepeat andCallback:(JS_Function)callback;

- (void)callback;

- (void)cancel;

@end


@implementation JSTimer


+ (JSTimer *)createTimerWithInterval:(NSTimeInterval)aInterval repeat:(BOOL)aRepeat andCallback:(JS_Function)callback {
    JSTimer* timer = [[self alloc] init];
    timer.jsCallback = callback;
    timer.timer = [NSTimer scheduledTimerWithTimeInterval:aInterval target:timer selector:@selector(callback) userInfo:nil repeats:aRepeat];
    return timer;
}

- (void)callback {
    ce::CEJsEngine::getInstance()->callJsFunction(self.jsCallback);
}

- (void)cancel {
    if (self.timer) {
        [self.timer invalidate];
        self.timer = nil;
    }
}

- (void)dealloc {
    [super dealloc];
}

@end



NS_CE_BEGIN


void* CEUtils::createTimer(float interval, bool repeat, JS_Function callback)
{
    return [JSTimer createTimerWithInterval:interval repeat:repeat andCallback:callback];
}


void CEUtils::cancelTimer(void *timer)
{
    [(JSTimer*)timer cancel];
}

NS_CE_END