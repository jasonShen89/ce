//
//  http.cpp
//  ce
//
//  Created by kaixuan on 15-7-9.
//  Copyright (c) 2015年 1901. All rights reserved.
//

#include "http.h"
#include "platform/network/HttpRequest.h"


static int jsb_httpRequest_get(duk_context* ctx)
{
    const char* url = duk_to_string(ctx, 0);
    if (!duk_is_function(ctx, 1))
    {
        CCLOG("arg(2) is't function.");
        return 0;
    }
    
    JS_Function listener = duk_get_heapptr(ctx, 1);
    ce::HttpRequest::createGetRequest(url, NULL, listener)->start();
    return 0;
}


static int jsb_httpRequest_post(duk_context* ctx)
{
    const char* url = duk_to_string(ctx, 0);
    
    const char* postData = NULL;
    if (duk_is_object(ctx, 1))
    {
        postData = duk_json_encode(ctx, 1);
    }
    else if (duk_is_string(ctx, 1))
    {
        postData = duk_to_string(ctx, 1);
    }
    
    if (!duk_is_function(ctx, 2))
    {
        CCLOG("arg(3) is't function.");
        return 0;
    }
    
    JS_Function listener = duk_get_heapptr(ctx, 2);
    ce::HttpRequest::createPostRequest(url, NULL, postData, listener)->start();
    return 0;
}



static duk_function_list_entry http_funcs[] = {
    { "get", jsb_httpRequest_get, 2 },
    { "post", jsb_httpRequest_post, 3 },
    { NULL, NULL, 0 }
};

int js_register_http(duk_context* ctx)
{
    duk_push_global_object(ctx);
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, http_funcs);
    duk_put_prop_string(ctx, -2, "http");
    duk_pop(ctx);  /* pop global */
    return 1;
}