## Android build
cd ~/buxoff/base/android/jni
~/Library/Android/sdk/ndk-bundle/ndk-build

## iOS build
cd ~/buxoff/base
make

## Tests run
make tests

Don't forget to build LevelDB before:
cd ~/buxoff/base/deps/leveldb
make

## Clean
make clean
