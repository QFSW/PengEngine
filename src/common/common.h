#pragma once

#pragma warning( push, 0 )
#include <libs/moodycamel/concurrentqueue.h>
#include <libs/moodycamel/blockingconcurrentqueue.h>
#include <libs/moodycamel/readerwriterqueue.h>
#pragma warning( pop )

namespace common
{
    template <typename T>
    using concurrent_queue = moodycamel::ConcurrentQueue<T>;

    template <typename T>
    using blocking_concurrent_queue = moodycamel::BlockingConcurrentQueue<T>;

    template <typename T>
    using spsc_queue = moodycamel::ReaderWriterQueue<T>;

    template <typename T>
    using blocking_spsc_queue = moodycamel::BlockingReaderWriterQueue<T>;
}