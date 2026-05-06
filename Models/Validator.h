#pragma once
#include "Product.h"
#include <stdexcept>

class Validator {
public:
    static void validate(const Product& p) {
        if (p.price < 0)
            throw std::invalid_argument(
                "Ціна товару не може бути від'ємною: " + p.name);
        if (p.quantity < 0)
            throw std::invalid_argument(
                "Кількість товару не може бути від'ємною: " + p.name);
        if (p.name.empty())
            throw std::invalid_argument("Назва товару не може бути порожньою");
    }
};
