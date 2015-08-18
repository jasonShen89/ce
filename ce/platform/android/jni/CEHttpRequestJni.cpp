#include <stdlib.h>
#include <android/log.h>
#include <string>
#include <map>
#include "common.h"
#include "JniHelper.h"
#include "CEHttpRequestJni.h"


static const char* HTTP_RESPONSE_HANDLER_CLASS_NAME = "org/unify/http/CEHttpResponseHandler";
static const char* HTTP_CLIENT_CLASS_NAME = "org/unify/http/SimpleHttpClient";


static std::map<std::string, ce::HttpRequest*> s_requests;

using namespace ce;


static std::string ptrString(void* p)
{
    char ptr[64] = { 0 };
    sprintf(ptr, "%p", p);
    return std::string(ptr);
}


extern "C" {

    JNIEXPORT void JNICALL Java_org_unify_http_CEHttpResponseHandler_onJniSuccess(JNIEnv*  env, jclass thiz, jstring context, jint statusCode, jstring response) {
        std::string ctx = JniHelper::jstring2string(context);
        HttpRequest* request = s_requests[ctx];
        if (request)
        {
            std::string s = JniHelper::jstring2string(response);
            request->callbackAndCleanup(statusCode, s.c_str());
            s_requests.erase(ctx);
        }
    }

    JNIEXPORT void JNICALL Java_org_unify_http_CEHttpResponseHandler_onJniFailure(JNIEnv*  env, jclass thiz, jstring context, jint statusCode, jstring errorMessage) {
        std::string ctx = JniHelper::jstring2string(context);
        HttpRequest* request = s_requests[ctx];
        if (request)
        {
            std::string s = JniHelper::jstring2string(errorMessage);
            request->callbackAndCleanup(statusCode, s.c_str());
            s_requests.erase(ctx);
        }
    }
}



static jobject createResponseHandler(const char* context)
{
    JniMethodInfo responseConstructMI;
    if (JniHelper::getMethodInfo(responseConstructMI, HTTP_RESPONSE_HANDLER_CLASS_NAME, "<init>", "()V")) 
    {
        JNIEnv* env = responseConstructMI.env;
        jobject responseObj = env->NewObject(responseConstructMI.classID, responseConstructMI.methodID);
        jfieldID contextFieldId = env->GetFieldID(responseConstructMI.classID, "context", "Ljava/lang/String;");
        jstring ctx = env->NewStringUTF(context);
        env->SetObjectField(responseObj, contextFieldId, ctx);
        env->DeleteLocalRef(ctx);
        env->DeleteLocalRef(responseConstructMI.classID);
        return responseObj;
    }
    else
    {
        CCLOG("Can't find java method: %s (%s)", "<init>", HTTP_RESPONSE_HANDLER_CLASS_NAME);
    }
    return NULL;
}


void httpGetJni(ce::HttpRequest* request)
{
    JniMethodInfo t;
    const char* methodName = "get";
    if (JniHelper::getStaticMethodInfo(t, HTTP_CLIENT_CLASS_NAME, methodName, "(Ljava/lang/String;Lcom/loopj/android/http/AsyncHttpResponseHandler;)V")) 
    {
        std::string context = ptrString(request);
        jobject responseHandler = createResponseHandler(context.c_str());
        if (responseHandler)
        {
            // save request with request point address
            s_requests[context] = request;

            // call java 
            jstring arg1 = t.env->NewStringUTF(request->getUrl().c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID, arg1, responseHandler);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(arg1);
            t.env->DeleteLocalRef(responseHandler);
        }
    }
    else
    {
        CCLOG("Can't find java method: %s (%s)", methodName, HTTP_CLIENT_CLASS_NAME);
    }
}

void httpPostJni(ce::HttpRequest* request)
{
    JniMethodInfo t;
    const char* methodName = "post";
    if (JniHelper::getStaticMethodInfo(t, HTTP_CLIENT_CLASS_NAME, methodName, "(Ljava/lang/String;Ljava/lang/String;Lcom/loopj/android/http/AsyncHttpResponseHandler;)V")) 
    {
        std::string context = ptrString(request);
        jobject responseHandler = createResponseHandler(context.c_str());
        if (responseHandler)
        {
            // save request with request point address
            s_requests[context] = request;

            // call java 
            jstring arg1 = t.env->NewStringUTF(request->getUrl().c_str());
            jstring arg2 = t.env->NewStringUTF(request->getPostData().c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID, arg1, arg2, responseHandler);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(arg1);
            t.env->DeleteLocalRef(arg2);
            t.env->DeleteLocalRef(responseHandler);
        }
    }
    else
    {
        CCLOG("Can't find java method: %s (%s)", methodName, HTTP_CLIENT_CLASS_NAME);
    }
}

