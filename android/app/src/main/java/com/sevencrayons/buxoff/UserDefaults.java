package com.sevencrayons.buxoff;

public class UserDefaults {
    public native String get(String key, String def);
    public native void put(String key, String value);
}
