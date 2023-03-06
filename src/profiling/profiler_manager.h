#pragma once

#include <memory>
#include <utils/singleton.h>

#include "profiler.h"

namespace profiling
{
	class ProfilerManager : public utils::Singleton<ProfilerManager>
	{
		using Singleton::Singleton;

	public:
		// Loads a new profiler into the profiler manager releasing the existing profiler
		void load_profiler(std::unique_ptr<IProfiler>&& profiler);

		// Creates and loads a new profiler into the profiler manager
		template <std::derived_from<IProfiler> T>
		requires std::constructible_from<T>
		void load_profiler();

		// Gets the current profiler in use by the profiler manager
		// If no profiler has been loaded, a default stub profiler will be loaded
		const std::unique_ptr<IProfiler>& current_profiler();

	private:
		std::unique_ptr<IProfiler> _current_profiler;
	};

	template <std::derived_from<IProfiler> T>
	requires std::constructible_from<T>
	void ProfilerManager::load_profiler()
	{
		load_profiler(std::make_unique<T>());
	}
}
