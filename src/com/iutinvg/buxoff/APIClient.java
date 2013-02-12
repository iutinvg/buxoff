package com.iutinvg.buxoff;

import org.json.JSONObject;

import android.content.Context;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.text.TextUtils;
import android.util.Log;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;

public class APIClient {
	private static final String TAG = "APIClient";

	public static final String ROOT_URL = "https://www.buxfer.com/api/";

	private static AsyncHttpClient client = new AsyncHttpClient();
	public static String token = "";

	public static void cancel() {
		client.cancelRequests(null, true);
	}

	public static void get(String command, RequestParams params,
			AsyncHttpResponseHandler responseHandler) {
		if (params == null) {
			params = new RequestParams();
		}
		if (APIClient.isAuthenticated()) {
			params.put("token", token);
		}
		String absoluteUrl = getAbsoluteUrl(command);
		client.get(absoluteUrl, params, responseHandler);
		Log.i(TAG, "get: " + absoluteUrl + '?' + params);
	}

	public static void post(String command, RequestParams params,
			AsyncHttpResponseHandler responseHandler) {
		client.post(getAbsoluteUrlWithToken(command), params, responseHandler);
	}

	public static JSONObject handleResponse(JSONObject response) {
		try {
			JSONObject obj = response.getJSONObject("response");
			String status = obj.getString("status");
			Log.d(TAG, "status: " + status);
			if (status.equalsIgnoreCase("OK")) {
				return obj; 
			}
		} catch (Exception e) {
			Log.e(TAG, e.getLocalizedMessage());
		}
		return null;
	}

	public static String handleError(JSONObject response) {
		try {
			JSONObject obj = response.getJSONObject("error");
			String message = obj.getString("message");
			Log.e(TAG, "error: " + message);
			return message; 
		} catch (Exception e) {
			Log.e(TAG, e.getLocalizedMessage());
		}
		return null;
	}

	private static String getAbsoluteUrl(String command) {
		return ROOT_URL + command + ".json";
	}

	private static String getAbsoluteUrlWithToken(String command) {
		return getAbsoluteUrl(command) + "?token=" + token;
	}
	
	public static void logout(Context c) {
		APIClient.token(c, "");
		APIClient.token = "";
	}

	public static Boolean isAuthenticated() {
		Log.d(TAG, "token is " + token);
		return !TextUtils.isEmpty(token);
	}

	/**
	 * To build new instance of client. For example after basic authentication,
	 * when we need "clear" client further.
	 */
	public static void initialize(Context c) {
		APIClient.token = APIClient.token(c, null);
		client = new AsyncHttpClient();
	}

	public static boolean isNetworkOnline(Context c) {
		boolean status = false;
		try {
			ConnectivityManager cm = (ConnectivityManager) c
					.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo netInfo = cm.getNetworkInfo(0);
			if (netInfo != null
					&& netInfo.getState() == NetworkInfo.State.CONNECTED) {
				status = true;
			} else {
				netInfo = cm.getNetworkInfo(1);
				if (netInfo != null
						&& netInfo.getState() == NetworkInfo.State.CONNECTED)
					status = true;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return status;

	}

	/**
	 * Get API token from any place in the app. Also to check if the user is
	 * authenticated.
	 * 
	 * @param c
	 * @param token
	 * @return auth token
	 */
	public static String token(Context c, String token) {
		final SharedPreferences sp = c.getSharedPreferences(
				c.getString(R.string._preference_file), Context.MODE_PRIVATE);

		if (null != token) {
			if (token == "") {
				token = null;
			}
			SharedPreferences.Editor edit = sp.edit();
			edit.putString(c.getString(R.string._auth_token), token);
			edit.commit();
			return token;
		}

		return sp.getString(c.getString(R.string._auth_token), null);
	}

	/**
	 * Get username from any place in the app. Also to check if the user is
	 * authenticated.
	 * 
	 * @param c
	 * @param username
	 * @return username
	 */
	public static String username(Context c, String username) {
		final SharedPreferences sp = c.getSharedPreferences(
				c.getString(R.string._preference_file), Context.MODE_PRIVATE);

		if (null != username) {
			SharedPreferences.Editor edit = sp.edit();
			edit.putString(c.getString(R.string._auth_username), username);
			edit.commit();
			return username;
		}

		return sp.getString(c.getString(R.string._auth_username), null);
	}

}
