#ifndef __ANDROID_JNI_HELPER_H__
#define __ANDROID_JNI_HELPER_H__

#include <string>
#include <jni.h>
#include "platform/CCPlatformMacros.h"

NS_CE_BEGIN

typedef struct JniMethodInfo_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;

class CC_DLL JniHelper
{
public:
    //获取虚拟机参数
    static JavaVM* getJavaVM();
    //设置虚拟机参数
    static void setJavaVM(JavaVM *javaVM);
    //获取java类的实例id
    static jclass getClassID(const char *className, JNIEnv *env=0);
    //检测java类中是否有对应的静态方法
    static bool getStaticMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode);
    //检测java类中是否有对应的方法
    static bool getMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode);
    //把jstring转换为C中的string
    static std::string jstring2string(jstring str);

    // convert jobject to primitive value
    // return std::pair<is success, result>
    static std::pair<bool, int> jobject2int(JNIEnv* env, jobject obj);
    static std::pair<bool, double> jobject2number(JNIEnv* env, jobject obj);
    static std::pair<bool, bool> jobject2bool(JNIEnv* env, jobject obj);
    static std::pair<bool, std::string> jobject2string(JNIEnv* env, jobject obj);

    // convert primitive value to jobject
    static jobject int2jobject(int arg);
    static jobject number2jobject(double arg);
    static jobject bool2jobject(bool arg);
    static jobject string2jobject(const char* arg);

private:
    static JavaVM *m_psJavaVM;
    static jobject *jobj;
};

NS_CE_END

#endif // __ANDROID_JNI_HELPER_H__
