package com.sevencrayons.buxoff;

public class Buxoff {
    public native void init(String filename);
    public native void add(String amount, String desc, String tag, String account);
    public native String push(String amount, String desc, String tag, String account);
    public native int count();
    public native String subject();
    public native boolean enableAdd(String amount, String account);
    public native boolean enablePush(int records_count, String amount, String account, String email);

    public native String udGet(String key);
    public native void udPut(String key, String value);

    static {
        System.loadLibrary("buxoff");
    }
}
