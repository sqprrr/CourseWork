#include "ProductHandler.h"
#include <iostream>
#include <iomanip>
#include <string>

int ProductHandler::pickManufacturer() {
    auto list = manufacturerRepo_.getAll();
    if (list.empty()) {
        std::cout << "[!] Спочатку додайте виробника.\n";
        return -1;
    }

    std::cout << "\nВиробники:\n";
    for (const auto& m : list)
        std::cout << "  [" << m.id << "] " << m.name << "\n";

    int id;
    std::cout << "Оберіть ID виробника: ";
    std::cin >> id;
    return id;
}

void ProductHandler::handleList() {
    auto products = productRepo_.getAll();
    if (products.empty()) {
        std::cout << "Список товарів порожній.\n";
        return;
    }

    std::cout << "\n--- Товари ---\n";
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(25) << "Назва"
              << std::setw(12) << "Ціна"
              << std::setw(10) << "Кількість"
              << std::setw(8)  << "Тип"
              << "Виробник ID\n";
    std::cout << std::string(65, '-') << "\n";

    for (const auto& p : products) {
        std::cout << std::left
                  << std::setw(5)  << p.id
                  << std::setw(25) << p.name
                  << std::setw(12) << std::fixed << std::setprecision(2) << p.price
                  << std::setw(10) << p.quantity
                  << std::setw(8)  << p.product_type
                  << p.manufacturer_id << "\n";
    }
}

void ProductHandler::handleGet() {
    int choice;
    std::cout << "\n1 - Всі товари\n"
              << "2 - За виробником\n"
              << "3 - За ID\n"
              << "Вибір: ";
    std::cin >> choice;

    if (choice == 1) {
        handleList();

    } else if (choice == 2) {
        int manufacturerId;
        std::cout << "ID виробника: ";
        std::cin >> manufacturerId;

        auto products = productRepo_.getByManufacturer(manufacturerId);
        if (products.empty()) {
            std::cout << "Товарів не знайдено.\n";
            return;
        }
        for (const auto& p : products)
            std::cout << "[" << p.id << "] " << p.name
                      << " — " << p.price << " грн"
                      << " | кількість: " << p.quantity << "\n";

    } else if (choice == 3) {
        int id;
        std::cout << "ID товару: ";
        std::cin >> id;

        auto product = productRepo_.getById(id);
        if (!product) {
            std::cout << "[ERR] Товар не знайдено.\n";
            return;
        }
        std::cout << "[" << product->id << "] " << product->name
                  << " — " << product->price << " грн"
                  << " | кількість: " << product->quantity << "\n";

    } else {
        std::cout << "Невірний вибір.\n";
    }
}

void ProductHandler::handleAdd() {
    Product p;

    p.manufacturer_id = pickManufacturer();
    if (p.manufacturer_id == -1) return;

    std::cout << "Назва товару: ";
    std::cin.ignore();
    std::getline(std::cin, p.name);

    std::cout << "Ціна: ";
    std::cin >> p.price;

    std::cout << "Кількість: ";
    std::cin >> p.quantity;

    std::cout << "Тип товару (число): ";
    std::cin >> p.product_type;

    if (productRepo_.add(p))
        std::cout << "[OK] Товар додано.\n";
    else
        std::cout << "[ERR] Помилка при додаванні.\n";
}

void ProductHandler::handleUpdate() {
    handleList();

    int id;
    std::cout << "ID товару для редагування: ";
    std::cin >> id;

    auto existing = productRepo_.getById(id);
    if (!existing) {
        std::cout << "[ERR] Товар не знайдено.\n";
        return;
    }

    Product p = *existing;
    std::cout << "Нова назва [" << p.name << "]: ";
    std::cin.ignore();
    std::getline(std::cin, p.name);

    std::cout << "Нова ціна [" << p.price << "]: ";
    std::cin >> p.price;

    std::cout << "Нова кількість [" << p.quantity << "]: ";
    std::cin >> p.quantity;

    std::cout << "Новий тип [" << p.product_type << "]: ";
    std::cin >> p.product_type;

    std::cout << "Змінити виробника? (1 - так / 0 - ні): ";
    int change;
    std::cin >> change;
    if (change == 1)
        p.manufacturer_id = pickManufacturer();

    if (productRepo_.update(p))
        std::cout << "[OK] Товар оновлено.\n";
    else
        std::cout << "[ERR] Помилка оновлення.\n";
}

void ProductHandler::handleDelete() {
    handleList();

    int id;
    std::cout << "ID товару для видалення: ";
    std::cin >> id;

    auto existing = productRepo_.getById(id);
    if (!existing) {
        std::cout << "[ERR] Товар не знайдено.\n";
        return;
    }

    std::cout << "Видалити \"" << existing->name << "\"? (1 - так / 0 - ні): ";
    int confirm;
    std::cin >> confirm;
    if (confirm != 1) { std::cout << "Скасовано.\n"; return; }

    if (productRepo_.remove(id))
        std::cout << "[OK] Видалено.\n";
    else
        std::cout << "[ERR] Помилка видалення.\n";
}
