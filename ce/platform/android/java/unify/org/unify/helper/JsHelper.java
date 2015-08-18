package org.unify.helper;

//import android.os.Message;

public class JsHelper {

	// ===========================================================
	// Native Methods : Java -->> C++ -->> javaScript
	// ===========================================================

	// 执行JS代码，参数为JS代码
	public static native int executeJsString(final String source);

	// 执行js文件，参数为js的完整路径(apk和项目里的文件不能调用)
	public static native int executeJsFile(final String jsFile);

	// 调用C/C++的全局方法
	public static native Object callGlobalFunction(final String functionName,
			final Object... args);

	// 调用具体某个C/C++类中的方法
	public static native Object callProxy(final String proxyName,
			final String functionName, final Object... args);
	
	public static native void addNotificationObserver(final String name, final String observerClassName);
	
	public static native void addNotificationObservers(final String[] names, final String observerClassName);
	
	public static native void removeNotificationObserver(final String name, final String observerClassName);
	
	public static native void removeAllNotifications(final String observerClassName);
	
	
	

	// ===========================================================
	// Native Methods : Java <<-- C++ <<-- javaScript
	// ===========================================================

	/**
	 * 用来显示回调结果的activity（测试用）
	 */
//	public static JniShowActivity mainActivity = null;

	/**
	 * 回调初始化函数
	 */
	public native static void initCallBackParams();

	/**
	 * 通用回调函数(C/C++回调java中的某个方法)
	 * 
	 * @param clazz
	 *            回调需要相应的类名(一般是activity)
	 * @param code
	 *            状态码
	 * @param args
	 *            对象数组(实际上返回的数据)
	 * @return
	 */
//	public static int onNativeCallBack(String clazz, int code, Object... args) {
//		Message msg = new Message();
//		msg.obj = "统一回调函数收到的参数>> class:" + clazz + " code:" + code
//				+ " object[]:" + args.toString() + "\n";
//		for (int k = 0; k <= 2; k++) {
//			msg.obj = msg.obj + args[k].toString() + "\n";
//		}
//		msg.obj = msg.obj + args[3].toString() + "\n" + args[4].toString();
//		mainActivity.mHandler.sendMessage(msg);
//		return 1;
//	};

}
