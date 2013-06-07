package com.iutinvg.buxoff;

import java.util.Map;
import java.util.Set;

import android.content.Context;
import android.content.SharedPreferences;

public class Storage {
	private Context _context;
	
	public final static String PREF_FILE = "com.whirix.buxoff.pref";
	public final static String SAVED_LAST_TAGS = "com.whirix.buxoff.tags";
	public final static String SAVED_LAST_DESC = "com.whirix.buxoff.desc";
	public final static String SAVED_LAST_ACCT = "com.whirix.buxoff.acct";
	public final static String SAVED_LAST_EMAIL = "com.whirix.buxoff.email";
	public final static String SAVED_TRANSACTIONS = "com.whirix.buxoff.transactions";
	public final static String SAVED_SECURE_MODE = "com.iutinvg.buxoff.secure";

	// to store the tags
	public final static String PREF_DESC = "com.whirix.buxoff.all_desc";
	public final static String PREF_TAGS = "com.whirix.buxoff.all_tags";
	public final static String PREF_ACCT = "com.whirix.buxoff.all_accounts";
	public final static String PREF_RULES = "com.whirix.buxoff.rules";
	
	public Storage(Context context) {
		_context = context;
	}
	
	public Boolean secureMode(Boolean secure) {
		final SharedPreferences sp = _context.getSharedPreferences(Storage.PREF_FILE,
				Context.MODE_PRIVATE);
		if (secure==null) {
			return sp.getBoolean(SAVED_SECURE_MODE, true);
		} else {
			SharedPreferences.Editor edit = sp.edit();
			edit.putBoolean(SAVED_SECURE_MODE, secure);
			edit.commit();
		}
		
		return secure;
	}
	
	public String email(String newEmail, Boolean dropDigits) {
		final SharedPreferences sp = _context.getSharedPreferences(Storage.PREF_FILE,
				Context.MODE_PRIVATE);
		if (newEmail==null) {
			String email = sp.getString(Storage.SAVED_LAST_EMAIL, "@m.buxfer.com");
			if (dropDigits) {
				return emailWithoutDigits(email);
			}
			return email;
		} else {
			if (dropDigits) {
				newEmail = emailWithoutDigits(newEmail);
			}
			SharedPreferences.Editor edit = sp.edit();
			edit.putString(Storage.SAVED_LAST_EMAIL, newEmail);
			edit.commit();
		}
		return newEmail;
	}
	
	private String emailWithoutDigits(String email) {
		return email.replaceAll("\\d", "");
	}

	public String rule(String desc, String tag) {
		final SharedPreferences sp = _context.getSharedPreferences(Storage.PREF_RULES,
				Context.MODE_PRIVATE);
	
		if (tag == null) {
			tag = sp.getString(desc, null);
		} else {
			SharedPreferences.Editor edit = sp.edit();
			edit.putString(desc.trim(), tag.trim());
			edit.commit();
		}
	
		return tag;
	}

	public String[] autocompletes(String new_item, String pref_id) {
		final SharedPreferences sp = _context.getSharedPreferences(pref_id, Context.MODE_PRIVATE);
	
		if (new_item != null) {
			SharedPreferences.Editor edit = sp.edit();
			edit.putString(new_item.trim(), "");
			edit.commit();
		}
	
		Map<String, ?> all = sp.getAll();
		Set<String> keys = all.keySet();
	
		return keys.toArray(new String[keys.size()]);
	}

}
