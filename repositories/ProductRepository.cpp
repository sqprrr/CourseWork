#include "ProductRepository.h"
#include "Models/ProductType.h"
#include "Models/Validator.h"
#include <iostream>


Product ProductRepository::fromStatement(sqlite3_stmt* stmt) {
    Product p;
    p.id            = sqlite3_column_int(stmt, 0);
    p.name          = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    p.price         = sqlite3_column_double(stmt, 2);
    p.quantity      = sqlite3_column_int(stmt, 3);
    p.product_type  = static_cast<ProductType>(sqlite3_column_int(stmt, 4));
    p.manufacturer_id = sqlite3_column_int(stmt, 5);
    return p;
}

bool ProductRepository::add(const Product& p) {
    try {
            Validator::validate(p);
        } catch (const std::invalid_argument& e) {
            std::cerr << "[Validation] " << e.what() << "\n";
            return false;
        }

    auto* db = Database::getInstance().get();
    const char* sql =
        "INSERT INTO product (name, price, quantity, product_type, manufacturer_id) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, p.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, p.price);
    sqlite3_bind_int(stmt, 3, p.quantity);
    sqlite3_bind_int(stmt, 4, static_cast<int>(p.product_type));
    sqlite3_bind_int(stmt, 5, p.manufacturer_id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool ProductRepository::update(const Product& p) {
    try {
            Validator::validate(p);
        } catch (const std::invalid_argument& e) {
            std::cerr << "[Validation] " << e.what() << "\n";
            return false;
        }

    auto* db = Database::getInstance().get();
    const char* sql =
        "UPDATE product SET name=?, price=?, quantity=?, product_type=?, "
        "manufacturer_id=? WHERE id=?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, p.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, p.price);
    sqlite3_bind_int(stmt, 3, p.quantity);
    sqlite3_bind_int(stmt, 4, static_cast<int>(p.product_type));
    sqlite3_bind_int(stmt, 5, p.manufacturer_id);
    sqlite3_bind_int(stmt, 6, p.id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool ProductRepository::remove(int id) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "DELETE FROM product WHERE id=?;",
                           -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

std::optional<Product> ProductRepository::getById(int id) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db,
        "SELECT id, name, price, quantity, product_type, manufacturer_id "
        "FROM product WHERE id=?;", -1, &stmt, nullptr) != SQLITE_OK)
        return std::nullopt;

    sqlite3_bind_int(stmt, 1, id);
    std::optional<Product> result;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        result = fromStatement(stmt);

    sqlite3_finalize(stmt);
    return result;
}

std::vector<Product> ProductRepository::getAll() {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, name, price, quantity, product_type, manufacturer_id "
        "FROM product;", -1, &stmt, nullptr);

    std::vector<Product> products;
    while (sqlite3_step(stmt) == SQLITE_ROW)
        products.push_back(fromStatement(stmt));

    sqlite3_finalize(stmt);
    return products;
}

bool ProductRepository::updateQuantity(int id, int newQuantity) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "UPDATE product SET quantity=? WHERE id=?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, newQuantity);
    sqlite3_bind_int(stmt, 2, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

std::vector<Product> ProductRepository::getByManufacturer(int manufacturerId) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, name, price, quantity, product_type, manufacturer_id "
        "FROM product WHERE manufacturer_id=?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, manufacturerId);

    std::vector<Product> products;
    while (sqlite3_step(stmt) == SQLITE_ROW)
        products.push_back(fromStatement(stmt));

    sqlite3_finalize(stmt);
    return products;
}

std::vector<Product> ProductRepository::getByType(int productType) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, name, price, quantity, product_type, manufacturer_id "
        "FROM product WHERE product_type=?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, productType);

    std::vector<Product> products;
    while (sqlite3_step(stmt) == SQLITE_ROW)
        products.push_back(fromStatement(stmt));

    sqlite3_finalize(stmt);
    return products;
}

int ProductRepository::totalCount() {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT SUM(quantity) FROM product;",
        -1, &stmt, nullptr);

    int total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        total = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return total;
}

double ProductRepository::totalValue() {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT SUM(price * quantity) FROM product;",
        -1, &stmt, nullptr);

    double total = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        total = sqlite3_column_double(stmt, 0);

    sqlite3_finalize(stmt);
    return total;
}

std::vector<ProductWithManufacturer> ProductRepository::getAllWithManufacturer() {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,
        "SELECT p.id, p.name, p.price, p.quantity, p.product_type, "
        "       COALESCE(m.name, 'Невідомо') "
        "FROM product p "
        "LEFT JOIN manufacturer m ON p.manufacturer_id = m.id;",
        -1, &stmt, nullptr);

    std::vector<ProductWithManufacturer> list;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ProductWithManufacturer pw;
        pw.id               = sqlite3_column_int(stmt, 0);
        pw.productName      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        pw.price            = sqlite3_column_double(stmt, 2);
        pw.quantity         = sqlite3_column_int(stmt, 3);
        pw.productType      = sqlite3_column_int(stmt, 4);
        pw.manufacturerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        list.push_back(pw);
    }

    sqlite3_finalize(stmt);
    return list;
}
