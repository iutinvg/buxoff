package com.whirix.buxferoffline;

import java.util.Map;
import java.util.Set;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Menu;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.EditText;

public class MainActivity extends Activity {

	private final static String PREF_FILE = "com.whirix.buxoff.pref";
	private final static String SAVED_EMAIL = "com.whirix.buxoff.email";
	private final static String SAVED_LAST_TAGS = "com.whirix.buxoff.tags";
	private final static String SAVED_LAST_DESC = "com.whirix.buxoff.desc";
	private final static String SAVED_LAST_ACCT = "com.whirix.buxoff.acct";

	// to store the tags
	private final static String PREF_DESC = "com.whirix.buxoff.all_desc";
	private final static String PREF_TAGS = "com.whirix.buxoff.all_tags";
	private final static String PREF_ACCT = "com.whirix.buxoff.all_accounts";
	private final static String PREF_RULES = "com.whirix.buxoff.rules";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		initEmailHandlers();
		initDescHandlers();
		initTagsHandlers();
		initAcctHandlers();
		setupAdapters();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	protected void initEmailHandlers() {
		EditText email = (EditText) findViewById(R.id.edit_email);

		// set saved value
		final SharedPreferences sp = getSharedPreferences(PREF_FILE,
				MODE_PRIVATE);
		email.setText(sp.getString(SAVED_EMAIL, "crackNNNN@buxfer.com"));

		email.addTextChangedListener(new TextWatcher() {

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
				edit.putString(SAVED_EMAIL, s.toString());
				edit.commit();
			}
		});
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

	public void sendMessage(View view) {
		EditText email = (EditText) findViewById(R.id.edit_email);
		EditText amount = (EditText) findViewById(R.id.edit_amount);
		AutoCompleteTextView tags = (AutoCompleteTextView) findViewById(R.id.edit_tags);
		AutoCompleteTextView desc = (AutoCompleteTextView) findViewById(R.id.edit_desc);
		AutoCompleteTextView acct = (AutoCompleteTextView) findViewById(R.id.edit_acct);

		// save history before sending
		autocompletes(desc.getText().toString(), PREF_DESC);
		autocompletes(tags.getText().toString(), PREF_TAGS);
		autocompletes(acct.getText().toString(), PREF_ACCT);
		rule(desc.getText().toString(), tags.getText().toString());

		// update autocomplete adapters with new tags/descriptions
		this.setupAdapters();

		// comment
		Intent emailIntent = new Intent(Intent.ACTION_SEND);
		emailIntent.putExtra(Intent.EXTRA_EMAIL, new String[] { email.getText()
				.toString() });
		emailIntent.putExtra(Intent.EXTRA_SUBJECT, "expense");
		emailIntent.putExtra(Intent.EXTRA_TEXT, desc.getText().toString() + " "
				+ amount.getText().toString() + " tags:"
				+ tags.getText().toString() + " acct:" + acct.getText().toString());
		emailIntent.setType("plain/text");
		startActivity(Intent.createChooser(emailIntent,
				getString(R.string.email_prompt)));
	}

	protected void setupAdapters() {
		AutoCompleteTextView tags = (AutoCompleteTextView) findViewById(R.id.edit_tags);
		AutoCompleteTextView desc = (AutoCompleteTextView) findViewById(R.id.edit_desc);
		AutoCompleteTextView acct = (AutoCompleteTextView) findViewById(R.id.edit_acct);

		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, autocompletes(null, PREF_DESC));
		desc.setAdapter(adapter);

		adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, autocompletes(null, PREF_TAGS));
		tags.setAdapter(adapter);

		adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, autocompletes(null, PREF_ACCT));
		acct.setAdapter(adapter);
	}

	protected String[] autocompletes(String new_item, String pref_id) {
		final SharedPreferences sp = getSharedPreferences(pref_id, MODE_PRIVATE);

		if (new_item != null) {
			SharedPreferences.Editor edit = sp.edit();
			edit.putString(new_item, "");
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
			edit.putString(desc, tag);
			edit.commit();
		}

		return tag;
	}
}
