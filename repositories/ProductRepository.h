#pragma once
#include "IProductRepository.h"
#include "../db/Database.h"

class ProductRepository : public IProductRepository {
public:
    bool add(const Product& p) override;
    bool update(const Product& p) override;
    bool remove(int id) override;
    std::optional<Product> getById(int id) override;
    std::vector<Product> getAll() override;
    std::vector<Product> getByManufacturer(int manufacturerId) override;
    std::vector<Product> getByType(int productType) override;
    bool updateQuantity(int id, int newQuantity) override;
    int totalCount() override;
    double totalValue() override;
    std::vector<ProductWithManufacturer> getAllWithManufacturer() override;
private:
    static Product fromStatement(sqlite3_stmt* stmt);
};
