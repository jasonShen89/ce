//
//  NotificationCenter.cpp
//  ce
//
//  Created by kaixuan on 15-6-29.
//  Copyright (c) 2015年 1901. All rights reserved.
//

#include "NotificationCenter.h"
#include "CEJsEngine.h"

#if UNIFY_IOS
#include <objc/message.h>

typedef void (*notifyFunc)(id, SEL, const char*, const char*);
static notifyFunc objc_callback = (notifyFunc)objc_msgSend;


typedef void (*iosNotifyFunc)(id , SEL,id,const char *,const char *);
static iosNotifyFunc ios_objc_callback = (iosNotifyFunc)objc_msgSend;

#elif UNIFY_ANDROID
#include "jni/CELibHelper.h"
#endif


NS_CE_BEGIN

static NotificationCenter* _instance = NULL;


NotificationCenter::NotificationCenter()
{}


NotificationCenter::~NotificationCenter()
{
    _observers.clear();
}

//to do delete
void NotificationCenter::list_____list(){
    std::map<std::string,std::vector<observer>>::iterator itr1  =  _observers.begin();
    std::cout <<  "============= start" << std::endl ;
    for(  ;  itr1  !=  _observers.end();++itr1)
    {
       // CCLOG("")
        
        std::cout << itr1->first << std::endl;
        std::cout << (&itr1->second)->size() << std::endl;
        
    }
    
    std::cout <<  "=============  end" <<std::endl;
    
    
}

NotificationCenter* NotificationCenter::getInstance()
{
    if (!_instance)
    {
        _instance = new NotificationCenter();
    }
    return _instance;
}

void NotificationCenter::destroyInstance()
{
    CC_SAFE_DELETE(_instance);
}


//void NotificationCenter::testCallObjc(id target, void *selector)
//{
//    typedef void (*send_type)(void*, void*, const char*);
//    
//    send_type func = (send_type)objc_msgSend;
//    func((void*)target, selector, "hello");
//}
//
//void NotificationCenter::addObserver(const char *name, void *obs, callback_string_string callback)
//{
//    observer o;
//    o.func = callback;
//    _observers[name].push_back(o);
//}
//
//
//void NotificationCenter::addObservers(const std::vector<std::string> &names, void *obs, callback_string_string callback)
//{
//    for (auto name : names)
//    {
//        addObserver(name.c_str(), obs, callback);
//    }
//}


void NotificationCenter::removeObserverByCondition(const char *name, removecheckFunc checker)
{
    std::vector<observer>& obs = _observers[name];
    if (!obs.empty())
    {
        int i = 0;
        std::vector<int> delIndexs;
        for (auto o : obs)
        {
            if (checker(o)) { delIndexs.push_back(i); }
            i++;
        }
        
        for (int i : delIndexs)
        {
            obs.erase(obs.begin() + i);
        }
    }
}

void NotificationCenter::removeAllObserversByCondition(removecheckFunc checker)
{
    for (auto obs : _observers)
    {
        int i = 0;
        std::vector<int> delIndexs;
        for (auto o : obs.second)
        {
            if (checker(o)) { delIndexs.push_back(i); }
            i++;
        }
        
        for (int i : delIndexs)
        {
            obs.second.erase(obs.second.begin() + i);
        }
    }
}


#if UNIFY_IOS
//void NotificationCenter::addObserverWithBlock(const char* name, id observer, IOSCallBackBlock block){
//    NotificationCenter::observer o;
//    o.objcTarget = observer;
//    o.objBlock = block;
//    _observers[name].push_back(o);
//    
//    
//    //print
//    //list_____list();
//    
//    
//    
//}
//void NotificationCenter::addObserversWithBlock(const std::vector<std::string>& names, id observer,IOSCallBackBlock block){
//    for (auto name : names){
//        addObserverWithBlock(name.c_str(), observer, block);
//    }
//}
//
//void NotificationCenter::removeObserverWithBlock(const char *name,id observer){
//    //to do add
//}


//多传个target
void NotificationCenter::addObserver(const char* name, id target, id objsLastTarget,SEL selector){
    NotificationCenter::observer o;
    o.objcTarget = target;
    o.objcLastTarget = objsLastTarget;
    o.objcCallback = selector;
    _observers[name].push_back(o);
    
    list_____list();
}


void NotificationCenter::removeObservers(const char *name,id target,id objsLastTarget){
    
    //to do kaixuan 整理
    
    std::map<std::string,std::vector<observer>>::iterator itr1  =  _observers.begin();
    std::cout <<  "============= start" << std::endl ;
    for(  ;  itr1  !=  _observers.end();++itr1)
    {
        // CCLOG("")
        
        std::cout << itr1->first << std::endl;
        std::cout << (&itr1->second)->size() << std::endl;
        
        if (itr1->first != name) {
            continue;
        }
        
        int i = 0;
        std::vector<int> delIndexs;
        for (auto o : itr1->second) {
            if (o.objcLastTarget == objsLastTarget && o.objcTarget == target) {
                delIndexs.push_back(i);
            }
            i++;
            for (int i : delIndexs)
            {
                itr1->second.erase(itr1->second.begin() + i);
            }
            
        }
        
    }
    
    CCLOG("删除");
    list_____list();
  

}

//
 

void NotificationCenter::addObserver(const char *name, id observer, SEL selector)
{
    
    NotificationCenter::observer o;
    o.objcTarget = observer;
    o.objcCallback = selector;
    _observers[name].push_back(o);
}

void NotificationCenter::addObservers(const std::vector<std::string> &names, id observer, SEL selector)
{
    for (auto name : names)
    {
        addObserver(name.c_str(), observer, selector);
    }
}

void NotificationCenter::removeObserver(const char *name, id target, SEL selector)
{
    this->removeObserverByCondition(name, [target, selector](observer& o) {
        return (o.objcTarget == target && o.objcCallback == selector);
    });
}

void NotificationCenter::removeAllObserversForTarget(const char *name, id target)
{
    this->removeAllObserversByCondition([target](observer& o) {
        return (o.objcTarget == target);
    });
}

#elif UNIFY_ANDROID

void NotificationCenter::addObserverForJni(const char *name, const char *className)
{
    observer o;
    o.className = className;
    _observers[name].push_back(o);
}

void NotificationCenter::addObserversForJni(const std::vector<std::string> &names, const char *className)
{
    for (auto name : names)
    {
        addObserverForJni(name.c_str(), className);
    }
}

void NotificationCenter::removeObserverForJni(const char *name, const char *className)
{
    this->removeObserverByCondition(name, [className](observer& o) {
        return (o.className == className);
    });
}

void NotificationCenter::removeAllObserversForClassJni(const char *className)
{
    this->removeAllObserversByCondition([className](observer& o) {
        return (o.className == className);
    });
}

#endif



void NotificationCenter::addObserverForJs(const char *name, const char *tag, JS_Function callback)
{
    observer o;
    o.tag = tag;
    o.jsCallback = callback;
    _observers[name].push_back(o);
}


void NotificationCenter::addObserversForJs(const std::vector<std::string> &names, const char *tag, JS_Function callback)
{
    for (auto name : names)
    {
        addObserverForJs(name.c_str(), tag, callback);
    }
}


void NotificationCenter::removeObserverForJs(const char *name, const char *tag)
{
    this->removeObserverByCondition(name, [tag](observer& o) {
        return (o.tag == tag);
    });
}



void NotificationCenter::notify(const char *name, const char* body)
{
    for (auto obs : _observers[name])
    {
        if (obs.func)
        {
            obs.func(name, body);
        }
        else if (obs.jsCallback)
        {
            CEJsEngine::getInstance()->callJsFunction(obs.jsCallback, { JS_STRING(name), JS_STRING(body) });
        }
#if UNIFY_IOS
//        else if (obs.objBlock && obs.objcTarget){
//            obs.objBlock(name,body);
//            //objc_callback(obs.objcTarget, obs.objBlock, name, body);
//        }
        else if (obs.objcTarget && obs.objcLastTarget && obs.objcCallback){
            ios_objc_callback(obs.objcTarget,obs.objcCallback,obs.objcLastTarget,name,body);
        }
//        else if (obs.objcTarget && obs.objcCallback){
//            objc_callback(obs.objcTarget, obs.objcCallback,name, body);
//        }
#elif UNIFY_ANDROID
        else if (!obs.className.empty())
        {
            notifyToJni(name, obs.className.c_str(), body);
        }
#endif
    }
}
    
NS_CE_END