//
//  fs.h
//  ce
//
//  Created by kaixuan on 15-6-12.
//  Copyright (c) 2015年 kaixuan. All rights reserved.
//

#ifndef __ce__fs__
#define __ce__fs__

#if __cplusplus
extern "C" {
#endif
    
#include "duktape.h"
    
#if __cplusplus
}
#endif



int js_register_fs(duk_context* J);


#endif /* defined(__ce__) */
