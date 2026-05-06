CREATE TABLE IF NOT EXISTS manufacturer (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    description TEXT
);

CREATE TABLE IF NOT EXISTS product (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    price REAL NOT NULL,
    quantity INTEGER NOT NULL DEFAULT 0,
    product_type INTEGER NOT NULL,
    manufacturer_id INTEGER,

    FOREIGN KEY (manufacturer_id) REFERENCES manufacturer(id) ON DELETE SET NULL
);
