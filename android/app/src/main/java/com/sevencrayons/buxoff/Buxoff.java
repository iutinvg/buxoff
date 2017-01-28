package com.sevencrayons.buxoff;

public class Buxoff {
    public native void init(String filename);
    public native void add(String amount, String desc, String tag, String account);
    public native int count();
    public native void exc();

    static {
        System.loadLibrary("buxoff");
    }
}
