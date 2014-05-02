package com.iutinvg.buxoff;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;

public class ErrorAlertDialogFragment extends DialogFragment {

    public static ErrorAlertDialogFragment newInstance(String message) {
        ErrorAlertDialogFragment frag = new ErrorAlertDialogFragment();
        Bundle args = new Bundle();
        args.putString("message", message);
        frag.setArguments(args);
        return frag;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        String message = getArguments().getString("message");

        return new AlertDialog.Builder(getActivity())
                //.setIcon(R.drawable.alert_dialog_icon)
                .setTitle(R.string.error_alert_dialog_title)
                .setMessage(message)
                .setPositiveButton(R.string.error_alert_dialog_ok,
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int whichButton) {
                            //((FragmentAlertDialog)getActivity()).doPositiveClick();
                        }
                    }
                )
                .create();
    }
}
