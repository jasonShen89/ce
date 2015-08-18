//
//  HttpRequestAndroid.cpp
//  ce
//
//  Created by kaixuan on 15-7-9.
//  Copyright (c) 2015年 1901. All rights reserved.
//

#include "HttpRequest.h"
#include "CEJsEngine.h"
#include "jni/CEHttpRequestJni.h"



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
    if (_method == kCEHTTPRequestMethodGET)
    {
        httpGetJni(this);
    }
    else if (_method == kCEHTTPRequestMethodPOST)
    {
        httpPostJni(this);
    }
    else
    {
        CCLOG("Error: Unsupported method(%d)!", _method);
        return;
    }
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
    
    delete this;
}


NS_CE_END