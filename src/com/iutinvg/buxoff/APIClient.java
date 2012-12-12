package com.iutinvg.buxoff;

import android.content.Context;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;

public class APIClient {
	private static final String TAG = "APIClient";

	public static final String ROOT_URL = "http://159.253.3.57:8000/";

	private static final String BASE_URL = "api/v1/";

	private static AsyncHttpClient client = new AsyncHttpClient();
	public static String api_key = "";

	public static void cancel() {
		client.cancelRequests(null, true);
	}

	public static void get(String url, RequestParams params,
			AsyncHttpResponseHandler responseHandler) {
		if (params == null) {
			params = new RequestParams();
		}
		addAuthToParams(params);
		String absoluteUrl = getAbsoluteUrl(url);
		client.get(absoluteUrl, params, responseHandler);
		Log.i(TAG, "get: " + absoluteUrl + '?' + params);
	}

	public static void post(String url, RequestParams params,
			AsyncHttpResponseHandler responseHandler) {
		client.post(getAbsoluteUrlWithAuth(url), params, responseHandler);
	}

	public static void auth(String username, String password,
			AsyncHttpResponseHandler responseHandler) {
		client.setBasicAuth(username, password);
		client.get(getAbsoluteUrl("auth/login/"), responseHandler);
	}

	/**
	 * Unset basic HTTP auth params. Have to be called when api_token is got.
	 */
	public static void authClear() {
		client = new AsyncHttpClient();
	}

	private static String getAbsoluteUrl(String relativeUrl) {
		return ROOT_URL + BASE_URL + relativeUrl;
	}

	private static String getAbsoluteUrlWithAuth(String relativeUrl) {
		return getAbsoluteUrl(relativeUrl) + "?api_key=" + api_key;
	}

	private static void addAuthToParams(RequestParams params) {
		params.put("api_key", api_key);
	}

	public static void logout(Context c) {
		APIClient.token(c, "");
		APIClient.api_key = "";
	}

	public static Boolean isAuthenticated() {
		try {
			return !APIClient.api_key.isEmpty();
		} catch (NullPointerException e) {
			return false;
		} catch (Exception e) {
			Log.e(TAG, e.getLocalizedMessage());
		}

		return false;
	}

	/**
	 * To build new instance of client. For example after basic authentication,
	 * when we need "clear" client further.
	 */
	public static void initialize(Context c) {
		client = new AsyncHttpClient();
		APIClient.api_key = APIClient.token(c, null);
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
