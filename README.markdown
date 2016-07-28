# Build on Macos
Read:
https://github.com/hoisie/LevelDB-ObjC
https://github.com/litl/android-leveldb/blob/master/src/main/jni/Android.mk

make TARGET_OS=IOS
xcode-select --install
make TARGET_OS=OS_ANDROID_CROSSCOMPILE

lipo -create
out-ios-arm/libleveldb.a
out-ios-x86/libleveldb.a
-output out-ios-universal/libleveldb.a

xcrun -sdk iphoneos c++ -I. -I./include -std=c++0x  -DOS_MACOSX -DLEVELDB_PLATFORM_POSIX -DLEVELDB_ATOMIC_PRESENT -O2 -DNDEBUG -isysroot "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS9.3.sdk" -arch armv6 -arch armv7 -arch armv7s -arch arm64 -c util/status.cc -o out-ios-arm/util/status.o

xcrun -sdk iphonesimulator c++ -I. -I./include -std=c++0x  -DOS_MACOSX -DLEVELDB_PLATFORM_POSIX -DLEVELDB_ATOMIC_PRESENT -O2 -DNDEBUG -isysroot "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator9.3.sdk" -arch i686 -arch x86_64 -c port/port_posix.cc -o out-ios-x86/port/port_posix.o

xcrun ar -rs out-ios-arm/libmemenv.a out-ios-arm/helpers/memenv/memenv.o



#What?
Buxoff is short for a Buxfer Offline. And this is yet another client app for the famous
financial tracker. It is just to log your expenses.

There are three main differences from the other clients:

1. it is offline;
2. that's why it is fast;
3. it is safe (read below).

#Who?
This is application for those who are traveling a lot and always on the run. Yes, 3G is everywhere nowadays,
but, alas, sometime it is terribly slow or too expensive. Do you like international roaming?

#Why?
Buxoff is not one more slow / buggy web app forcing you to wait forages until it loads.
This is a native mobile application working as fast as you phone can. You can save all your
expenses on the spot and push them to server later -- when you are in free WiFi zone or 3G is fine.

The transaction properties (tags, transactions, rules) are saved locally on device and autocompletion
helps you fill the fields. The more you use the app, the more rules it knows.

Buxoff uses email interface of buxfer.com, thus it doesn't require your buxfer account credentials
and even Internet connection. And nobody can see your buxfer account using this app if you happy enough
to lose you mobile.

#More
Support page: http://sevencrayons.com/buxoff

Play Market App: https://play.google.com/store/apps/details?id=com.sevencrayons.buxoff
