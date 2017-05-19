## Android build
cd ~/buxoff/base/android/jni
~/Library/Android/sdk/ndk-bundle/ndk-build

## iOS build
cd ~/buxoff/base
make

Build static LevelDB.
```
cd ~/buxoff/base/deps/leveldb
make PLATFORM=IOS SHARED_OUTDIR=out-shared
```

`so_out` is necessary as a workaround for a bug in building system. Probably,
it will be fixed soon.


## Tests run
make tests

Don't forget to build LevelDB before:
cd ~/buxoff/base/deps/leveldb
make

## Clean
make clean
