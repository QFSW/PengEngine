#pragma once

#include <functional>

#include <memory/shared_ref.h>
#include <common/common.h>
#include <utils/strtools.h>

#include "reflected_type.h"

// A general factory for constructing items with a base of ItemBase, using Args...
// Factory functions must be registered for each type before they can be constructed
template <typename ItemBase, typename...Args>
class ItemFactory
{
public:
	void register_factory(
		peng::shared_ref<const ReflectedType> item_type,
		std::function<ItemBase(Args...)> factory
	);

	ItemBase construct_item(
		peng::shared_ref<const ReflectedType> item_type,
		Args&&...args
	) const;

private:
	common::unordered_map<peng::shared_ref<const ReflectedType>, std::function<ItemBase(Args...)>> _type_to_factory;
};

template <typename ItemBase, typename ... Args>
void ItemFactory<ItemBase, Args...>::register_factory(
	peng::shared_ref<const ReflectedType> item_type,
	std::function<ItemBase(Args...)> factory
)
{
	_type_to_factory[item_type] = factory;
}

template <typename ItemBase, typename ... Args>
ItemBase ItemFactory<ItemBase, Args...>::construct_item(
	peng::shared_ref<const ReflectedType> item_type,
	Args&&...args
) const
{
	if (const auto it = _type_to_factory.find(item_type); it != _type_to_factory.end())
	{
		return it->second(std::forward<Args>(args)...);
	}

	throw std::runtime_error(strtools::catf(
		"Cannot construct item of type '%s' as no corresponding factory exists",
		item_type->name.c_str()
	));
}
