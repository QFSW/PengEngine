#pragma once

#include <vector>
#include <functional>
#include <tuple>
#include <string>

namespace utils
{
	template <typename...Args>
	class EventInterface
	{
	public:
		using invocable = std::function<void(Args...)>;
		using named_listener = std::tuple<invocable, std::string>;

		void subscribe(invocable&& listener, const std::string& name = "N/A");
		void unsubscribe(const std::string& name);

	protected:
		EventInterface() = default;
		void process_pending_actions();

		std::vector<named_listener> _listeners;
		std::vector<std::function<void()>> _pending_actions;
		bool _is_invoking = false;
	};

	template <typename...Args>
	class Event : public EventInterface<Args...>
	{
	public:
		using named_listener = EventInterface<Args...>::named_listener;

		void invoke(Args...args);
		void operator()(Args...args);
	};

	template <typename...Args>
	void EventInterface<Args...>::subscribe(invocable&& listener, const std::string& name)
	{
		if (_is_invoking)
		{
			_pending_actions.push_back([this, listener=std::move(listener), name]() mutable {
				subscribe(std::move(listener), name);
			});
		}
		else
		{
			_listeners.push_back(named_listener(std::move(listener), name));
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
			_listeners.erase(std::remove_if(
				_listeners.begin(),
				_listeners.end(),
				[&](const named_listener& listener)
				{
					return std::get<1>(listener) == name;
				}
			));
		}
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
			std::get<0>(listener)(std::forward(args)...);
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