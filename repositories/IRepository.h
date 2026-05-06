#pragma once
#include <vector>
#include <optional>

template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;

    virtual bool add(const T& entity) = 0;
    virtual bool update(const T& entity) = 0;
    virtual bool remove(int id) = 0;
    virtual std::optional<T> getById(int id) = 0;
    virtual std::vector<T> getAll() = 0;
};
