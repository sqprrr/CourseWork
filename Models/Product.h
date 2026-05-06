#pragma once
#include <string>

struct Product {
    int id = 0;
    std::string name;
    double price = 0.0;
    int quantity = 0;
    int product_type = 0;
    int manufacturer_id = 0;
};
