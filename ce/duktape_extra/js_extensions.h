//
//  js_extensions.h
//  testJS
//
//  Created by kaixuan on 15-6-12.
//  Copyright (c) 2015年 kaixuan. All rights reserved.
//

#ifndef __mujs_extensions__
#define __mujs_extensions__

#if __cplusplus
extern "C" {
#endif
    
#include "duktape.h"
void jsopen_extensions(duk_context* ctx);
#if __cplusplus
}
#endif

#endif /* defined(__mujs_extensions__) */
