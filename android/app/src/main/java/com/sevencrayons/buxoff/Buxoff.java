package com.sevencrayons.buxoff;

public class Buxoff {
    public native void init(String filename);
    public native void add();
    public native int count();

    static {
        System.loadLibrary("buxoff");
    }
}
