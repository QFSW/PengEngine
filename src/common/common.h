#pragma once

#include <libs/tsl/robin_map.h>
#include <libs/tsl/robin_set.h>
#include <libs/moodycamel/concurrentqueue.h>
#include <libs/moodycamel/blockingconcurrentqueue.h>
#include <libs/moodycamel/readerwriterqueue.h>

namespace common
{
    template <typename TKey, typename TVal>
    using unordered_map = tsl::robin_map<TKey, TVal>;

    template <typename T>
    using unordered_set = tsl::robin_set<T>;

    template <typename T>
    using concurrent_queue = moodycamel::ConcurrentQueue<T>;

    template <typename T>
    using blocking_concurrent_queue = moodycamel::BlockingConcurrentQueue<T>;

    template <typename T>
    using spsc_queue = moodycamel::ReaderWriterQueue<T>;

    template <typename T>
    using blocking_spsc_queue = moodycamel::BlockingReaderWriterQueue<T>;
}