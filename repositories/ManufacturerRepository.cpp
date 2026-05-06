#include "ManufacturerRepository.h"

Manufacturer ManufacturerRepository::fromStatement(sqlite3_stmt* stmt) {
    Manufacturer m;
    m.id          = sqlite3_column_int(stmt, 0);
    m.name        = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    auto desc     = sqlite3_column_text(stmt, 2);
    m.description = desc ? reinterpret_cast<const char*>(desc) : "";
    return m;
}

bool ManufacturerRepository::add(const Manufacturer& m) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db,
        "INSERT INTO manufacturer (name, description) VALUES (?, ?);",
        -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, m.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, m.description.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool ManufacturerRepository::update(const Manufacturer& m) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db,
        "UPDATE manufacturer SET name=?, description=? WHERE id=?;",
        -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, m.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, m.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, m.id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool ManufacturerRepository::remove(int id) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "DELETE FROM manufacturer WHERE id=?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

std::optional<Manufacturer> ManufacturerRepository::getById(int id) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, name, description FROM manufacturer WHERE id=?;",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    std::optional<Manufacturer> result;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        result = fromStatement(stmt);

    sqlite3_finalize(stmt);
    return result;
}

std::optional<Manufacturer> ManufacturerRepository::getByName(const std::string& name) {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, name, description FROM manufacturer WHERE name=?;",
        -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    std::optional<Manufacturer> result;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        result = fromStatement(stmt);

    sqlite3_finalize(stmt);
    return result;
}

std::vector<Manufacturer> ManufacturerRepository::getAll() {
    auto* db = Database::getInstance().get();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, name, description FROM manufacturer;",
        -1, &stmt, nullptr);

    std::vector<Manufacturer> list;
    while (sqlite3_step(stmt) == SQLITE_ROW)
        list.push_back(fromStatement(stmt));

    sqlite3_finalize(stmt);
    return list;
}
