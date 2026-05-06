#pragma once
#include <string>
#include <map>

enum class ProductType {
    Electronics   = 1,
    Food          = 2,
    Clothing      = 3,
    Furniture     = 4,
    Other         = 5
};

inline std::map<int, std::string> productTypeNames() {
    return {
        {1, "Електроніка"},
        {2, "Продукти харчування"},
        {3, "Одяг"},
        {4, "Меблі"},
        {5, "Інше"}
    };
}

inline std::string getTypeName(ProductType type) {
    auto names = productTypeNames();
    auto it = names.find(static_cast<int>(type));
    if (it != names.end())
        return it->second;
    return "Невідомо(" + std::to_string(static_cast<int>(type)) + ")";
}
