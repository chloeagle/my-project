#include "admin.h"

// 修改价格
void adminSetPrice(const string& id, double newPrice) {
    if (catalog.count(id)) {
        catalog[id].price = newPrice;
        saveCatalog(); // 持久化：写回文件
        cout << "Price updated." << endl;
    } else {
        cout << "Error: Item " << id << " not found." << endl;
    }
}

// 添加新商品
void adminItemAdd(const string& id, const string& name, double price) {
    if (catalog.count(id)) {
        cout << "Error: Item " << id << " already exists." << endl;
    } else {
        catalog[id] = {name, price};
        saveCatalog(); // 持久化：写回文件
        cout << name << "(" << id << ") added." << endl;
    }
}

// 删除商品
void adminItemDel(const string& id) {
    if (catalog.count(id)) {
        string name = catalog[id].name;
        catalog.erase(id); 
        saveCatalog(); // 持久化：写回文件
        cout << name << "(" << id << ") removed." << endl;
    } else {
        cout << "Error: Item " << id << " not found." << endl;
    }
}

// 查看所有商品
void adminViewPrices() {
    cout << left << setw(10) << "Item" 
         << setw(6) << "No." 
         << "Pri." << endl;
    cout << "--------------------" << endl;
    for (const auto& item : catalog) {
        cout << left << setw(10) << item.second.name 
             << setw(6) << item.first 
             << fixed << setprecision(2) << item.second.price << endl;
    }
}

// 解析并执行管理员命令
void handleAdminCommand(const string& input) {
    stringstream ss(input);
    string cmd;
    ss >> cmd;

    if (cmd == "setprice") {
        string id; double price;
        if (ss >> id >> price) adminSetPrice(id, price);
        else cout << "Usage: setprice <barcode> <new_price>" << endl;
    }
    else if (cmd == "itemadd") {
        string id, name; double price;
        if (ss >> id >> name >> price) adminItemAdd(id, name, price);
        else cout << "Usage: itemadd <barcode> <name> <price>" << endl;
    }
    else if (cmd == "itemdel") {
        string id;
        if (ss >> id) adminItemDel(id);
        else cout << "Usage: itemdel <barcode>" << endl;
    }
    else if (cmd == "prices") {
        adminViewPrices();
    }
    else if (cmd == "back") {
        isAdminMode = false; // 切换回收银员模式
        cout << "Bye." << endl;
    }
    else {
        cout << "Unknown admin command." << endl;
    }
}