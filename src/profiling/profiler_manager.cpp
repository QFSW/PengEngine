#include "profiler_manager.h"

#include "stub_profiler.h"

using namespace profiling;

void ProfilerManager::load_profiler(std::unique_ptr<IProfiler>&& profiler)
{
	_current_profiler = std::move(profiler);
}

const std::unique_ptr<IProfiler>& ProfilerManager::current_profiler()
{
	if (!_current_profiler)
	{
		_current_profiler = std::make_unique<StubProfiler>();
	}

	return _current_profiler;
}
