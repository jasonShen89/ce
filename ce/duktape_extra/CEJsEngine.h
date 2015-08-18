//
//  CEJsEngine.h
//  ce
//
//  Created by kaixuan on 15-6-15.
//  Copyright (c) 2015年 kaixuan. All rights reserved.
//

#ifndef __ce__CEJsEngine__
#define __ce__CEJsEngine__

#include <iostream>
#include "common.h"
#include "duktapeExtra.h"

NS_CE_BEGIN

class CEJsEngine
{
public:
    static CEJsEngine* getInstance(void);
    virtual ~CEJsEngine(void);
    
    inline duk_context* getJsState() const { return ctx; }
    
    
    void setScriptPath(const char* path);
    
    int executeString(const char* source);
    int executeJsFile(const char* filename);
    

    void callJsFunction(JS_Function func);
    void callJsFunction(JS_Function func, const std::vector<CEJsValue>& args);
    
    CEJsValue callGlobalFunction(const char* functionName);
    CEJsValue callGlobalFunction(const char* functionName, const std::vector<CEJsValue>& args);
    
    CEJsValue callProxy(const char* proxyName, const char* funcName);
    CEJsValue callProxy(const char* proxyName, const char* funcName, const std::vector<CEJsValue>& args);
    
    
    
    void clean();
    
private:
    CEJsEngine(void) : ctx(NULL)
    {}
    
    bool init(void);
    
    static CEJsEngine* _instance;
    duk_context* ctx;
    
    std::string _scriptPath;
    
};

NS_CE_END

#endif /* defined(__ce__CEJsEngine__) */
