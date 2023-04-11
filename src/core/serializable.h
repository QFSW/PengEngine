#pragma once

#include <vector>
#include <functional>

struct Archive;

class Serializable
{
public:
    Serializable() = default;
    virtual ~Serializable() = default;


    virtual void serialize(Archive& archive) const;
    virtual void deserialize(const Archive& archive);

protected:
    void add_serializer(std::function<void(Archive& archive)>&& serializer);
    void add_deserializer(std::function<void(const Archive& archive)>&& deserializer);

private:
    std::vector<std::function<void(Archive& archive)>> _serializers;
    std::vector<std::function<void(const Archive& archive)>> _deserializers;
};
