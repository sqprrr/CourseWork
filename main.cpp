// main.cpp
#include <iostream>
#include "db/Database.h"
#include "repositories/ManufacturerRepository.h"
#include "repositories/ProductRepository.h"
#include "handlers/ManufacturerHandler.h"
#include "handlers/ProductHandler.h"

void printMenu() {
    std::cout << "\n=== Склад ===\n"
              << "1. Виробники\n"
              << "2. Товари\n"
              << "0. Вихід\n"
              << "Вибір: ";
}

void printSubMenu(const std::string& section) {
    std::cout << "\n--- " << section << " ---\n"
              << "1. Список\n"
              << "2. Додати\n"
              << "3. Редагувати\n"
              << "4. Видалити\n"
              << "0. Назад\n"
              << "Вибір: ";
}

int main() {
    // Ініціалізація БД і міграція
    Database::getInstance("warehouse.db")
             .runMigrations("migrations/01_init.sql");

    ManufacturerRepository manufacturerRepo;
    ProductRepository productRepo;

    ManufacturerHandler manufacturerHandler(manufacturerRepo);
    ProductHandler productHandler(productRepo, manufacturerRepo);

    int choice;
    while (true) {
        printMenu();
        std::cin >> choice;

        if (choice == 0) break;

        int sub;
        switch (choice) {
            case 1:
                printSubMenu("Виробники");
                std::cin >> sub;
                if (sub == 1) manufacturerHandler.handleList();
                else if (sub == 2) manufacturerHandler.handleAdd();
                else if (sub == 3) manufacturerHandler.handleUpdate();
                else if (sub == 4) manufacturerHandler.handleDelete();
                break;

            case 2:
                printSubMenu("Товари");
                std::cin >> sub;
                if (sub == 1) productHandler.handleGet();
                else if (sub == 2) productHandler.handleAdd();
                else if (sub == 3) productHandler.handleUpdate();
                else if (sub == 4) productHandler.handleDelete();
                break;

            default:
                std::cout << "Невірний вибір.\n";
        }
    }

    std::cout << "До побачення!\n";
    return 0;
}
