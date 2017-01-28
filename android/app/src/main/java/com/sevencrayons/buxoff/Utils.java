package com.sevencrayons.buxoff;

import android.content.Context;
import android.content.ContextWrapper;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

import java.io.File;

public class Utils {
    public static String getDocPath(ContextWrapper cw) {
        File data = cw.getCacheDir();
        File child = new File(data, "MyData");
        return child.getAbsolutePath();
    }

    // http://stackoverflow.com/a/27750335/444966
    public static void focus(ContextWrapper cw, EditText v) {
        v.requestFocus();
        InputMethodManager imm = (InputMethodManager) cw.getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(v, InputMethodManager.SHOW_IMPLICIT);
    }
}
