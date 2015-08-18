package org.unify.http;

import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;

public class SimpleHttpClient {

	private static AsyncHttpClient _client = new AsyncHttpClient();
	
	public static void get(String url, AsyncHttpResponseHandler responseHandler) {
		_client.get(url, responseHandler);
	}
	
	
	public static void post(final String url, RequestParams params, AsyncHttpResponseHandler responseHandler) {
		_client.post(url, params, responseHandler);
	}
	
	
	public static void post(String url, String postData, AsyncHttpResponseHandler responseHandler) {
		RequestParams params = new RequestParams();
		
		try {
			JSONObject jsonObject = new JSONObject(postData);
			Iterator<?> it = jsonObject.keys();
			String key = "";
			String value = "";
			while (it.hasNext()) {
				key = it.next().toString();
				value = jsonObject.getString(key);
				params.add(key, value);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
		_client.post(url, params, responseHandler);
	}
}
