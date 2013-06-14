package com.iutinvg.buxoff;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.View.OnFocusChangeListener;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.TextView;

import com.actionbarsherlock.app.SherlockFragmentActivity;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuItem;
import com.actionbarsherlock.view.Window;

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
	private EditText _email;
	private CheckBox _securityCheckBox;

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
		_email = (EditText) findViewById(R.id.edit_email);
		_securityCheckBox = (CheckBox) findViewById(R.id.checkbox_secure);
		
//		String a = null;
//		a.length();
	}

	@Override
	protected void onStart() {
		super.onStart();
		
		//String a = null;
		//a.length();

		_storage = new Storage(this);

		setupCounter();
		initDescHandlers();
		initTagsHandlers();
		initAcctHandlers();
		initEmailHandlers();
		initCheckSecure();
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
		case R.id.menu_show:
			actionShow(null);
			return true;
		case R.id.menu_save:
			saveTransaction(_buttonSave);
			return true;			
		case R.id.menu_push:
			saveTransaction(_buttonPush);
			return true;			
		default:
			return super.onOptionsItemSelected(item);
		}
	}
	
	public void actionShow(View view) {
		// TODO show saved transactions...
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

	protected void initEmailHandlers() {
		// set saved value
		_email.setText(_storage.email(null, _storage.secureMode(null)));

		_email.addTextChangedListener(new TextWatcher() {
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
				_storage.email(s.toString(), _storage.secureMode(null));
			}
			
			
		});
		
		_email.setOnFocusChangeListener(new OnFocusChangeListener() {
			
			@Override
			public void onFocusChange(View v, boolean hasFocus) {
				if (!hasFocus) {
					return;
				}
				// set position right before @
				String email = _email.getText().toString();
				int index = email.indexOf("@");
				if (index > 0) {
					_email.setSelection(index);
				}
			}
		});
	}
	
	protected void initCheckSecure() {
		_securityCheckBox.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				_storage.secureMode(isChecked);
				_storage.email(_email.getText().toString(), isChecked);
			}
		});
		_securityCheckBox.setChecked(_storage.secureMode(null));
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
		
		// check amount
		if (TextUtils.isEmpty(_email.getText().toString())) {
			this.validation_error += "\n";
			this.validation_error = this.getString(R.string.error_email_empty);
			showDialog();
			return;
		}


		
		// deal to start email client
		Intent emailIntent = new Intent(Intent.ACTION_SEND);
		emailIntent.putExtra(Intent.EXTRA_EMAIL, new String[] { _email.getText().toString() });
		emailIntent.putExtra(Intent.EXTRA_SUBJECT, "expense");
		emailIntent.putExtra(Intent.EXTRA_TEXT, transactions);
		emailIntent.setType("plain/text");
		startActivity(Intent.createChooser(emailIntent, getString(R.string.email_prompt)));
		
		handleAddTransaction();
	}
	
	private void handleAddTransaction() {
		// clear saved transactions
		SharedPreferences sp = getSharedPreferences(Storage.PREF_FILE,
				MODE_PRIVATE);
		SharedPreferences.Editor edit = sp.edit();
		edit.putString(Storage.SAVED_TRANSACTIONS, "");
		edit.commit();
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

//	private void setButtonsEnabled(boolean enabled) {
//		_buttonPush.setEnabled(enabled);
//		_buttonSave.setEnabled(enabled);
//	}

}
