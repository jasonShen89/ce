//
//  HttpRequest.cpp
//  ce
//
//  Created by kaixuan on 15-7-9.
//  Copyright (c) 2015年 1901. All rights reserved.
//

#include "HttpRequest.h"
#include "CEJsEngine.h"
#import "ASIHTTPRequest.h"
#import "ASIFormDataRequest.h"



NS_CE_BEGIN


HttpRequest::HttpRequest() : _jsListener(NULL)
{}


HttpRequest::~HttpRequest()
{
    _jsListener = NULL;
}


HttpRequest* HttpRequest::createGetRequest(const char *aUrl, const char* aHeaderData, JS_Function aJsListener)
{
    HttpRequest* request = new HttpRequest();
    request->init(aUrl, aHeaderData, NULL, kCEHTTPRequestMethodGET, aJsListener);
    return request;
}


HttpRequest* HttpRequest::createPostRequest(const char *aUrl, const char* aHeaderData, const char* aPostData, JS_Function aJsListener)
{
    HttpRequest* request = new HttpRequest();
    request->init(aUrl, aHeaderData, aPostData, kCEHTTPRequestMethodPOST, aJsListener);
    return request;
}

void HttpRequest::init(const char *aUrl, const char *aHeaderData, const char *aPostData, int aMethod, JS_Function aJsListener)
{
    _url = aUrl;
    _headerData = aHeaderData ? std::string(aHeaderData) : "";
    _postData = aPostData ? std::string(aPostData) : "";
    _method = aMethod;
    _jsListener = aJsListener;
    
}


void HttpRequest::start()
{
    NSURL* url = [NSURL URLWithString:[NSString stringWithUTF8String:_url.c_str()]];

    
    id headers = nil;
    id postData = nil;
    
    if (!_headerData.empty())
    {
        NSData* data = [[NSString stringWithUTF8String:_headerData.c_str()] dataUsingEncoding:NSUTF8StringEncoding];
        NSError* error = nil;
        headers = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
        if (error)
        {
            NSLog(@"invalid http header, Error: %@", [error description]);
        }
    }
    
    if (!_postData.empty())
    {
        NSData* data = [[NSString stringWithUTF8String:_postData.c_str()] dataUsingEncoding:NSUTF8StringEncoding];
        NSError* error = nil;
        postData = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
        if (error)
        {
            NSLog(@"invalid post data, Error: %@", [error description]);
        }
    }
    
    
    
    __block ASIHTTPRequest* request = nil;

    
    if (_method == kCEHTTPRequestMethodGET)
    {
        request = [ASIHTTPRequest requestWithURL:url];

    }
    else if (_method == kCEHTTPRequestMethodPOST)
    {
        request = [ASIFormDataRequest requestWithURL:url];
        for (id key in postData) {
            [(ASIFormDataRequest*)request setPostValue:[postData objectForKey:key] forKey:key];
        }
        
//        for upload file
//         [(ASIFormDataRequest*)request setFile:@"/xxx/test.png" forKey:@"photo"];
    }
    else
    {
        CCLOG("Error: Unsupported method(%d)!", _method);
        return;
    }
    
    for (id key in headers) {
        [request addRequestHeader:key value:[headers objectForKey:key]];
    }
    
    [request setCompletionBlock:^{
        int statusCode = [request responseStatusCode];
        NSData* responseData = [request responseData];
        unsigned char* bytes = (unsigned char*)[responseData bytes];
        this->callbackAndCleanup(statusCode, (const char*)bytes, [responseData length]);
        
//      this->callbackAndCleanup(statusCode, [[request responseString] UTF8String]);
    }];
    
    [request setFailedBlock:^{
        NSError *error = [request error];
        this->callbackAndCleanup(kNetworkErrorCode, [[error description] UTF8String]);
    }];
    
    [request startAsynchronous];
}


void HttpRequest::callbackAndCleanup(int statusCode, const char *result, size_t length)
{
    duk_context* ctx = CEJsEngine::getInstance()->getJsState();
    duk_push_heapptr(ctx, _jsListener);
    duk_push_int(ctx, statusCode);
    duk_push_lstring(ctx, result, (length == -1) ? strlen(result) : length);
    if (duk_pcall(ctx, 2) != DUK_EXEC_SUCCESS)
    {
        CCLOG("Call js function error: %s", duk_to_string(ctx, -1));
    }
    
    CEJsEngine::getInstance()->clean();
    delete this;
}

NS_CE_END