//
//  Utils.h
//  ce
//
//  Created by kaixuan on 15-7-27.
//  Copyright (c) 2015年 1901. All rights reserved.
//

#ifndef __ce__Utils__
#define __ce__Utils__

#include <iostream>
#include "common.h"
#include "duktapeExtra.h"

NS_CE_BEGIN

class CEUtils
{
public:
    static void* createTimer(float interval, bool repeat, JS_Function callback);
    static void cancelTimer(void* timer);
};

NS_CE_END

#endif /* defined(__ce__Utils__) */
