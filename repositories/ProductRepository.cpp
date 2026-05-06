#include "ProductRepository.h"

Product ProductRepository::fromStatement(sqlite3_stmt* stmt) {
    Product p;
    p.id            = sqlite3_column_int(stmt, 0);
    p.name          = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    p.price         = sqlite3_column_double(stmt, 2);
    p.quantity      = sqlite3_column_int(stmt, 3);
    p.product_type  = sqlite3_column_int(stmt, 4);
    p.manufacturer_id = sqlite3_column_int(stmt, 5);
    return p;
}

bool ProductRepository::add(const Product& p) {
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
    sqlite3_bind_int(stmt, 4, p.product_type);
    sqlite3_bind_int(stmt, 5, p.manufacturer_id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool ProductRepository::update(const Product& p) {
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
    sqlite3_bind_int(stmt, 4, p.product_type);
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
