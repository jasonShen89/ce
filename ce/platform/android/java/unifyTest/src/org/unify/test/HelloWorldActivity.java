package org.unify.test;

import org.unify.helper.CELibHelper;
import org.unify.helper.JsHelper;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class HelloWorldActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        TextView text = new TextView(this);
        text.setText("Hello World, Unify!");
        setContentView(text);
        //setContentView(R.layout.main);
        
        
        // 初始化CELibrary
        CELibHelper.init(this);
        
        // 执行main.js文件
        if (JsHelper.executeJsString("require('main');") != 0) {
        	Log.e("unify", "load main.js failed!");
        	return;
        }
        
        // 执行全局方法
        Object result = JsHelper.callGlobalFunction("globalFunction", "hello");
        Log.d("unify", (result != null) ? (String)result : "null");

        // 监听通知; 调用proxy
        JsHelper.addNotificationObserver("LOGIN_VM_CHANGED", "org.unify.package");
        JsHelper.callProxy("LoginProxy", "login", "abc", "123456");
    }
    
    // 加载appJs.so
    static {
    	System.loadLibrary("unifylib");
    }
}