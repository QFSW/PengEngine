#pragma once

#include <memory/shared_ref.h>
#include <common/common.h>

#include "reflected_type.h"

// A general factory for constructing items with a base of ItemBase, using Args...
// Factory functions must be registered for each type before they can be constructed
template <typename ItemBase, typename...Args>
class ItemFactory
{
public:
	static ItemFactory& get();

	ItemFactory(const ItemFactory&) = delete;
	ItemFactory(ItemFactory&&) = delete;

	// TODO: add a register_factory function
	// TODO: add a construct_item function

private:
	ItemFactory() = default;

	common::unordered_map<peng::shared_ref<const ReflectedType>, std::function<Entity(Args...)>> _type_to_factory;
};

template <typename ItemBase, typename ... Args>
ItemFactory<ItemBase, Args...>& ItemFactory<ItemBase, Args...>::get()
{
	static ItemFactory item_factory;
	return item_factory;
}
