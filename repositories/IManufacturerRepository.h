#pragma once
#include "IRepository.h"
#include "../Models/Manufacturer.h"

class IManufacturerRepository : public IRepository<Manufacturer> {
public:
    virtual std::optional<Manufacturer> getByName(const std::string& name) = 0;
};
