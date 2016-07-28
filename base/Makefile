CXXFLAGS=-c -Wall -std=c++0x -Ideps/leveldb/include/
SOURCES=Storage.cc \
	Record.cc

PLATFORM_OUTDIR=x86_64
STATIC_LIBOBJECTS := $(addprefix $(PLATFORM_OUTDIR)/, $(SOURCES:.cc=.o))

TSOURCES=tests/tests_record.cc \
	tests/tests_storage.cc
TOBJECTS=$(TSOURCES:.cc=.o)
STATIC=$(PLATFORM_OUTDIR)/libbuxoff.a
LIBS=-Ldeps/leveldb/out-static/ -lleveldb -L$(PLATFORM_OUTDIR) -lbuxoff

ifeq ($(PLATFORM), IOS)
# Note: iOS should probably be using libtool, not ar.
AR=xcrun ar
LIBS=-Ldeps/leveldb/out-ios-universal -lleveldb -L. -lbuxoff
SIMULATORSDK=$(shell xcrun -sdk iphonesimulator --show-sdk-path)
DEVICESDK=$(shell xcrun -sdk iphoneos --show-sdk-path)
DEVICE_CFLAGS = -isysroot "$(DEVICESDK)" -arch armv6 -arch armv7 -arch armv7s -arch arm64
SIMULATOR_CFLAGS = -isysroot "$(SIMULATORSDK)" -arch i686 -arch x86_64
PLATFORM_OUTDIR=ios

DEVICE_LIBOBJECTS := $(addprefix $(DEVICE_OUTDIR)/, $(SOURCES:.cc=.o))
SIMULATOR_LIBOBJECTS := $(addprefix $(SIMULATOR_OUTDIR)/, $(SOURCES:.cc=.o))

SIMULATOR_OUTDIR=$(PLATFORM_OUTDIR)/simulator
DEVICE_OUTDIR=$(PLATFORM_OUTDIR)/device

DEVICE_ALLOBJS := $(DEVICE_LIBOBJECTS) $(DEVICE_MEMENVOBJECTS)
SIMULATOR_ALLOBJS := $(SIMULATOR_LIBOBJECTS) $(SIMULATOR_MEMENVOBJECTS)
endif

ifeq ($(PLATFORM), IOS)
all: $(STATIC)
$(STATIC): $(PLATFORM_OUTDIR) $(STATIC_LIBOBJECTS)
	$(AR) -rs $(STATIC) $(STATIC_LIBOBJECTS)

$(PLATFORM_OUTDIR):
	mkdir -p $(SIMULATOR_OUTDIR)
	mkdir -p $(DEVICE_OUTDIR)

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

tests: $(TOBJECTS) $(STATIC)
	$(CXX) $(TOBJECTS) $(LIBS) -o tests/$@
	./tests/tests

$(PLATFORM_OUTDIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@
endif

clean:
	rm -rf $(STATIC) *.o *.a \
		tests/*.o test.db tests/tests $(PLATFORM_OUTDIR)
