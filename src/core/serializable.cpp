#include "serializable.h"

#include "archive.h"

void Serializable::serialize(Archive& archive) const
{
	for (const auto& serializer : _serializers)
	{
		serializer(archive);
	}
}

void Serializable::deserialize(const Archive& archive)
{
	for (const auto& deserializer : _deserializers)
	{
		deserializer(archive);
	}
}

void Serializable::add_serializer(std::function<void(Archive& archive)>&& serializer)
{
	_serializers.push_back(serializer);
}

void Serializable::add_deserializer(std::function<void(const Archive& archive)>&& deserializer)
{
	_deserializers.push_back(deserializer);
}