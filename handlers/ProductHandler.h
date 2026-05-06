#pragma once
#include "../repositories/IProductRepository.h"
#include "../repositories/IManufacturerRepository.h"

class ProductHandler {
public:
    ProductHandler(IProductRepository& productRepo,
                   IManufacturerRepository& manufacturerRepo)
        : productRepo_(productRepo),
          manufacturerRepo_(manufacturerRepo) {}

    void handleList();
    void handleGet();
    void handleAdd();
    void handleUpdate();
    void handleDelete();

private:
    IProductRepository& productRepo_;
    IManufacturerRepository& manufacturerRepo_;

    int pickManufacturer();
};
