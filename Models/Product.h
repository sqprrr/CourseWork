#pragma once
#include "Models/ProductType.h"
#include <string>

struct Product {
    int id = 0;
    std::string name;
    double price = 0.0;
    int quantity = 0;
    ProductType product_type = ProductType::Other;
    int manufacturer_id = 0;
};
