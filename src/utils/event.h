#pragma once

#include <vector>
#include <functional>
#include <tuple>
#include <string>

#include "vectools.h"

namespace utils
{
	template <typename...Args>
	class EventInterface
	{
	public:
		using invocable = std::function<void(Args...)>;
		using listener_handle = int32_t;
		using named_listener = std::tuple<invocable, std::string, listener_handle>;

		listener_handle subscribe(invocable&& listener, const std::string& name = "");
		listener_handle subscribe_once(invocable&& listener, const std::string& name = "");

		void unsubscribe(listener_handle handle);
		void unsubscribe(const std::string& name);

		static constexpr listener_handle null_handle = -1;

	protected:
		EventInterface() = default;

		void subscribe(invocable&& listener, const std::string& name, listener_handle handle);
		void process_pending_actions();

		listener_handle get_new_handle();

		listener_handle _next_handle = 0;
		std::vector<named_listener> _listeners;
		std::vector<std::function<void()>> _pending_actions;
		bool _is_invoking = false;
	};

	template <typename...Args>
	class Event : public EventInterface<Args...>
	{
	public:
		using invocable = EventInterface<Args...>::invocable;
		using named_listener = EventInterface<Args...>::named_listener;

		void invoke(Args...args);
		void operator()(Args...args);
	};

	template <typename...Args>
	EventInterface<Args...>::listener_handle EventInterface<Args...>::subscribe(invocable&& listener, const std::string& name)
	{
		listener_handle handle = get_new_handle();
		subscribe(std::move(listener), name, handle);

		return handle;
	}

	template <typename ... Args>
	typename EventInterface<Args...>::listener_handle EventInterface<Args...>::subscribe_once(invocable&& listener,
		const std::string& name)
	{
		listener_handle handle = get_new_handle();
		invocable listener_once = [inner = std::move(listener), handle, this](Args...args)
		{
			inner(std::forward(args)...);
			unsubscribe(handle);
		};

		subscribe(std::move(listener_once), name, handle);
		return handle;
	}

	template <typename...Args>
	void EventInterface<Args...>::subscribe(invocable&& listener, const std::string& name, listener_handle handle)
	{
		if (_is_invoking)
		{
			_pending_actions.push_back([this, listener = std::move(listener), name, handle]() mutable {
				subscribe(std::move(listener), name, handle);
			});
		}
		else
		{

			_listeners.push_back(named_listener(std::move(listener), name, handle));
		}
	}

	template <typename...Args>
	void EventInterface<Args...>::unsubscribe(listener_handle handle)
	{
		if (_is_invoking)
		{
			_pending_actions.push_back([this, handle] {
				unsubscribe(handle);
			});
		}
		else
		{
			vectools::remove_all<named_listener>(_listeners, [&](const named_listener& listener)
			{
				return std::get<listener_handle>(listener) == handle;
			});
		}
	}

	template <typename...Args>
	void EventInterface<Args...>::unsubscribe(const std::string& name)
	{
		if (_is_invoking)
		{
			_pending_actions.push_back([this, name] {
				unsubscribe(name);
			});
		}
		else
		{
			vectools::remove_all<named_listener>(_listeners, [&](const named_listener& listener)
			{
				return std::get<std::string>(listener) == name;
			});
		}
	}

	template <typename...Args>
	EventInterface<Args...>::listener_handle EventInterface<Args...>::get_new_handle()
	{
		return _next_handle++;
	}

	template <typename...Args>
	void EventInterface<Args...>::process_pending_actions()
	{
		assert(!_is_invoking);

		for (const std::function<void()>& action : _pending_actions)
		{
			action();
		}

		_pending_actions.clear();
	}

	template <typename...Args>
	void Event<Args...>::invoke(Args...args)
	{
		this->_is_invoking = true;

		for (const named_listener& listener : this->_listeners)
		{
			std::get<invocable>(listener)(std::forward(args)...);
		}

		this->_is_invoking = false;
		this->process_pending_actions();
	}

	template <typename...Args>
	void Event<Args...>::operator()(Args...args)
	{
		invoke(std::forward(args)...);
	}
}

#define DEFINE_EVENT(name, ...) \
public: \
	utils::EventInterface<__VA_ARGS__>& ##name() noexcept { return _##name; } \
private: \
	utils::Event<__VA_ARGS__> _##name;