#include "superluminal_profiler.h"

#include <windows.h>
#include <libs/superluminal/PerformanceAPI_loader.h>
#include <core/logger.h>

using namespace profiling;

SuperluminalProfiler::SuperluminalProfiler()
{
	// TODO: would be good if we could avoid hard-coding this somehow
	constexpr const wchar_t* dll_path = LR"(C:\Program Files\Superluminal\Performance\API\dll\x64\PerformanceAPI.dll)";

	_module = PerformanceAPI_LoadFrom(dll_path, &_functions);
	if (_module)
	{
		Logger::success("Superluminal profiler initialized");
	}
	else
	{
		Logger::error("Failed to load Superluminal profiler");
	}
}

SuperluminalProfiler::~SuperluminalProfiler()
{
	if (_module)
	{
		PerformanceAPI_ModuleHandle module_typed = static_cast<PerformanceAPI_ModuleHandle>(_module);
		PerformanceAPI_Free(&module_typed);

		Logger::log("Shutdown Superluminal profiler");
	}
}

void SuperluminalProfiler::begin_event(const EventData& event)
{
	if (_module)
	{
		const uint32_t color_packed = PERFORMANCEAPI_MAKE_COLOR(event.color.x, event.color.y, event.color.z);
		_functions.BeginEvent(event.id, event.extra, color_packed);
	}
}

void SuperluminalProfiler::end_event()
{
	if (_module)
	{
		_functions.EndEvent();
	}
}
