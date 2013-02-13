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
	public final static String SAVED_TRANSACTIONS = "com.whirix.buxoff.transactions";

	// to store the tags
	public final static String PREF_DESC = "com.whirix.buxoff.all_desc";
	public final static String PREF_TAGS = "com.whirix.buxoff.all_tags";
	public final static String PREF_ACCT = "com.whirix.buxoff.all_accounts";
	public final static String PREF_RULES = "com.whirix.buxoff.rules";
	
	public Storage(Context context) {
		_context = context;
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
