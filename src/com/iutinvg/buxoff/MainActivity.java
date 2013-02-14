package com.iutinvg.buxoff;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import org.json.JSONObject;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.actionbarsherlock.app.SherlockFragmentActivity;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuItem;
import com.actionbarsherlock.view.Window;
import com.loopj.android.http.JsonHttpResponseHandler;
import com.loopj.android.http.RequestParams;

public class MainActivity extends SherlockFragmentActivity {
	private final static String TAG = "MainActivity";

	// the error message to show in validation error dialog
	private String validation_error;

	private Button _buttonSave;
	private Button _buttonPush;
	private AutoCompleteTextView _tags;
	private AutoCompleteTextView _desc;
	private AutoCompleteTextView _acct;
	private EditText _amount;
	private TextView _counter;

	private Storage _storage;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
		setContentView(R.layout.activity_main);

		getSupportActionBar();
		setSupportProgressBarIndeterminateVisibility(false);

		_buttonSave = (Button) findViewById(R.id.button_save);
		_buttonPush = (Button) findViewById(R.id.button_push);
		_tags = (AutoCompleteTextView) findViewById(R.id.edit_tags);
		_desc = (AutoCompleteTextView) findViewById(R.id.edit_desc);
		_acct = (AutoCompleteTextView) findViewById(R.id.edit_acct);
		_amount = (EditText) findViewById(R.id.edit_amount);
		_counter = (TextView) findViewById(R.id.text_counter);

		APIClient.initialize(this);
	}

	@Override
	protected void onStart() {
		super.onStart();
		
		//String a = null;
		//a.length();

		_storage = new Storage(this);

		if (!APIClient.isAuthenticated()) {
			Intent i = new Intent(this, LoginActivity.class);
			startActivity(i);
			return;
		}

		setupCounter();
		initDescHandlers();
		initTagsHandlers();
		initAcctHandlers();
		setupAdapters();
		
		
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getSupportMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle item selection
		switch (item.getItemId()) {
		case android.R.id.home:
			finish();
			return true;
		case R.id.menu_logout:
			actionLogout(null);
			return true;
		default:
			return super.onOptionsItemSelected(item);
		}
	}
	
	public void actionLogout(View view) {
		APIClient.token(this, "");
		Intent i = new Intent(this, LoginActivity.class);
		startActivity(i);
	}

	protected void initTagsHandlers() {
		// set saved value
		final SharedPreferences sp = getSharedPreferences(Storage.PREF_FILE,
				MODE_PRIVATE);
		_tags.setText(sp.getString(Storage.SAVED_LAST_TAGS, ""));
		_tags.addTextChangedListener(new TextWatcher() {

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
			}

			@Override
			public void afterTextChanged(Editable s) {
				SharedPreferences.Editor edit = sp.edit();
				edit.putString(Storage.SAVED_LAST_TAGS, s.toString());
				edit.commit();
			}
		});
	}

	protected void initDescHandlers() {
		// set saved value
		final SharedPreferences sp = getSharedPreferences(Storage.PREF_FILE,
				MODE_PRIVATE);
		_desc.setText(sp.getString(Storage.SAVED_LAST_DESC, "buxoff"));

		_desc.addTextChangedListener(new TextWatcher() {

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
			}

			@Override
			public void afterTextChanged(Editable s) {
				SharedPreferences.Editor edit = sp.edit();
				edit.putString(Storage.SAVED_LAST_DESC, s.toString());
				edit.commit();

				// also we will try to find a rule for that
				String possible_tag = _storage.rule(s.toString(), null);
				if (possible_tag != null) {
					_tags.setText(possible_tag);
				}
			}
		});
	}

	protected void initAcctHandlers() {
		// set saved value
		final SharedPreferences sp = getSharedPreferences(Storage.PREF_FILE,
				MODE_PRIVATE);
		_acct.setText(sp.getString(Storage.SAVED_LAST_ACCT, ""));

		_acct.addTextChangedListener(new TextWatcher() {

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
			}

			@Override
			public void afterTextChanged(Editable s) {
				SharedPreferences.Editor edit = sp.edit();
				edit.putString(Storage.SAVED_LAST_ACCT, s.toString());
				edit.commit();
			}
		});
	}

	protected void showDialog() {
		DialogFragment newFragment = ErrorAlertDialogFragment
				.newInstance(this.validation_error);
		newFragment.show(getSupportFragmentManager(), "dialog");
	}

	protected void setupCounter() {
		final SharedPreferences sp = getSharedPreferences(Storage.PREF_FILE,
				MODE_PRIVATE);
		String transactions = sp.getString(Storage.SAVED_TRANSACTIONS, "");

		if (transactions == "") {
			_counter.setVisibility(View.INVISIBLE);
		} else {
			String[] lines = transactions.split("\r\n|\r|\n");
			_counter.setText(String.format("%d", lines.length));
			_counter.setVisibility(View.VISIBLE);
		}
	}

	protected void pushTransactions() {
		final SharedPreferences sp = getSharedPreferences(Storage.PREF_FILE,
				MODE_PRIVATE);

		String transactions = sp.getString(Storage.SAVED_TRANSACTIONS, "");
		if (TextUtils.isEmpty(transactions)) {
			Log.d(TAG, "nothing to push");
			return;
		}

		RequestParams params = new RequestParams();
		params.put("format", "sms");
		params.put("text", transactions);

		APIClient.post("add_transaction", params,
				new JsonHttpResponseHandler() {

					@Override
					public void onStart() {
						super.onStart();
						getSupportActionBar();
						setSupportProgressBarIndeterminateVisibility(true);
						setButtonsEnabled(false);
					}

					@Override
					public void onSuccess(int statusCode, JSONObject response) {
						handleAddTransaction(response);
						super.onSuccess(statusCode, response);
					}

					@Override
					public void onFinish() {
						setupCounter();
						setButtonsEnabled(true);
						getSupportActionBar();
						setSupportProgressBarIndeterminateVisibility(false);
						super.onFinish();
					}

					/*
					 * @Override public void onFailure(Throwable error, String
					 * content) { // TODO Auto-generated method stub
					 * super.onFailure(error, content); Log.e(TAG, "fail? "+
					 * error.getLocalizedMessage()); Log.e(TAG, "fail? "+
					 * content); }
					 */

					@Override
					public void onFailure(Throwable e, JSONObject errorResponse) {
						String message = APIClient.handleError(errorResponse);
						if (!TextUtils.isEmpty(message)) {
							validation_error = message;
							showDialog();
						}

						Log.e(TAG, "fail? " + e.getLocalizedMessage());
						super.onFailure(e, errorResponse);
					}

					/*
					 * @Override protected void handleFailureMessage(Throwable
					 * e, String responseBody) { // TODO Auto-generated method
					 * stub super.handleFailureMessage(e, responseBody);
					 * Log.e(TAG, "fail? "+ e.getLocalizedMessage()); Log.e(TAG,
					 * "fail? "+ responseBody); }
					 */

				});

	}

	private void handleAddTransaction(JSONObject response) {
		String errorMessage = null;
		Log.d(TAG, response.toString());
		try {
			JSONObject obj = APIClient.handleResponse(response);
			if (obj != null) {
				// clear saved transactions
				SharedPreferences sp = getSharedPreferences(Storage.PREF_FILE,
						MODE_PRIVATE);
				SharedPreferences.Editor edit = sp.edit();
				edit.putString(Storage.SAVED_TRANSACTIONS, "");
				edit.commit();
			} else {
				errorMessage = "can't parse server response";
			}
		} catch (Exception e) {
			Log.e(TAG, e.getLocalizedMessage());
			errorMessage = e.getLocalizedMessage();
		}

		if (errorMessage != null) {
			DialogFragment newFragment = ErrorAlertDialogFragment
					.newInstance(errorMessage);
			newFragment.show(getSupportFragmentManager(), "push_errors");
		}
	}

	public void saveTransaction(View view) {
		Boolean pushing = view.getId() == R.id.button_push;
		Boolean valid = validate();

		// validation
		if (!valid && !pushing) {
			showDialog();
			return;
		}

		// put new words, rules, etc
		this.updateRules();

		// update autocomplete adapters with new tags/descriptions
		this.setupAdapters();

		if (valid) {
			String transaction = getTransactionString();

			final SharedPreferences sp = getSharedPreferences(
					Storage.PREF_FILE, MODE_PRIVATE);
			String existing = sp.getString(Storage.SAVED_TRANSACTIONS, "");

			existing = existing + transaction + "\n";

			SharedPreferences.Editor edit = sp.edit();
			edit.putString(Storage.SAVED_TRANSACTIONS, existing);
			edit.commit();
		}
		setupCounter();

		_amount.setText("");
		_amount.requestFocus();

		if (pushing) {
			pushTransactions();
		}
	}

	/**
	 * Returns transaction string from current form data.
	 * 
	 * @return a string to insert in email message
	 */
	protected String getTransactionString() {
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:dd:ss",
				Locale.getDefault());
		String now = format.format(new Date());

		String result = _desc.getText().toString().trim() + " "
				+ _amount.getText().toString().trim() + " tags:"
				+ _tags.getText().toString().trim() + " acct:"
				+ _acct.getText().toString().trim() + " date:" + now;

		return result;
	}

	protected void setupAdapters() {
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, _storage.autocompletes(
						null, Storage.PREF_DESC));
		_desc.setAdapter(adapter);

		adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, _storage.autocompletes(
						null, Storage.PREF_TAGS));
		_tags.setAdapter(adapter);

		adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, _storage.autocompletes(
						null, Storage.PREF_ACCT));
		_acct.setAdapter(adapter);
	}

	protected void updateRules() {
		// save history before sending
		_storage.autocompletes(_desc.getText().toString(), Storage.PREF_DESC);
		_storage.autocompletes(_tags.getText().toString(), Storage.PREF_TAGS);
		_storage.autocompletes(_acct.getText().toString(), Storage.PREF_ACCT);
		_storage.rule(_desc.getText().toString(), _tags.getText().toString());
	}

	protected Boolean validate() {
		String tmp;
		Boolean result = true;

		this.validation_error = "";

		// check amount
		tmp = _amount.getText().toString();
		if (TextUtils.isEmpty(tmp)) {
			this.validation_error += "\n";
			this.validation_error = this.getString(R.string.error_amount_empty);
			result = false;
		}

		// check description
		tmp = _desc.getText().toString();
		if (TextUtils.isEmpty(tmp)) {
			this.validation_error += "\n";
			this.validation_error += this.getString(R.string.error_desc_empty);
			result = false;
		}

		// check tags
		tmp = _tags.getText().toString();
		if (TextUtils.isEmpty(tmp)) {
			this.validation_error += "\n";
			this.validation_error += this.getString(R.string.error_tags_empty);
			result = false;
		}

		// check account
		tmp = _acct.getText().toString();
		if (tmp == null || tmp.length() == 0) {
			this.validation_error += "\n";
			this.validation_error += this
					.getString(R.string.error_account_empty);
			result = false;
		}

		return result;
	}

	private void setButtonsEnabled(boolean enabled) {
		_buttonPush.setEnabled(enabled);
		_buttonSave.setEnabled(enabled);
	}

}
