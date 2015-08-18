//
//  CEJsEngine.cpp
//  ce
//
//  Created by kaixuan on 15-6-15.
//  Copyright (c) 2015年 kaixuan. All rights reserved.
//

#include "CEJsEngine.h"
#include <algorithm>

#include "CEJsValue.h"
#include "js_extensions.h"
#include "platform/CCFileUtils.h"



NS_CE_BEGIN

static int js_pushCEJsValue(duk_context* J, const CEJsValue& value)
{
    int result = 1;
    
    CEJsValueType type = value.getType();
    switch (type) {
        case JsValueNone:
            result = 0;
            break;
        case JsValueTypeUndefined:
            duk_push_undefined(J);
            break;
        case JsValueTypeNull:
            duk_push_null(J);
            break;
        case JsValueTypeBool:
            duk_push_boolean(J, value.boolValue());
            break;
        case JsValueTypeNumber:
            duk_push_number(J, value.numberValue());
            break;
        case JsValueTypeString:
            duk_push_string(J, value.stringValue().c_str());
            break;
        default:
            break;
    }
    
    return result;
}




CEJsEngine* CEJsEngine::_instance = NULL;


CEJsEngine* CEJsEngine::getInstance(void)
{
    if (!_instance)
    {
        _instance = new CEJsEngine();
        _instance->init();
    }
    return _instance;
}


bool CEJsEngine::init(void)
{
    setScriptPath("scripts/");
    ctx = duk_create_heap_default();
    jsopen_extensions(ctx);
    return true;
}


CEJsEngine::~CEJsEngine()
{
    duk_destroy_heap(ctx);
    ctx = NULL;
}


void CEJsEngine::setScriptPath(const char *path)
{
    if (!_scriptPath.empty())
    {
        // remove old script path from search paths
        std::vector<std::string> newPaths = CCFileUtils::getInstance()->getSearchPaths();
        auto foundIter = newPaths.erase(std::find(newPaths.begin(), newPaths.end(), _scriptPath));
        if (foundIter != newPaths.end()) { newPaths.erase(foundIter); }
        CCFileUtils::getInstance()->setSearchPaths(newPaths);
    }
    
    _scriptPath = std::string(path);
    CCFileUtils::getInstance()->addSearchPath(path);
}



int CEJsEngine::executeString(const char *source)
{
    int result = duk_peval_string(ctx, source);
    if (result == DUK_EXEC_ERROR) { printf("executeString Error: %s\n", duk_to_string(ctx, -1)); }
    duk_pop(ctx);
    return result;
}

int CEJsEngine::executeJsFile(const char *filename)
{
    std::string filePath = ce::CCFileUtils::getInstance()->fullPathForFilename(filename);
    int result = duk_peval_file(ctx, filePath.c_str());
    if (result == DUK_EXEC_ERROR) { printf("executeJsFile Error: %s\n", duk_to_string(ctx, -1)); }
    duk_pop(ctx);
    return result;
}


void CEJsEngine::callJsFunction(JS_Function func)
{
    callJsFunction(func, { JS_NONE });
}

void CEJsEngine::callJsFunction(JS_Function func, const std::vector<CEJsValue> &args)
{
    duk_push_heapptr(ctx, func);
    
    int argNum = 0;
    for (auto arg : args)
    {
        argNum += js_pushCEJsValue(ctx, arg);
    }

    if (duk_pcall(ctx, argNum) != DUK_EXEC_SUCCESS)
    {
        CCLOG("callJsFunction Error: %s", duk_to_string(ctx, -1));
    }
    
    clean();
}


CEJsValue CEJsEngine::callGlobalFunction(const char* functionName)
{
    std::vector<CEJsValue> args;
    return callGlobalFunction(functionName, args);
}

//CEJsValue CEJsEngine::callGlobalFunction(const char* functionName, const CEJsValue& a)
//{
//    return callGlobalFunction(functionName, { a });
//}


static duk_ret_t js_callback_bind(duk_context* ctx)
{
//    int i = duk_to_(ctx);
//    const char* a = duk_to_string(ctx, 0);
    return 0;
}

//static duk_c_function generate_callback(duk_context* ctx, std::function<void(const char*)> func)
//{
//    return js_callback_bind;
//}

//CEJsValue CEJsEngine::callGlobalFunction(const char* functionName, const CEJsValue& a, std::function<void(const char*)> func)
//{
//    bool ret = duk_get_global_string(ctx, functionName);
//    if (!ret || !duk_is_function(ctx, 0))
//    {
//        printf("Can't find funciton: %s\n", functionName);
//        duk_pop(ctx);
//        return JS_NULL;
//    }
// 
//    CEJsValue result;
//    int argNum = 2;
//    js_pushCEJsValue(ctx, a);
//    duk_push_c_function(ctx, js_callback_bind, 1);
//    if (duk_pcall(ctx, argNum) == DUK_EXEC_ERROR)
//    {
//        printf("Error (%s): %s\n", functionName, duk_to_string(ctx, -1));
//    }
//    else
//    {
//        result = js_to_cejs_value(ctx, 0);
//    }
//    
//    duk_pop(ctx);
//    return result;
//}


CEJsValue CEJsEngine::callGlobalFunction(const char *functionName, const std::vector<CEJsValue>& args)
{
    bool ret = duk_get_global_string(ctx, functionName);
    if (!ret || !duk_is_function(ctx, -1))
    {
        CCLOG("Can't find funciton: %s", functionName);
        duk_pop(ctx);
        return JS_NULL;
    }
    
    
    int argNum = 0;
    for (auto arg : args)
    {
        argNum += js_pushCEJsValue(ctx, arg);
    }
    
    CEJsValue result;
    if (duk_pcall(ctx, argNum) == DUK_EXEC_ERROR)
    {
        CCLOG("Error (%s): %s\n", functionName, duk_to_string(ctx, -1));
    }
    else
    {
        result = js_to_cejs_value(ctx, -1);
    }
    duk_pop(ctx);
    clean();
    
//    printf("callGlobalFunction: %d\n", duk_get_top(J));
    return result;
}




CEJsValue CEJsEngine::callProxy(const char *proxyName, const char *funcName)
{
    std::vector<CEJsValue> args;
    return callProxy(proxyName, funcName, args);
}

CEJsValue CEJsEngine::callProxy(const char *proxyName, const char *funcName, const std::vector<CEJsValue> &args)
{
    std::vector<CEJsValue> clonedArgs(args);
    clonedArgs.insert(clonedArgs.begin(), JS_STRING(funcName));
    clonedArgs.insert(clonedArgs.begin(), JS_STRING(proxyName));
    return this->callGlobalFunction("callProxy", clonedArgs);
}


void CEJsEngine::clean()
{
    duk_set_top(ctx, 0);
}


NS_CE_END
