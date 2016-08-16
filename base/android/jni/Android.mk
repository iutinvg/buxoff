LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := buxoff
LOCAL_SRC_FILES := ../../Storage.cc ../../Record.cc ../../Email.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include $(LOCAL_PATH)/../../deps/catch/include $(LOCAL_PATH)/../../deps/json/src $(LOCAL_PATH)/../../deps/leveldb/include $(LOCAL_PATH)/../../deps/leveldb
LOCAL_STATIC_LIBRARIES += leveldb
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := leveldb
LOCAL_CFLAGS := -D_REENTRANT -DOS_ANDROID -DLEVELDB_PLATFORM_POSIX -DNDEBUG
LOCAL_CPP_EXTENSION := .cc
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../deps/leveldb/include $(LOCAL_PATH)/../../deps/leveldb
LOCAL_SRC_FILES := ../../deps/leveldb/db/builder.cc ../../deps/leveldb/db/c.cc ../../deps/leveldb/db/db_impl.cc ../../deps/leveldb/db/db_iter.cc ../../deps/leveldb/db/dbformat.cc ../../deps/leveldb/db/filename.cc ../../deps/leveldb/db/log_reader.cc ../../deps/leveldb/db/log_writer.cc ../../deps/leveldb/db/memtable.cc ../../deps/leveldb/db/repair.cc ../../deps/leveldb/db/table_cache.cc ../../deps/leveldb/db/version_edit.cc ../../deps/leveldb/db/version_set.cc ../../deps/leveldb/db/write_batch.cc ../../deps/leveldb/table/block.cc ../../deps/leveldb/table/block_builder.cc ../../deps/leveldb/table/filter_block.cc ../../deps/leveldb/table/format.cc ../../deps/leveldb/table/iterator.cc ../../deps/leveldb/table/merger.cc ../../deps/leveldb/table/table.cc ../../deps/leveldb/table/table_builder.cc ../../deps/leveldb/table/two_level_iterator.cc ../../deps/leveldb/util/arena.cc ../../deps/leveldb/util/bloom.cc ../../deps/leveldb/util/cache.cc ../../deps/leveldb/util/coding.cc ../../deps/leveldb/util/comparator.cc ../../deps/leveldb/util/crc32c.cc ../../deps/leveldb/util/env.cc ../../deps/leveldb/util/env_posix.cc ../../deps/leveldb/util/filter_policy.cc ../../deps/leveldb/util/hash.cc ../../deps/leveldb/util/histogram.cc ../../deps/leveldb/util/logging.cc ../../deps/leveldb/util/options.cc ../../deps/leveldb/util/status.cc ../../deps/leveldb/port/port_posix.cc
include $(BUILD_STATIC_LIBRARY)
