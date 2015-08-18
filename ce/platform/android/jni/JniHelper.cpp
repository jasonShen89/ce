#include "JniHelper.h"
#include <android/log.h>
#include <string.h>

#if 1
#define  LOG_TAG    "JniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...) 
#endif

#define JAVAVM    ce::JniHelper::getJavaVM()

using namespace std;

extern "C"
{

    //////////////////////////////////////////////////////////////////////////
    // java vm helper function
    //////////////////////////////////////////////////////////////////////////

    static pthread_key_t s_threadKey;

    static void detach_current_thread (void *env) {
        JAVAVM->DetachCurrentThread();
    }
    
    static bool getEnv(JNIEnv **env)
    {
        bool bRet = false;

        switch(JAVAVM->GetEnv((void**)env, JNI_VERSION_1_4))
        {
        case JNI_OK:
            bRet = true;
                break;
        case JNI_EDETACHED:
            pthread_key_create (&s_threadKey, detach_current_thread);
            if (JAVAVM->AttachCurrentThread(env, 0) < 0)
            {
                LOGD("%s", "Failed to get the environment using AttachCurrentThread()");
                break;
            }
            if (pthread_getspecific(s_threadKey) == NULL)
                pthread_setspecific(s_threadKey, env); 
            bRet = true;
            break;
        default:
            LOGD("%s", "Failed to get the environment using GetEnv()");
            break;
        }      

        return bRet;
    }

    static jclass getClassID_(const char *className, JNIEnv *env)
    {
        JNIEnv *pEnv = env;
        jclass ret = 0;

        do 
        {
            if (! pEnv)
            {
                if (! getEnv(&pEnv))
                {
                    break;
                }
            }
            
            ret = pEnv->FindClass(className);
            if (! ret)
            {
                 LOGD("Failed to find class of %s", className);
                break;
            }
        } while (0);

        return ret;
    }

    static bool getStaticMethodInfo_(ce::JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
    {
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;

        do 
        {
            if (! getEnv(&pEnv))
            {
                break;
            }

            jclass classID = getClassID_(className, pEnv);

            methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
            if (! methodID)
            {
                LOGD("Failed to find static method id of %s", methodName);
                break;
            }

            methodinfo.classID = classID;
            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;

            bRet = true;
        } while (0);

        return bRet;
    }

    static bool getMethodInfo_(ce::JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
    {
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;

        do 
        {
            if (! getEnv(&pEnv))
            {
                break;
            }

            jclass classID = getClassID_(className, pEnv);

            methodID = pEnv->GetMethodID(classID, methodName, paramCode);
            if (! methodID)
            {
                LOGD("Failed to find method id of %s", methodName);
                break;
            }

            methodinfo.classID = classID;
            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;

            bRet = true;
        } while (0);

        return bRet;
    }

    static string jstring2string_(jstring jstr)
    {
        if (jstr == NULL)
        {
            return "";
        }
        
        JNIEnv *env = 0;

        if (! getEnv(&env))
        {
            return 0;
        }

        const char* chars = env->GetStringUTFChars(jstr, NULL);
        string ret(chars);
        env->ReleaseStringUTFChars(jstr, chars);

        return ret;
    }
    
    
    JNIEXPORT void JNICALL Java_com_vocinno_utils_jni_Native_initCallBackParams(JNIEnv *env, jobject obj)
    {
        jclass clazz = env->GetObjectClass(obj); //通过类的对象
        
        //void(int)------------------------------------------------------------
        jmethodID mid = env->GetMethodID(clazz,"onNativeCallback","(I)V");//查找java中的show方法的ID，最后的签名符号为void类型
        if(mid == NULL)
        {
            __android_log_print(ANDROID_LOG_INFO,"HGY", "method show ID not found");
            return; //如果方法ID没有找到
        }
        
        __android_log_print(ANDROID_LOG_INFO,"wanggsx", "will execute onNativeCallback function");
        env->CallVoidMethod(obj,mid,159); //执行方法
        //void(int)------------------------------------------------------------
        
        //void(int,int)--------------------------------------------------------
        jmethodID intshowID = env->GetMethodID(clazz,"onNativeCallback1","(II)V");
        if(intshowID == NULL)
        {
            __android_log_print(ANDROID_LOG_INFO,"HGY", "method onNativeCallback1 ID not found");
            return; //如果方法ID没有找到
        }
        __android_log_print(ANDROID_LOG_INFO,"HGY", "will execute onNativeCallback1 function");
        env->CallVoidMethod(obj,intshowID,186,159); //执行方法
        //void(int,int)-------------------------------------------------------
        
        //int(int,int)--------------------------------------------------------
        jmethodID mdid = env->GetMethodID(clazz,"onSumCallback","(II)I");
        if(mdid == NULL)
        {
            __android_log_print(ANDROID_LOG_INFO,"HGY", "method onSumCallback ID not found");
            return; //如果方法ID没有找到
        }
        __android_log_print(ANDROID_LOG_INFO,"HGY", "will execute onSumCallback function");
        env->CallIntMethod(obj,mdid,186,159); //执行onSumCallback方法
        //int(int,int)--------------------------------------------------------
        
        //int(string)--------------------------------------------------------
        jmethodID mdidStr = env->GetMethodID(clazz,"onCallbackString","(Ljava/lang/String;)I");
        if(mdidStr == NULL)
        {
            __android_log_print(ANDROID_LOG_INFO,"HGY", "method onCallbackString ID not found");
            return; //如果方法ID没有找到
        }
        __android_log_print(ANDROID_LOG_INFO,"HGY", "will execute onCallbackString function");
        
        jstring encode = env->NewStringUTF( "哈哈哦来啦");
        
        env->CallIntMethod(obj,mdidStr,encode); //执行onSumCallback方法
        //int(string)--------------------------------------------------------
        
        //int(object)--------------------------------------------------------
        jmethodID mdidobj = env->GetMethodID(clazz,"onCallbackStringFromObj","(Lcom/ticktick/jnicallback/Person;)I");
        if(mdidobj == NULL)
        {
            __android_log_print(ANDROID_LOG_INFO,"HGY", "method onCallbackStringFromObj ID not found");
            return; //如果方法ID没有找到
        }
        
        jclass pm_cls = env->FindClass("com/ticktick/jnicallback/Person");
        jmethodID pmid = env->GetMethodID(pm_cls,"<init>","()V");
        jobject objPN = env->NewObject(pm_cls,pmid);
        
        jfieldID fid_name = env->GetFieldID(pm_cls,"username","Ljava/lang/String;");
        jfieldID fid_id = env->GetFieldID(pm_cls,"age","I");
        
        env->SetObjectField(objPN,fid_name,env->NewStringUTF("姓名wanggsx"));
        env->SetIntField(objPN,fid_id,196);
        
        env->CallIntMethod(obj,mdidobj,objPN); //执行onSumCallback方法
        //(object)---------------------------------------------------------------
        
        //int(object[])--------------------------------------------------------
        jmethodID mdidobjarray = env->GetMethodID(clazz,"onCallbackStringFromObjArray","([Lcom/ticktick/jnicallback/Person;)I");
        if(mdidobjarray == NULL)
        {
            __android_log_print(ANDROID_LOG_INFO,"HGY", "method onCallbackStringFromObj ID not found");
            return; //如果方法ID没有找到
        }
        
        jclass cls_array=env->FindClass("java/lang/Object");
        jobjectArray obj_array=env->NewObjectArray(3,cls_array,0);
        
        jclass pm_cls1 = env->FindClass("com/ticktick/jnicallback/Person");
        jmethodID m = env->GetMethodID(pm_cls1,"<init>","()V");
        
        jfieldID fid_name1 = env->GetFieldID(pm_cls1,"username","Ljava/lang/String;");
        jfieldID fid_id1 = env->GetFieldID(pm_cls1,"age","I");
        
        //第一个对象
        jobject obj11=env->NewObject(pm_cls1,m);
        
        jobject o1=env->NewStringUTF("kwanggsx1");
        env->SetObjectField(obj11,fid_name1,o1);
        env->SetIntField(obj11,fid_id1,159);
        
        env->SetObjectArrayElement(obj_array,0,obj11);
        //第二个对象
        jobject obj2=env->NewObject(pm_cls1,m);
        
        jobject o2=env->NewStringUTF("kwanggsx2");
        env->SetObjectField(obj2,fid_name1,o2);
        env->SetIntField(obj2,fid_id1,160);
        
        env->SetObjectArrayElement(obj_array,1,obj2);
        //第三个对象
        jobject obj3=env->NewObject(pm_cls1,m);
        
        jobject o3=env->NewStringUTF("kwanggsx3");
        env->SetObjectField(obj3,fid_name1,o3);
        env->SetIntField(obj3,fid_id1,161);
        
        env->SetObjectArrayElement(obj_array,2,obj3);
        
        env->CallIntMethod(obj,mdidobjarray,obj_array);//执行onCallbackStringFromListObj方法
        //int(object[])--------------------------------------------------------
        
        //int(object[])--------------------------------------------------------
        jmethodID mdidobjarrayobj = env->GetMethodID(clazz,"onCallbackStringFromObjArrayObj","([Ljava/lang/Object;)I");
        if(mdidobjarrayobj == NULL)
        {
            __android_log_print(ANDROID_LOG_INFO,"HGY", "method onCallbackStringFromObjArrayObj ID not found");
            return; //如果方法ID没有找到
        }
        
        jclass cls_arrayobj=env->FindClass("java/lang/Object");
        jobjectArray obj_arrayobj=env->NewObjectArray(5,cls_arrayobj,0);
        
        jclass pm_cls1obj = env->FindClass("com/ticktick/jnicallback/Person");
        jmethodID mobj = env->GetMethodID(pm_cls1obj,"<init>","()V");
        
        jfieldID fid_name1obj = env->GetFieldID(pm_cls1obj,"username","Ljava/lang/String;");
        jfieldID fid_id1obj = env->GetFieldID(pm_cls1obj,"age","I");
        
        //第一个对象
        jobject obj11obj=env->NewObject(pm_cls1obj,m);
        
        jobject o1obj=env->NewStringUTF("kwanggsx1");
        env->SetObjectField(obj11obj,fid_name1obj,o1obj);
        env->SetIntField(obj11obj,fid_id1obj,1591);
        
        env->SetObjectArrayElement(obj_arrayobj,0,obj11obj);
        //第二个对象
        jobject obj2obj=env->NewObject(pm_cls1obj,mobj);
        
        jobject o2obj=env->NewStringUTF("kwanggsx2");
        env->SetObjectField(obj2obj,fid_name1obj,o2obj);
        env->SetIntField(obj2obj,fid_id1obj,1601);
        
        env->SetObjectArrayElement(obj_arrayobj,1,obj2obj);
        //第三个对象
        jobject obj3obj=env->NewObject(pm_cls1obj,mobj);
        
        jobject o3obj=env->NewStringUTF("kwanggsx3");
        env->SetObjectField(obj3obj,fid_name1obj,o3obj);
        env->SetIntField(obj3obj,fid_id1obj,1611);
        
        env->SetObjectArrayElement(obj_arrayobj,2,obj3obj);
        //第四个对象：字符串
        jobject obj4obj = env->NewStringUTF( "我是第四个对象，就是一个字符串");
        env->SetObjectArrayElement(obj_arrayobj,3,obj4obj);
        
        //第五个对象：int
        //获取Integer对象的class类型
        jclass integer_class = env->FindClass("java/lang/Integer");
        //获取Integer对象的构造方法ID
        //new一个Integer对象，是要传入一个int数字，所以在它的构造方法里面需要使用"I"(大写的I)，注意啊。
        jmethodID integer_init =env->GetMethodID(integer_class,"<init>","(I)V");
        //生成integer对象
        jobject obj5obj = env->NewObject(integer_class,integer_init,1234567890);
        
        env->SetObjectArrayElement(obj_arrayobj,4,obj5obj);
        
        env->CallIntMethod(obj,mdidobjarrayobj,obj_arrayobj);//执行onCallbackStringFromListObj方法
        //int(object[])--------------------------------------------------------
    }
}

NS_CE_BEGIN

JavaVM* JniHelper::m_psJavaVM = NULL;

JavaVM* JniHelper::getJavaVM()
{
    return m_psJavaVM;
}

void JniHelper::setJavaVM(JavaVM *javaVM)
{
    m_psJavaVM = javaVM;
}

jclass JniHelper::getClassID(const char *className, JNIEnv *env)
{
    return getClassID_(className, env);
}

bool JniHelper::getStaticMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
{
    return getStaticMethodInfo_(methodinfo, className, methodName, paramCode);
}

bool JniHelper::getMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
{
    return getMethodInfo_(methodinfo, className, methodName, paramCode);
}

string JniHelper::jstring2string(jstring str)
{
    return jstring2string_(str);
}


std::pair<bool, int> JniHelper::jobject2int(JNIEnv* env, jobject obj)
{
    jmethodID methodId = NULL;
    jclass integer_cls = env->FindClass("java/lang/Integer");

    if (env->IsInstanceOf(obj, integer_cls))
    {
        methodId = env->GetMethodID(integer_cls, "intValue", "()I");
        int value = env->CallIntMethod(obj, methodId);
        return std::make_pair(true, value);
    }
    
    return std::make_pair(false, 0);
}


std::pair<bool, double> JniHelper::jobject2number(JNIEnv* env, jobject obj)
{
    jmethodID methodId = NULL;
    jclass float_cls = env->FindClass("java/lang/Float");
    jclass double_cls = env->FindClass("java/lang/Double");

    if (env->IsInstanceOf(obj, float_cls))
    {
        methodId = env->GetMethodID(float_cls, "doubleValue", "()D");
        double value = env->CallDoubleMethod(obj, methodId);
        return std::make_pair(true, value);
    }
    else if (env->IsInstanceOf(obj, double_cls))
    {
        methodId = env->GetMethodID(double_cls, "doubleValue", "()D");
        double value = env->CallDoubleMethod(obj, methodId);
        return std::make_pair(true, value);    
    }
    
    return std::make_pair(false, 0.0);
}


std::pair<bool, bool> JniHelper::jobject2bool(JNIEnv* env, jobject obj)
{
    jmethodID methodId = NULL;
    jclass cls = env->FindClass("java/lang/Boolean");

    if (env->IsInstanceOf(obj, cls))
    {
        methodId = env->GetMethodID(cls, "booleanValue", "()Z");
        bool value = env->CallBooleanMethod(obj, methodId);
        return std::make_pair(true, value);
    }
    
    return std::make_pair(false, false);
}


std::pair<bool, std::string> JniHelper::jobject2string(JNIEnv* env, jobject obj)
{
    jmethodID methodId = NULL;
    jclass cls = env->FindClass("java/lang/String");

    if (env->IsInstanceOf(obj, cls))
    {
        return std::make_pair(true, JniHelper::jstring2string((jstring)obj));
    }
    
    return std::make_pair(false, "");
}



jobject JniHelper::int2jobject(int arg)
{
    JniMethodInfo constructMI;
    if (JniHelper::getMethodInfo(constructMI, "java/lang/Integer", "<init>", "(I)V"))
    {
        jobject result = constructMI.env->NewObject(constructMI.classID, constructMI.methodID, arg);
        constructMI.env->DeleteLocalRef(constructMI.classID);
        return result;
    }
    return NULL;
}

jobject JniHelper::number2jobject(double arg)
{
    JniMethodInfo constructMI;
    if (JniHelper::getMethodInfo(constructMI, "java/lang/Double", "<init>", "(D)V"))
    {
        jobject result = constructMI.env->NewObject(constructMI.classID, constructMI.methodID, arg);
        constructMI.env->DeleteLocalRef(constructMI.classID);
        return result;
    }
    return NULL;
}

jobject JniHelper::bool2jobject(bool arg)
{
    JniMethodInfo constructMI;
    if (JniHelper::getMethodInfo(constructMI, "java/lang/Boolean", "<init>", "(Z)V"))
    {
        jobject result = constructMI.env->NewObject(constructMI.classID, constructMI.methodID, arg);
        constructMI.env->DeleteLocalRef(constructMI.classID);
        return result;
    }
    return NULL;
}

jobject JniHelper::string2jobject(const char* arg)
{
    JNIEnv* env = NULL;
    if (! getEnv(&env)) return NULL;
    return env->NewStringUTF(arg);
}



NS_CE_END
