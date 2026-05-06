#pragma once
#include "IManufacturerRepository.h"
#include "../db/Database.h"

class ManufacturerRepository : public IManufacturerRepository {
public:
    bool add(const Manufacturer& m) override;
    bool update(const Manufacturer& m) override;
    bool remove(int id) override;
    std::optional<Manufacturer> getById(int id) override;
    std::optional<Manufacturer> getByName(const std::string& name) override;
    std::vector<Manufacturer> getAll() override;

private:
    static Manufacturer fromStatement(sqlite3_stmt* stmt);
};
