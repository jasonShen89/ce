package org.unify.http;

import org.apache.http.Header;

import com.loopj.android.http.AsyncHttpResponseHandler;

public class CEHttpResponseHandler extends AsyncHttpResponseHandler {
	public String context;
	
	private native void onJniSuccess(final String context, final int statusCode, final String responseString);
	private native void onJniFailure(final String context, final int statusCode, final String errorMessage);
	
	
	@Override
	public void onSuccess(int statusCode, Header[] headers, byte[] responseData) {
		String responseString = new String(responseData);
		this.onJniSuccess(this.context, statusCode, responseString);
	}
	
	
	@Override
	public void onFailure(int statusCode, Header[] headers, byte[] errorResponse, Throwable e) {
		this.onJniFailure(this.context, statusCode, e.toString());
	}
}
