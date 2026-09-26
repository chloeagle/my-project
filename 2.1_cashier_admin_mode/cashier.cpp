#include "cashier.h"

// 打印购物车 
double printCart(const map<string, int>& cart, bool isReceipt) {
    if (isReceipt) {
        cout << "Receipt" << endl;
        cout << left << setw(10) << "Item" << right << setw(6) << "Pri." 
             << setw(4) << "Qty" << setw(8) << "Amount" << endl;
        cout << "------------------------" << endl;
    }
    double total = 0;
    for (const auto& item : cart) {
        string id = item.first;
        int qty = item.second;
        if (catalog.find(id) == catalog.end()) continue;
        double price = catalog[id].price;
        string name = catalog[id].name;
        double amount = price * qty;
        total += amount;
        cout << left << setw(10) << name << right << fixed << setprecision(2)
             << setw(6) << price << " x" << qty << " =" << setw(6) << amount << endl;
    }
    if (isReceipt) {
        cout << "------------------------" << endl;
        cout << left << setw(16) << "Total" << " =" << setw(6) << total << endl;
    }
    return total;
}

// 结账逻辑
void processCheckout(map<string, int>& cart) {
    if (cart.empty()) { cout << "Cart is empty." << endl; return; }
    double total = printCart(cart, true);
    
    string itemsStr;
    for (const auto& item : cart) {
        if (catalog.count(item.first)) 
            itemsStr += catalog[item.first].name + " x" + to_string(item.second) + " ";
    }
    
    int transId = 1;
    for (const auto& rec : salesHistory) if (rec.day == currentDay) transId++;

    SaleRecord record = {currentDay, transId, getCurrentTimeStr(), itemsStr, total};
    salesHistory.push_back(record);
    saveSalesRecord(record); // 调用 common.cpp 里的保存函数
    
    cout << "Sales record saved." << endl;
    cart.clear();
}

// 查看销售记录
void processShowSales(int day) {
    cout << "Date: " << day << endl;
    cout << left << setw(4) << "No." << setw(10) << "Time" << setw(22) << "Items" << "Total" << endl;
    cout << "----------------------------------------" << endl;
    double dailyTotal = 0;
    for (const auto& rec : salesHistory) {
        if (rec.day == day) {
            dailyTotal += rec.total;
            cout << left << setw(4) << rec.transId << setw(10) << rec.timeStr 
                 << rec.itemsStr << endl;
            cout << setw(14) << " " << right << setw(8) << fixed << setprecision(2) << rec.total << endl;
        }
    }
    cout << "----------------------------------------" << endl;
    cout << "Daily: " << fixed << setprecision(2) << dailyTotal << endl;
}

// 收银员核心路由函数：根据用户输入调用不同的子功能
void handleCashierCommand(const string& input, map<string, int>& cart) {
    if (input == "checkout") {
        processCheckout(cart);
    } 
    else if (input == "drop") {
        cart.clear();
    } 
    else if (input == "print") {
        printCart(cart, false);
    } 
    else if (input == "prices") {
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
    else if (input == "newday") {
        currentDay++;
        cout << "New day started. Today's sales records cleared." << endl;
    }
    else if (input.substr(0, 5) == "sales") {
        stringstream ss(input);
        string cmd, dayStr;
        ss >> cmd;
        if (ss >> dayStr) processShowSales(stoi(dayStr));
        else processShowSales(currentDay);
    }
    else {
        // 处理商品条码 (加购/退购)
        stringstream ss(input);
        string token;
        while (ss >> token) {
            if (token[0] == '-') {
                string id = token.substr(1);
                if (cart.count(id)) {
                    cart[id]--;
                    if (cart[id] <= 0) cart.erase(id);
                } else {
                    cout << "Error: Item " << id << " not in cart!" << endl;
                }
            } else {
                if (catalog.count(token)) cart[token]++;
                else cout << "Error: Item " << token << " not found!" << endl;
            }
        }
        printCart(cart, false);
    }
}