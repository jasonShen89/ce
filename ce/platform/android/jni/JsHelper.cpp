#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>
#include "JniHelper.h"
#include "CELibHelper.h"
#include "platform/NotificationCenter.h"

#define  LOG_TAG    "JsHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)


#include "CEJsEngine.h"

using namespace ce;



static CEJsValue jobject2jsvalue(JNIEnv* env, jobject obj)
{
    std::pair<bool, std::string> ret_bs = JniHelper::jobject2string(env, obj);
    if (ret_bs.first) {
        return JS_STRING(ret_bs.second.c_str());
    }

    std::pair<bool, int> ret_is = JniHelper::jobject2int(env, obj);
    if (ret_is.first) {
        return JS_NUMBER(ret_is.second);
    }

    std::pair<bool, double> ret_ns = JniHelper::jobject2number(env, obj);
    if (ret_ns.first) {
        return JS_NUMBER(ret_ns.second);
    }

    std::pair<bool, bool> ret_bb = JniHelper::jobject2bool(env, obj);
    if (ret_bb.first) {
        return JS_BOOL(ret_bb.second);
    } 

    return JS_NULL;  
}


static jobject jsvalue2jobject(const CEJsValue& value)
{
    switch (value.getType())
    {
        case JsValueTypeNumber:
            return JniHelper::number2jobject(value.numberValue());

        case JsValueTypeBool:
            return JniHelper::bool2jobject(value.boolValue());

        case JsValueTypeString:
            return JniHelper::string2jobject(value.cstringValue());

        case JsValueTypeArray:
        case JsValueTypeMap:
            return JniHelper::string2jobject(value.toString().c_str());

        default:
            break;
    }

    return NULL;
}



extern "C" {

    JNIEXPORT int JNICALL Java_org_unify_helper_JsHelper_executeJsString(JNIEnv*  env, jobject thiz, jstring source) {
        std::string s = JniHelper::jstring2string(source);
        return CEJsEngine::getInstance()->executeString(s.c_str());
    }


    JNIEXPORT int JNICALL Java_org_unify_helper_JsHelper_executeJsFile(JNIEnv*  env, jobject thiz, jstring jsFile) {
        std::string s = JniHelper::jstring2string(jsFile);
        return CEJsEngine::getInstance()->executeJsFile(s.c_str());
    }

    
    JNIEXPORT void JNICALL Java_org_unify_helper_JsHelper_addNotificationObserver(JNIEnv*  env, jobject thiz, jstring name, jstring className) {
        std::string noticeName = JniHelper::jstring2string(name);
        std::string observerClassName = JniHelper::jstring2string(className);
        NotificationCenter::getInstance()->addObserverForJni(noticeName.c_str(), observerClassName.c_str());
    }

    JNIEXPORT void JNICALL Java_org_unify_helper_JsHelper_addNotificationObservers(JNIEnv*  env, jobject thiz, jobjectArray names, jstring className) {
        std::string observerClassName = JniHelper::jstring2string(className);

        std::vector<std::string> v;
        jsize count = env->GetArrayLength(names);
        for (int i = 0; i < count; ++i)
        {
            jstring str = (jstring)(env->GetObjectArrayElement(names, i));
            v.push_back(JniHelper::jstring2string(str));
        }

        NotificationCenter::getInstance()->addObserversForJni(v, observerClassName.c_str());
    }
    

    JNIEXPORT void JNICALL Java_org_unify_helper_JsHelper_removeNotificationObserver(JNIEnv*  env, jobject thiz, jstring name, jstring className) {
        std::string arg1 = JniHelper::jstring2string(name);
        std::string arg2 = JniHelper::jstring2string(className);
        NotificationCenter::getInstance()->removeObserverForJni(arg1.c_str(), arg2.c_str());
    }


    JNIEXPORT void JNICALL Java_org_unify_helper_JsHelper_removeAllNotifications(JNIEnv*  env, jobject thiz, jstring className) {
        std::string arg = JniHelper::jstring2string(className);
        NotificationCenter::getInstance()->removeAllObserversForClassJni(arg.c_str());
    }


    JNIEXPORT jobject JNICALL Java_org_unify_helper_JsHelper_callGlobalFunction(JNIEnv*  env, jobject thiz, jstring functionName, jobjectArray args) {
        std::string fn = JniHelper::jstring2string(functionName);

        std::vector<CEJsValue> v;
        jsize count = env->GetArrayLength(args);
        for (int i = 0; i < count; ++i)
        {
            jobject element = env->GetObjectArrayElement(args, i);
            v.push_back(jobject2jsvalue(env, element));
        }

        CEJsValue result = CEJsEngine::getInstance()->callGlobalFunction(fn.c_str(), v);
        return jsvalue2jobject(result);
    }


    JNIEXPORT jobject JNICALL Java_org_unify_helper_JsHelper_callProxy(JNIEnv* env, jobject thiz, jstring proxyName, jstring functionName, jobjectArray args) {
        std::string pn = JniHelper::jstring2string(proxyName);
        std::string fn = JniHelper::jstring2string(functionName);

        std::vector<CEJsValue> v;
        jsize count = env->GetArrayLength(args);
        for (int i = 0; i < count; ++i)
        {
            jobject element = env->GetObjectArrayElement(args, i);
            v.push_back(jobject2jsvalue(env, element));
        }

        CEJsValue result = CEJsEngine::getInstance()->callProxy(pn.c_str(), fn.c_str(), v);   
        return jsvalue2jobject(result);
    }

}
