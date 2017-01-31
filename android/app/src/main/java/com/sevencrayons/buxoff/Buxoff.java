package com.sevencrayons.buxoff;

public class Buxoff {
    public native void init(String filename);
    public native void add(String amount, String desc, String tag, String account);
    public native String push(String amount, String desc, String tag, String account);
    public native int count();
    public native String subject();

    static {
        System.loadLibrary("buxoff");
    }
}
