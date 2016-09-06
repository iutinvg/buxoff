package com.sevencrayons.buxoff;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    TextView textCount;
    Button buttonAdd;
    Buxoff buxoff;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buxoff = new Buxoff();
        buxoff.init(getDocPath());

        textCount = (TextView) findViewById(R.id.text);
        buttonAdd = (Button) findViewById(R.id.button);

        buttonAdd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateCount();
            }
        });
    }

    protected String getDocPath() {
        File data = getCacheDir();
        File child = new File(data, "MyData");
        return child.getAbsolutePath();
    }

    protected void updateCount() {
        buxoff.add();
        textCount.setText("Total: " + buxoff.count());
    }
}
