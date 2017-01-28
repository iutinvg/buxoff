package com.sevencrayons.buxoff;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    Buxoff buxoff;
    TextView labelStats;
    EditText textAmount;
    EditText textDescription;
    EditText textTag;
    EditText textAccount;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        initViews();
        initFab();
        initBuxoff();
    }

    private void initBuxoff() {
        buxoff = new Buxoff();
        buxoff.init(Utils.getDocPath(this));
    }

    private void initViews() {
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        labelStats = (TextView) findViewById(R.id.textStats);
        textAmount = (EditText) findViewById(R.id.textAmount);
        textDescription = (EditText) findViewById(R.id.textDescription);
        textTag = (EditText) findViewById(R.id.textTags);
        textAccount = (EditText) findViewById(R.id.textAccount);
    }

    private void initFab() {
        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                addRecord(view);
            }
        });
    }

    private void addRecord(View view) {
        try {
            buxoff.add(textAmount.getText().toString(), textDescription.getText().toString(),
                    textTag.getText().toString(), textAccount.getText().toString());
            updateStatus(view, "Record is saved");
            clearText();
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

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

//    protected String updateCount() {
//        try {
//            Log.e("Hm...", "here");
//            buxoff.exc();
//            return "No exception";
//        } catch (Exception e) {
//            return e.getMessage();
//        }
//        // buxoff.add("", "", "", "");
//        // buxoff.count();
//        // labelStats.setText("Total: " + buxoff.count());
//    }
}
