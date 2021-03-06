package com.sevencrayons.buxoff;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.AutoCompleteTextView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    static final String TAG = "MainActivity";

    Buxoff buxoff;
    UserDefaults ud;
    TagsStorage tagsStorage;
    RulesStorage rulesStorage;


    TextView labelStats;
    EditText textAmount;
    AutoCompleteTextView textDescription;
    AutoCompleteTextView textTag;
    boolean tagWasResolved = true; // flag tag was set by a resolution
    EditText textAccount;
    EditText textEmail;
    Button buttonAdd;
    Button buttonPush;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        initBuxoff();
        initViews();
        initButtons();
        initEditTexts();
        initTags();
        initPersistentEditTexts();
        initRulesResolution();

        updateButtons();
        updateUI();
    }

    private void initBuxoff() {
        buxoff = new Buxoff(Utils.getDocPath(this));
        ud = new UserDefaults();
        tagsStorage = new TagsStorage();
        rulesStorage = new RulesStorage();
    }

    private void initViews() {
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        labelStats = (TextView) findViewById(R.id.textStats);
        textAmount = (EditText) findViewById(R.id.textAmount);
        textDescription = (AutoCompleteTextView) findViewById(R.id.textDescription);
        textTag = (AutoCompleteTextView) findViewById(R.id.textTags);
        textAccount = (EditText) findViewById(R.id.textAccount);
        textEmail = (EditText) findViewById(R.id.textEmail);

        buttonAdd = (Button) findViewById(R.id.buttonSave);
        buttonPush = (Button) findViewById(R.id.buttonPush);
    }

    private void initEditTexts() {
        List<EditText> l = Arrays.asList(textAmount, textDescription, textAccount, textEmail);
        TextWatcher tw = new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {}

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                updateButtons();
            }

            @Override
            public void afterTextChanged(Editable editable) {}
        };

        for (EditText e : l) {
            e.addTextChangedListener(tw);
        }
    }

    private void initPersistentEditTexts() {
        List<EditText> l = Arrays.asList(textAccount, textEmail);
        for (final EditText e : l) {
            final String key = (String)e.getTag();
            e.setText(ud.get(key, ""));

            e.addTextChangedListener(new TextWatcher() {
                @Override
                public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {}

                @Override
                public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                    String value = charSequence.toString();
                    ud.put(key, value);
                }

                @Override
                public void afterTextChanged(Editable editable) {}
            });
        }
    }

    private void initRulesResolution() {
        textDescription.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {}

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                resolveRule();
            }

            @Override
            public void afterTextChanged(Editable editable) {}
        });
    }

    private void initTags() {
        textTag.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {}

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                tagWasResolved = false;
            }

            @Override
            public void afterTextChanged(Editable editable) {}
        });
    }

    private void initButtons() {
        buttonAdd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                add(view);
            }
        });
        buttonPush.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                push(view);
            }
        });
    }

    private void updateButtons() {
        Log.d(TAG, "update buttons");
        int count = buxoff.count();
        String amount = textAmount.getText().toString();
        String account = textAccount.getText().toString();
        String email = textEmail.getText().toString();
        buttonAdd.setEnabled(buxoff.enableAdd(amount, account));
        buttonPush.setEnabled(buxoff.enablePush(count, amount, account, email));
        labelStats.setText(getString(R.string.total) + count);
    }

    private void updateTagsAutocompletion() {
        String[] l = tagsStorage.all();
        Log.d(TAG, "tags autocompletion: " + Arrays.toString(l));
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_dropdown_item_1line, l);
        textTag.setAdapter(adapter);
    }

    private void updateRulesAutocompletion() {
        String[] l = rulesStorage.all();
        Log.d(TAG, "rules autocompletion: " + Arrays.toString(l));
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_dropdown_item_1line, l);
        textDescription.setAdapter(adapter);
    }

    private void updateUI() {
        Log.d(TAG, "updateUI");
        updateTagsAutocompletion();
        updateRulesAutocompletion();
    }

    private void resolveRule() {
        Log.d(TAG, "try to resolve rule...");
        String tag = textTag.getText().toString();
        // we won't tag if it was set by user
        if (tag.isEmpty() || tagWasResolved) {
            String desc = textDescription.getText().toString();
            textTag.setText(rulesStorage.tags(desc));
            Log.d(TAG, "resolve rule for " + desc + "...");
            tagWasResolved = true;
        } else {
            Log.d(TAG, "but it's empty or set by user");
        }
    }

    private void add(View view) {
        try {
            buxoff.add(textAmount.getText().toString(), textDescription.getText().toString(),
                    textTag.getText().toString(), textAccount.getText().toString());
            updateStatus(view, getString(R.string.saved));
            clearText();
            updateUI();
        } catch (RuntimeException e) {
            updateStatus(view, e.getMessage());
        }
    }

    private void push(View view) {
        try {
            String body = buxoff.push(textAmount.getText().toString(), textDescription.getText().toString(),
                    textTag.getText().toString(), textAccount.getText().toString());
            clearText();
            sendEmail(buxoff.subject(), body);
            updateUI();
        } catch (RuntimeException e) {
            updateStatus(view, e.getMessage());
        }
    }

    private void updateStatus(View view, String msg) {
        Snackbar.make(view, msg, Snackbar.LENGTH_LONG)
                .setAction("Action", null).show();
    }

    private void clearText() {
        // we don't clear account, it tends to be the same
        List<EditText> l = Arrays.asList(textAmount, textDescription, textTag);
        for (EditText e : l) {
            e.setText("");
        }

        Utils.focus(this, textAmount);
    }

    private void sendEmail(String subject, String body){
        Intent emailIntent = new Intent(Intent.ACTION_SENDTO);
        emailIntent.setData(Uri.parse("mailto:" + ud.get((String)textEmail.getTag(), "")));
        emailIntent.putExtra(Intent.EXTRA_SUBJECT, subject);
        emailIntent.putExtra(Intent.EXTRA_TEXT, body);
        startActivity(Intent.createChooser(emailIntent, getString(R.string.send_using)));
//        } catch (android.content.ActivityNotFoundException ex) {
//            throw new RuntimeException("No email clients installed.");
////            Toast.makeText(this, "No email clients installed.", Toast.LENGTH_SHORT).show();
//        }
    }

//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        // Inflate the menu; this adds items to the action bar if it is present.
//        getMenuInflater().inflate(R.menu.menu_main, menu);
//        return true;
//    }

//    @Override
//    public boolean onOptionsItemSelected(MenuItem item) {
//        // Handle action bar item clicks here. The action bar will
//        // automatically handle clicks on the Home/Up button, so long
//        // as you specify a parent activity in AndroidManifest.xml.
//        int id = item.getItemId();
//
//        //noinspection SimplifiableIfStatement
//        if (id == R.id.action_settings) {
//            return true;
//        }
//
//        return super.onOptionsItemSelected(item);
//    }
}
