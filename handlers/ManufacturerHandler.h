#pragma once
#include "../repositories/ManufacturerRepository.h"

class ManufacturerHandler {
public:
    explicit ManufacturerHandler(IManufacturerRepository& repo) : repo_(repo) {}

    void handleAdd();
    void handleList();
    void handleDelete();
    void handleUpdate();

private:
    IManufacturerRepository& repo_;
};
