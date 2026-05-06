#pragma once
#include "IRepository.h"
#include "../Models/Product.h"
#include "../Models/ProductWithManufacturer.h"
#include <string>

class IProductRepository : public IRepository<Product> {
public:
    virtual std::vector<Product> getByManufacturer(int manufacturerId) = 0;
    virtual std::vector<Product> getByType(int productType) = 0;
    virtual bool updateQuantity(int id, int newQuantity) = 0;
    virtual int totalCount() = 0;
    virtual double totalValue() = 0;
    virtual  std::vector<ProductWithManufacturer> getAllWithManufacturer() = 0;
};
