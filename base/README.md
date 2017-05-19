# How to Build

## Android build
```
cd ~/buxoff/base/android/jni
~/Library/Android/sdk/ndk-bundle/ndk-build
```

Now open the project in Android Studio and build.

## iOS build

First of all, build static LevelDB lib
```
cd ~/buxoff/base/deps/leveldb
make PLATFORM=IOS SHARED_OUTDIR=out-shared
```

`SHARED_OUTDIR=out-shared` is necessary as a workaround for a bug in building system:
https://github.com/google/leveldb/issues/467
Probably, it will be fixed soon.

Then you can build static libbuxoff
```
cd ~/buxoff/base
make
```

Now open the project in Xcode and build.


# Other Useful Things

## Tests run
```
cd ~/buxoff/base/deps/leveldb
make
cd ~/buxoff/base
make tests
```

## Clean
```
cd ~/buxoff/base
make clean
```
