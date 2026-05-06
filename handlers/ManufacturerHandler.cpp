#include "ManufacturerHandler.h"
#include <iostream>

void ManufacturerHandler::handleAdd() {
    Manufacturer m;
    std::cout << "Назва виробника: ";
    std::cin >> m.name;
    std::cout << "Опис: ";
    std::cin >> m.description;

    if (repo_.add(m))
        std::cout << "[OK] Виробника додано.\n";
    else
        std::cout << "[ERR] Помилка при додаванні.\n";
}

void ManufacturerHandler::handleList() {
    auto list = repo_.getAll();
    if (list.empty()) { std::cout << "Список порожній.\n"; return; }

    std::cout << "\n--- Виробники ---\n";
    for (const auto& m : list)
        std::cout << "[" << m.id << "] " << m.name
                  << " — " << m.description << "\n";
}

void ManufacturerHandler::handleDelete() {
    handleList();

    int id;
    std::cout << "ID виробника для видалення: ";
    std::cin >> id;

    if (repo_.remove(id))
        std::cout << "[OK] Видалено.\n";
    else
        std::cout << "[ERR] Не знайдено.\n";
}

void ManufacturerHandler::handleUpdate() {
    handleList();

    int id;
    std::cout << "ID виробника для редагування: ";
    std::cin >> id;

    auto existing = repo_.getById(id);
    if (!existing) { std::cout << "[ERR] Не знайдено.\n"; return; }

    Manufacturer m = *existing;
    std::cout << "Нова назва [" << m.name << "]: ";
    std::cin >> m.name;
    std::cout << "Новий опис [" << m.description << "]: ";
    std::cin >> m.description;

    if (repo_.update(m))
        std::cout << "[OK] Оновлено.\n";
    else
        std::cout << "[ERR] Помилка оновлення.\n";
}
