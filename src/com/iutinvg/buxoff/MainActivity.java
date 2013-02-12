package com.iutinvg.buxoff;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

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
import com.actionbarsherlock.view.Window;
import com.loopj.android.http.JsonHttpResponseHandler;
import com.loopj.android.http.RequestParams;

public class MainActivity extends SherlockFragmentActivity {
	private final static String TAG = "MainActivity";

	private final static String PREF_FILE = "com.whirix.buxoff.pref";
	private final static String SAVED_LAST_TAGS = "com.whirix.buxoff.tags";
	private final static String SAVED_LAST_DESC = "com.whirix.buxoff.desc";
	private final static String SAVED_LAST_ACCT = "com.whirix.buxoff.acct";
	private final static String SAVED_TRANSACTIONS = "com.whirix.buxoff.transactions";

	// to store the tags
	private final static String PREF_DESC = "com.whirix.buxoff.all_desc";
	private final static String PREF_TAGS = "com.whirix.buxoff.all_tags";
	private final static String PREF_ACCT = "com.whirix.buxoff.all_accounts";
	private final static String PREF_RULES = "com.whirix.buxoff.rules";

	// the error message to show in validation error dialog
	private String validation_error;
	
	private Button _buttonSave;
	private Button _buttonPush;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
		setContentView(R.layout.activity_main);
		
		getSupportActionBar();
		setSupportProgressBarIndeterminateVisibility(false);
		
		_buttonSave = (Button)findViewById(R.id.button_save);
		_buttonPush = (Button)findViewById(R.id.button_push);
		APIClient.token(this, ""); // logout
		
		APIClient.initialize(this);
	}

	@Override
	protected void onStart() {
		super.onStart();

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

	protected void initTagsHandlers() {
		AutoCompleteTextView tags = (AutoCompleteTextView) findViewById(R.id.edit_tags);

		// set saved value
		final SharedPreferences sp = getSharedPreferences(PREF_FILE,
				MODE_PRIVATE);
		tags.setText(sp.getString(SAVED_LAST_TAGS, ""));

		tags.addTextChangedListener(new TextWatcher() {

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
				edit.putString(SAVED_LAST_TAGS, s.toString());
				edit.commit();
			}
		});
	}

	protected void initDescHandlers() {
		AutoCompleteTextView desc = (AutoCompleteTextView) findViewById(R.id.edit_desc);

		// set saved value
		final SharedPreferences sp = getSharedPreferences(PREF_FILE,
				MODE_PRIVATE);
		desc.setText(sp.getString(SAVED_LAST_DESC, "buxoff"));

		desc.addTextChangedListener(new TextWatcher() {

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
				edit.putString(SAVED_LAST_DESC, s.toString());
				edit.commit();

				// also we will try to find a rule for that
				String possible_tag = rule(s.toString(), null);
				if (possible_tag != null) {
					EditText tags = (EditText) findViewById(R.id.edit_tags);
					tags.setText(possible_tag);
				}
			}
		});
	}

	protected void initAcctHandlers() {
		AutoCompleteTextView acct = (AutoCompleteTextView) findViewById(R.id.edit_acct);

		// set saved value
		final SharedPreferences sp = getSharedPreferences(PREF_FILE,
				MODE_PRIVATE);
		acct.setText(sp.getString(SAVED_LAST_ACCT, ""));

		acct.addTextChangedListener(new TextWatcher() {

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
				edit.putString(SAVED_LAST_ACCT, s.toString());
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
		final SharedPreferences sp = getSharedPreferences(PREF_FILE,
				MODE_PRIVATE);
		String transactions = sp.getString(SAVED_TRANSACTIONS, "");
		TextView counter = (TextView) findViewById(R.id.text_counter);

		if (transactions == "") {
			counter.setVisibility(View.INVISIBLE);
		} else {
			String[] lines = transactions.split("\r\n|\r|\n");
			counter.setText(String.format("%d", lines.length));
			counter.setVisibility(View.VISIBLE);
		}
	}

	protected void pushTransactions() {
		final SharedPreferences sp = getSharedPreferences(PREF_FILE,
				MODE_PRIVATE);

		String transactions = sp.getString(SAVED_TRANSACTIONS, "");
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

					/*@Override
					public void onFailure(Throwable error, String content) {
						// TODO Auto-generated method stub
						super.onFailure(error, content);
						Log.e(TAG, "fail? "+ error.getLocalizedMessage());
						Log.e(TAG, "fail? "+ content);
					}*/
					
					@Override
					public void onFailure(Throwable e, JSONObject errorResponse) {
						String message = APIClient.handleError(errorResponse);
						if (!TextUtils.isEmpty(message)) {
							validation_error = message;
							showDialog();
						}
						
						Log.e(TAG, "fail? "+ e.getLocalizedMessage());
						super.onFailure(e, errorResponse);
					}

					/*@Override
					protected void handleFailureMessage(Throwable e,
							String responseBody) {
						// TODO Auto-generated method stub
						super.handleFailureMessage(e, responseBody);
						Log.e(TAG, "fail? "+ e.getLocalizedMessage());
						Log.e(TAG, "fail? "+ responseBody);
					}*/

				});

	}

	private void handleAddTransaction(JSONObject response) {
		String errorMessage = null;
		Log.d(TAG, response.toString());
		try {
			JSONObject obj = APIClient.handleResponse(response);
			if (obj != null) {
				// clear saved transactions
				SharedPreferences sp = getSharedPreferences(PREF_FILE,
						MODE_PRIVATE);
				SharedPreferences.Editor edit = sp.edit();
				edit.putString(SAVED_TRANSACTIONS, "");
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

			final SharedPreferences sp = getSharedPreferences(PREF_FILE,
					MODE_PRIVATE);
			String existing = sp.getString(SAVED_TRANSACTIONS, "");

			existing = existing + transaction + "\n";

			SharedPreferences.Editor edit = sp.edit();
			edit.putString(SAVED_TRANSACTIONS, existing);
			edit.commit();
		}
		setupCounter();

		EditText amount = (EditText) findViewById(R.id.edit_amount);
		amount.setText("");
		amount.requestFocus();

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
		EditText amount = (EditText) findViewById(R.id.edit_amount);
		AutoCompleteTextView tags = (AutoCompleteTextView) findViewById(R.id.edit_tags);
		AutoCompleteTextView desc = (AutoCompleteTextView) findViewById(R.id.edit_desc);
		AutoCompleteTextView acct = (AutoCompleteTextView) findViewById(R.id.edit_acct);

		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:dd:ss",
				Locale.getDefault());
		String now = format.format(new Date());

		String result = desc.getText().toString().trim() + " "
				+ amount.getText().toString().trim() + " tags:"
				+ tags.getText().toString().trim() + " acct:"
				+ acct.getText().toString().trim() + " date:" + now;

		return result;
	}

	protected void setupAdapters() {
		AutoCompleteTextView tags = (AutoCompleteTextView) findViewById(R.id.edit_tags);
		AutoCompleteTextView desc = (AutoCompleteTextView) findViewById(R.id.edit_desc);
		AutoCompleteTextView acct = (AutoCompleteTextView) findViewById(R.id.edit_acct);

		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, autocompletes(null,
						PREF_DESC));
		desc.setAdapter(adapter);

		adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, autocompletes(null,
						PREF_TAGS));
		tags.setAdapter(adapter);

		adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, autocompletes(null,
						PREF_ACCT));
		acct.setAdapter(adapter);
	}

	protected void updateRules() {
		AutoCompleteTextView tags = (AutoCompleteTextView) findViewById(R.id.edit_tags);
		AutoCompleteTextView desc = (AutoCompleteTextView) findViewById(R.id.edit_desc);
		AutoCompleteTextView acct = (AutoCompleteTextView) findViewById(R.id.edit_acct);

		// save history before sending
		autocompletes(desc.getText().toString(), PREF_DESC);
		autocompletes(tags.getText().toString(), PREF_TAGS);
		autocompletes(acct.getText().toString(), PREF_ACCT);
		rule(desc.getText().toString(), tags.getText().toString());
	}

	protected String[] autocompletes(String new_item, String pref_id) {
		final SharedPreferences sp = getSharedPreferences(pref_id, MODE_PRIVATE);

		if (new_item != null) {
			SharedPreferences.Editor edit = sp.edit();
			edit.putString(new_item.trim(), "");
			edit.commit();
		}

		Map<String, ?> all = sp.getAll();
		Set<String> keys = all.keySet();

		return keys.toArray(new String[keys.size()]);
	}

	// get/set tag for description
	protected String rule(String desc, String tag) {
		final SharedPreferences sp = getSharedPreferences(PREF_RULES,
				MODE_PRIVATE);

		if (tag == null) {
			tag = sp.getString(desc, null);
		} else {
			SharedPreferences.Editor edit = sp.edit();
			edit.putString(desc.trim(), tag.trim());
			edit.commit();
		}

		return tag;
	}

	protected Boolean validate() {
		String tmp;
		EditText edit;

		Boolean result = true;

		this.validation_error = "";

		// check amount
		edit = (EditText) findViewById(R.id.edit_amount);
		tmp = edit.getText().toString();
		if (TextUtils.isEmpty(tmp)) {
			this.validation_error += "\n";
			this.validation_error = this.getString(R.string.error_amount_empty);
			result = false;
		}

		// check description
		edit = (EditText) findViewById(R.id.edit_desc);
		tmp = edit.getText().toString();
		if (TextUtils.isEmpty(tmp)) {
			this.validation_error += "\n";
			this.validation_error += this.getString(R.string.error_desc_empty);
			result = false;
		}

		// check tags
		edit = (EditText) findViewById(R.id.edit_tags);
		tmp = edit.getText().toString();
		if (TextUtils.isEmpty(tmp)) {
			this.validation_error += "\n";
			this.validation_error += this.getString(R.string.error_tags_empty);
			result = false;
		}

		// check account
		edit = (EditText) findViewById(R.id.edit_acct);
		tmp = edit.getText().toString();
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
