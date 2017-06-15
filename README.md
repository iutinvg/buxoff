# What?

Simple offline buxfer.com client.

Buxoff is short for a Buxfer Offline. And this is yet another client app for the famous
financial tracker. It is just to log your expenses.

There are three main differences from the other clients:

1. it is offline
2. that's why it is fast
3. it is safe (read below)


# Who?

This is application for those who are traveling a lot and always on the run. Yes, 3G / 4G / LTE is everywhere nowadays, but, alas, sometime it is terribly slow or too expensive. Do you like international roaming?


# Why?

Buxoff is not yet another slow / buggy web app forcing you to wait for ages until it loads.
This is a native mobile application working as fast as your phone can. You can save all your
expenses on the spot and push them to buxfer.com later.

The transaction properties (tags, transactions, rules) are saved locally on device and autocompletion helps you fill the fields. The more you use the app, the more rules it knows.

Buxoff uses email interface of buxfer.com, thus it doesn't require your buxfer account credentials
and even Internet connection.


# More

Support page: http://sevencrayons.com/buxoff

This project is a sort of an academic project now. It's my investigation about how to create C++ cross platform code for mobiles. So it's my hobby C++ project. Some technical details could be a point of your interest:

* all "business logic" is implemented in C++, UI is implemented on a platform technology
* persistent layer is [LevelDB](https://github.com/google/leveldb)
* The records are stored in [JSON](https://github.com/nlohmann/json) format in the database
* [Catch](https://github.com/philsquared/Catch) is used for unit-testing
* `make` is used for building of libraries for Android and iPhone
* Play Market App: https://play.google.com/store/apps/details?id=com.sevencrayons.buxoff


## Reading
Some interesting things about cross mobile C++ development:

* was an inspiration for me: http://oleb.net/blog/2014/05/how-dropbox-uses-cplusplus-cross-platform-development/
* an idea how to build LevelDB for Android: https://github.com/litl/android-leveldb/blob/master/src/main/jni/Android.mk
* very useful list of C++ libs / tools: https://github.com/fffaraz/awesome-cpp
* good JNI examples / patterns can be found here: http://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html
