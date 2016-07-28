CXXFLAGS=-c -Wall -std=c++0x -O2 -Ideps/leveldb/include/ -Ideps/catch/include/
SOURCES=$(wildcard *.cc)

PLATFORM_OUTDIR=x86_64

STATIC_LIBOBJECTS := $(addprefix $(PLATFORM_OUTDIR)/, $(SOURCES:.cc=.o))
STATIC=$(PLATFORM_OUTDIR)/libbuxoff.a
LIBS=-Ldeps/leveldb/out-static/ -lleveldb -L$(PLATFORM_OUTDIR) -lbuxoff

TSOURCES=$(wildcard tests/*.cc)
TOBJECTS=$(TSOURCES:.cc=.o)
TEXE=tests/tests

ifeq ($(PLATFORM), IOS)
# Note: iOS should probably be using libtool, not ar.
AR=xcrun ar
LIBS=-Ldeps/leveldb/out-ios-universal -lleveldb -L$(PLATFORM_OUTDIR) -lbuxoff
SIMULATORSDK=$(shell xcrun -sdk iphonesimulator --show-sdk-path)
DEVICESDK=$(shell xcrun -sdk iphoneos --show-sdk-path)
DEVICE_CFLAGS = -isysroot "$(DEVICESDK)" -arch armv6 -arch armv7 -arch armv7s -arch arm64
SIMULATOR_CFLAGS = -isysroot "$(SIMULATORSDK)" -arch i686 -arch x86_64

PLATFORM_OUTDIR=ios

SIMULATOR_OUTDIR=$(PLATFORM_OUTDIR)/simulator
DEVICE_OUTDIR=$(PLATFORM_OUTDIR)/device

DEVICE_LIBOBJECTS := $(addprefix $(DEVICE_OUTDIR)/, $(SOURCES:.cc=.o))
SIMULATOR_LIBOBJECTS := $(addprefix $(SIMULATOR_OUTDIR)/, $(SOURCES:.cc=.o))

SIMULATOR_STATIC=$(SIMULATOR_OUTDIR)/libbuxoff.a
DEVICE_STATIC=$(DEVICE_OUTDIR)/libbuxoff.a
endif

ifeq ($(PLATFORM), IOS)
all: $(STATIC)
$(DEVICE_STATIC): $(PLATFORM_OUTDIR) $(DEVICE_LIBOBJECTS)
	$(AR) -rs $(DEVICE_STATIC) $(DEVICE_LIBOBJECTS)

$(SIMULATOR_STATIC): $(PLATFORM_OUTDIR) $(SIMULATOR_LIBOBJECTS)
	$(AR) -rs $(SIMULATOR_STATIC) $(SIMULATOR_LIBOBJECTS)

$(STATIC): $(PLATFORM_OUTDIR) $(DEVICE_STATIC) $(SIMULATOR_STATIC)
	lipo -create $(DEVICE_STATIC) $(SIMULATOR_STATIC) -output $@

$(PLATFORM_OUTDIR): $(SIMULATOR_OUTDIR) $(DEVICE_OUTDIR)

$(SIMULATOR_OUTDIR) $(DEVICE_OUTDIR):
	mkdir -p $@

$(SIMULATOR_OUTDIR)/%.o: %.cc
	xcrun -sdk iphonesimulator $(CXX) $(CXXFLAGS) $(SIMULATOR_CFLAGS) -c $< -o $@

$(DEVICE_OUTDIR)/%.o: %.cc
	xcrun -sdk iphoneos $(CXX) $(CXXFLAGS) $(DEVICE_CFLAGS) -c $< -o $@
else
all: $(STATIC)
$(STATIC): $(PLATFORM_OUTDIR) $(STATIC_LIBOBJECTS)
	$(AR) -rs $(STATIC) $(STATIC_LIBOBJECTS)

$(PLATFORM_OUTDIR):
	mkdir $@

EXECUTABLE=tests/tests
tests: $(EXECUTABLE) $(TOBJECTS) $(STATIC_LIBOBJECTS)
	./$(EXECUTABLE)

$(EXECUTABLE): $(TOBJECTS) $(STATIC)
	$(CXX) $(TOBJECTS) $(LIBS) -o $@

$(PLATFORM_OUTDIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

$(PLATFORM_OUTDIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@
endif

clean:
	rm -rf tests/*.o test.db tests/tests x86_64 ios
