package com.whirix.buxferoffline;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends Activity {
	
	private final static String PREF_FILE = "com.whirix.buxoff.pref";
	private final static String SAVED_EMAIL = "com.whirix.buxoff.email";
	private final static String SAVED_LAST_TAGS = "com.whirix.buxoff.tags";
	private final static String SAVED_LAST_DESC = "com.whirix.buxoff.desc";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        initEmailHandlers();
        initTagsHandlers();
        initDescHandlers();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
    
    protected void initEmailHandlers() {
    	EditText email = (EditText)findViewById(R.id.edit_email);
    	
    	// set saved value
    	final SharedPreferences sp = getSharedPreferences(PREF_FILE, MODE_PRIVATE);
    	email.setText(sp.getString(SAVED_EMAIL, "crackNNNN@buxfer.com"));
    	
    	email.addTextChangedListener(new TextWatcher() {
			
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
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
    	EditText tags = (EditText)findViewById(R.id.edit_tags);
    	
    	// set saved value
    	final SharedPreferences sp = getSharedPreferences(PREF_FILE, MODE_PRIVATE);
    	tags.setText(sp.getString(SAVED_LAST_TAGS, ""));
    	
    	tags.addTextChangedListener(new TextWatcher() {
			
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
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
    	EditText desc = (EditText)findViewById(R.id.edit_desc);
    	
    	// set saved value
    	final SharedPreferences sp = getSharedPreferences(PREF_FILE, MODE_PRIVATE);
    	desc.setText(sp.getString(SAVED_LAST_DESC, "buxoff"));
    	
    	desc.addTextChangedListener(new TextWatcher() {
			
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
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
			}
		});
    }
    
    public void sendMessage(View view) {
    	EditText email = (EditText)findViewById(R.id.edit_email);
    	EditText amount = (EditText)findViewById(R.id.edit_amount);
    	EditText tags = (EditText)findViewById(R.id.edit_tags);
    	EditText desc = (EditText)findViewById(R.id.edit_desc);
    	
    	// comment
    	Intent emailIntent = new Intent(Intent.ACTION_SEND);
    	emailIntent.putExtra(Intent.EXTRA_EMAIL, new String[]{email.getText().toString()});
    	emailIntent.putExtra(Intent.EXTRA_SUBJECT, "expense");
    	emailIntent.putExtra(Intent.EXTRA_TEXT,
    			desc.getText().toString()+
    			" "+amount.getText().toString()+
    			" tags:"+tags.getText().toString()+
    			" acct:cash");
    	emailIntent.setType("plain/text");
    	startActivity(Intent.createChooser(emailIntent, getString(R.string.email_prompt)));
    }
}
