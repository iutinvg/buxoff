package com.iutinvg.buxoff;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.Activity;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.TextView;

import com.loopj.android.http.JsonHttpResponseHandler;
import com.loopj.android.http.RequestParams;

/**
 * Activity which displays a login screen to the user, offering registration as
 * well.
 */
public class LoginActivity extends Activity {
	private static final String TAG = "LoginActivity";

	// Values for email and password at the time of the login attempt.
	private String mEmail;
	private String mPassword;

	// UI references.
	private EditText mEmailView;
	private EditText mPasswordView;
	private View mLoginFormView;
	private View mLoginStatusView;
	private TextView mLoginStatusMessageView;
	
	private HandleTagTask _tagsTask = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_login);
		
		// Set up the login form.
		mEmail = APIClient.username(this, null);
		mEmailView = (EditText) findViewById(R.id.email);
		mEmailView.setText(mEmail);

		mPasswordView = (EditText) findViewById(R.id.password);
		mPasswordView
				.setOnEditorActionListener(new TextView.OnEditorActionListener() {
					@Override
					public boolean onEditorAction(TextView textView, int id,
							KeyEvent keyEvent) {
						if (id == R.id.login || id == EditorInfo.IME_NULL) {
							attemptLogin();
							return true;
						}
						return false;
					}
				});

		mLoginFormView = findViewById(R.id.login_form);
		mLoginStatusView = findViewById(R.id.login_status);
		mLoginStatusMessageView = (TextView) findViewById(R.id.login_status_message);

		findViewById(R.id.sign_in_button).setOnClickListener(
				new View.OnClickListener() {
					@Override
					public void onClick(View view) {
						attemptLogin();
					}
				});
	}

	/**
	 * Attempts to sign in or register the account specified by the login form.
	 * If there are form errors (invalid email, missing fields, etc.), the
	 * errors are presented and no actual login attempt is made.
	 */
	public void attemptLogin() {
		// TODO avoid double request
		
		// Reset errors.
		mEmailView.setError(null);
		mPasswordView.setError(null);

		// Store values at the time of the login attempt.
		mEmail = mEmailView.getText().toString();
		mPassword = mPasswordView.getText().toString();

		boolean cancel = false;
		View focusView = null;

		// Check for a valid password.
		if (TextUtils.isEmpty(mPassword)) {
			mPasswordView.setError(getString(R.string.error_field_required));
			focusView = mPasswordView;
			cancel = true;
		} else if (mPassword.length() < 4) {
			mPasswordView.setError(getString(R.string.error_invalid_password));
			focusView = mPasswordView;
			cancel = true;
		}

		// Check for a valid email address.
		if (TextUtils.isEmpty(mEmail)) {
			mEmailView.setError(getString(R.string.error_field_required));
			focusView = mEmailView;
			cancel = true;
		} else if (!mEmail.contains("@")) {
			mEmailView.setError(getString(R.string.error_invalid_email));
			focusView = mEmailView;
			cancel = true;
		}

		if (cancel) {
			// There was an error; don't attempt login and focus the first
			// form field with an error.
			focusView.requestFocus();
		} else {
			// Show a progress spinner, and kick off a background task to
			// perform the user login attempt.
			mLoginStatusMessageView.setText(R.string.login_progress_signing_in);
			
			APIClient.username(this, mEmail);
			showProgress(true);
			//requestToken(mEmail, mPassword);
			requestToken("iutin@whirix.com", "Slava123");
		}
	}

	/**
	 * Shows the progress UI and hides the login form.
	 */
	@TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
	private void showProgress(final boolean show) {
		// On Honeycomb MR2 we have the ViewPropertyAnimator APIs, which allow
		// for very easy animations. If available, use these APIs to fade-in
		// the progress spinner.
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
			int shortAnimTime = getResources().getInteger(
					android.R.integer.config_shortAnimTime);

			mLoginStatusView.setVisibility(View.VISIBLE);
			mLoginStatusView.animate().setDuration(shortAnimTime)
					.alpha(show ? 1 : 0)
					.setListener(new AnimatorListenerAdapter() {
						@Override
						public void onAnimationEnd(Animator animation) {
							mLoginStatusView.setVisibility(show ? View.VISIBLE
									: View.GONE);
						}
					});

			mLoginFormView.setVisibility(View.VISIBLE);
			mLoginFormView.animate().setDuration(shortAnimTime)
					.alpha(show ? 0 : 1)
					.setListener(new AnimatorListenerAdapter() {
						@Override
						public void onAnimationEnd(Animator animation) {
							mLoginFormView.setVisibility(show ? View.GONE
									: View.VISIBLE);
						}
					});
		} else {
			// The ViewPropertyAnimator APIs are not available, so simply show
			// and hide the relevant UI components.
			mLoginStatusView.setVisibility(show ? View.VISIBLE : View.GONE);
			mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
		}
	}

	private void requestToken(String email, String password) {
		showProgress(true);
		
		RequestParams params = new RequestParams();
		params.put("userid", email);
		params.put("password", password);
		APIClient.get("login", params, new JsonHttpResponseHandler() {

			@Override
			public void onSuccess(int statusCode, JSONObject response) {
				Log.d(TAG, "login: " + response.toString());
				handleResponse(response);
				requestTags();
				super.onSuccess(statusCode, response);
			}

			@Override
			public void onFailure(Throwable e, JSONObject errorResponse) {
				Log.d(TAG, e.getLocalizedMessage());
				super.onFailure(e, errorResponse);
			}

			@Override
			public void onFailure(Throwable e, JSONArray errorResponse) {
				Log.d(TAG, e.getLocalizedMessage());
				super.onFailure(e, errorResponse);
			}

			@Override
			protected void handleFailureMessage(Throwable e, String responseBody) {
				Log.d(TAG, e.getLocalizedMessage());
				super.handleFailureMessage(e, responseBody);
			}

			@Override
			public void onFailure(Throwable error, String content) {
				Log.d(TAG, error.getLocalizedMessage());
				Log.d(TAG, content);
				super.onFailure(error, content);
			}

			/*@Override
			public void onFinish() {
				showProgress(false);
				finish();
				super.onFinish();
			}*/

		});
	}
	
	private void requestTags() {
		Log.d(TAG, "start getting tags");
		mLoginStatusMessageView.setText(R.string.login_progress_syncing_tags);
		APIClient.get("tags", null, new JsonHttpResponseHandler() {

			@Override
			public void onSuccess(int statusCode, JSONObject response) {
				Log.d(TAG, "tags: " + response.toString());
				handleTags(response);
				super.onSuccess(statusCode, response);
			}

			@Override
			public void onFailure(Throwable e, JSONObject errorResponse) {
				Log.d(TAG, e.getLocalizedMessage());
				super.onFailure(e, errorResponse);
			}

			@Override
			public void onFailure(Throwable e, JSONArray errorResponse) {
				Log.d(TAG, e.getLocalizedMessage());
				super.onFailure(e, errorResponse);
			}

			@Override
			protected void handleFailureMessage(Throwable e, String responseBody) {
				Log.d(TAG, e.getLocalizedMessage());
				super.handleFailureMessage(e, responseBody);
			}

			@Override
			public void onFailure(Throwable error, String content) {
				Log.d(TAG, error.getLocalizedMessage());
				Log.d(TAG, content);
				super.onFailure(error, content);
			}

			@Override
			public void onFinish() {
				super.onFinish();
			}

		});
	}
	
	private void handleResponse(JSONObject response) {
		try {
			JSONObject obj = APIClient.handleResponse(response);
			if (obj!=null) {
				String token = obj.getString("token");
				APIClient.token = token;
				APIClient.token(this, token);
			}
		} catch (JSONException e) {
			Log.e(TAG, e.getLocalizedMessage());
			// TODO show error to user??? "Unrecognized server response."
		}
	}

	private void handleTags(JSONObject response) {
		_tagsTask = new HandleTagTask();
		_tagsTask.execute(response);
	}
	
	private class HandleTagTask extends AsyncTask<JSONObject, Void, Boolean> {

		@Override
		protected Boolean doInBackground(JSONObject... params) {
			Log.d(TAG, "async task started");
			try {
				JSONObject obj = APIClient.handleResponse(params[0]);
				if (obj!=null) {
					Storage storage = new Storage(getBaseContext());
					JSONArray tags = obj.getJSONArray("tags");
					//Log.d(TAG, tags.toString());
					for (int i = 0; i < tags.length(); i++) {
						JSONObject tag = tags.getJSONObject(i);
						Log.d(TAG, tag.toString());
						this.handleSingleTag(storage, tag);
					}
				}
			} catch (JSONException e) {
				Log.e(TAG, e.getLocalizedMessage());
			}
			
			return true;
		}
		
		@Override
		protected void onPostExecute(final Boolean success) {
			Log.d(TAG, "async task finished");
			_tagsTask = null;
			showProgress(false);

			if (success) {
				finish();
			}
		}
		
		
		private void handleSingleTag(Storage storage, JSONObject tag) throws JSONException {
			tag = tag.getJSONObject("key-tag");
			
			String desc = null;
			String name = tag.getString("name");
			storage.autocompletes(name, Storage.PREF_TAGS);
			
			if (tag.has("keywords")) {
				desc = tag.getString("keywords");
				storage.autocompletes(desc, Storage.PREF_DESC);
				storage.rule(desc, name);
			}
		}
		
	}

}
