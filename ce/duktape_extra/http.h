//
//  http.h
//  ce
//
//  Created by kaixuan on 15-7-9.
//  Copyright (c) 2015年 1901. All rights reserved.
//

#ifndef __ce__http__
#define __ce__http__

#if __cplusplus
extern "C" {
#endif
    
#include "duktape.h"
    
#if __cplusplus
}
#endif



int js_register_http(duk_context* ctx);


#endif /* defined(__ce__http__) */
