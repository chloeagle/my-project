#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;

struct Product {
    string name;
    double price;
};
//销售记录结构体*
struct SaleRecord {
    int day;            
    int transId;        
    string timeStr;    
    string itemsStr;    
    double total;       
};

map<string, Product> catalog;      
vector<SaleRecord> salesHistory;    
int currentDay = 1;                 
const string SALES_FILE = "sales.csv"; 

//记录时间*
string getCurrentTimeStr() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm *parts = localtime(&now_c);
    stringstream ss;
    ss << put_time(parts, "%H:%M:%S");
    return ss.str();
}

// 读取CSV文件（同1.2）
void loadCatalog(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Warning: Could not open " << filename << endl;
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string id, name, priceStr;
        if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, priceStr, ',')) {
            catalog[id] = {name, stod(priceStr)};
        }
    }
}

// 从文件读取销售历史记录*
void loadSalesHistory() {
    ifstream file(SALES_FILE);
    if (!file.is_open()) return; // 如果没有文件，说明是第一次运行

    string line;
    getline(file, line);
    while (getline(file, line)){
        if (line.empty()) continue;

        // CSV 格式: Day,TransId,Time,Items,Total
        stringstream ss(line);
        string dayStr, idStr, timeStr, itemsStr, totalStr;
        
        if (getline(ss, dayStr, ',') && getline(ss, idStr, ',') && 
            getline(ss, timeStr, ',') && getline(ss, itemsStr, ',') && 
            getline(ss, totalStr, ',')) {
            
            SaleRecord record;
            record.day = stoi(dayStr);
            record.transId = stoi(idStr);
            record.timeStr = timeStr;
            record.itemsStr = itemsStr;
            record.total = stod(totalStr);
            
            salesHistory.push_back(record);
            
            // 更新当前最大天数
            if (record.day > currentDay) {
                currentDay = record.day;
            }
        }
    }
    file.close();
}

// 打印购物车（同1.2）
double printCart(const map<string, int>& cart, bool isReceipt) {
    if (isReceipt) {
        cout << "Receipt" << endl;
        cout << left << setw(10) << "Item" 
             << right << setw(6) << "Pri." 
             << setw(4) << "Qty" 
             << setw(8) << "Amount" << endl;
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

        cout << left << setw(10) << name 
             << right << fixed << setprecision(2)
             << setw(6) << price 
             << " x" << qty 
             << " =" << setw(6) << amount << endl;
    }

    if (isReceipt) {
        cout << "------------------------" << endl;
        cout << left << setw(16) << "Total" 
             << " =" << setw(6) << total << endl;
    }
    return total;
}

// 生成商品明细字符串 (例如: "Cola x1 Lollipop x1 ")
string generateItemsString(const map<string, int>& cart) {
    string result;
    for (const auto& item : cart) {
        string id = item .first;
        if (catalog.find(id) == catalog.end()) continue;
        result += catalog[id].name + " x" + to_string(item.second) + " ";
    }
    return result;
}

// 结账并保存到文件
void checkout(map<string, int>& cart) {
    if (cart.empty()) {
        cout << "Cart is empty. Nothing to checkout." << endl;
        return;
    }

    double total = printCart(cart, true); //checkout printCart
    string itemsStr = generateItemsString(cart); // 获取商品明细
    string timeStr = getCurrentTimeStr(); // 获取当前时间

    // 计算流水号 (当天已有记录数 + 1)
    int transId = 1;
    for (const auto& rec : salesHistory) {
        if (rec.day == currentDay) transId++;
    }

    // 1. 存入内存
    SaleRecord record;
    record.day = currentDay;
    record.transId = transId;
    record.timeStr = timeStr;
    record.itemsStr = itemsStr;
    record.total = total;
    salesHistory.push_back(record);

    // 2. 持久化写入文件 (追加模式)
    ofstream file(SALES_FILE, ios::app);
    if (file.is_open()) {
        // 检查文件是否为空，为空则写表头
        file.seekp(0, ios::end);
        if (file.tellp() == 0) {
            file << "Day,No,Time,Items,Total" << endl;
        }
        // 写入数据行
        file << record.day << "," << record.transId << "," 
             << record.timeStr << "," << record.itemsStr << "," 
             << fixed << setprecision(2) << record.total << endl;
        file.close();
        cout << "Sales record saved to " << SALES_FILE << endl;
    } else {
        cerr << "Error: Unable to save sales record!" << endl;
    }

    cart.clear(); // 清空购物车
}

// 查看指定日期的销售记录
void showSales(int day) {
    cout << "Date: " << day << endl;
    cout << left << setw(4) << "No." 
         << setw(10) << "Time" 
         << setw(22) << "Items" 
         << "Total" << endl;
    cout << "----------------------------------------" << endl;

    double dailyTotal = 0;
    bool found = false;

    for (const auto& rec : salesHistory) {
        if (rec.day != day) continue;
        found = true;
        dailyTotal += rec.total;

        cout << left << setw(4) << rec.transId 
             << setw(10) << rec.timeStr 
             << rec.itemsStr << endl;
        
        cout << setw(14) << "" << " " << setw(22) << "" 
             << " " << right << setw(8) << fixed << setprecision(2) << rec.total << endl;
    }

    if (!found) {
        cout << "No sales records for this day." << endl;
    }
    cout << "----------------------------------------" << endl;
    cout << "Daily: " << fixed << setprecision(2) << dailyTotal << endl;
}

// 开启新的一天
void newDay() {
    currentDay++;
    cout << "New day started. Today's sales records cleared." << endl;
}

// 欢迎菜单
void welcome() {
    cout << "========================================" << endl;
    cout << "        Welcome to POS System" << endl;
    cout << "========================================" << endl;
    cout << " Enter barcode (e.g. 001)   -> Add item" << endl;
    cout << " Enter -barcode (e.g. -001) -> Remove item" << endl;
    cout << " print                      -> View cart" << endl;
    cout << " drop                       -> Clear cart" << endl;
    cout << " checkout                   -> Checkout & save" << endl;
    cout << " sales [day]                -> View sales" << endl;
    cout << " newday                     -> Start new day" << endl;
    cout << " exit                       -> Exit program" << endl;
    cout << "========================================" << endl;
    cout << "Note: Multiple barcodes separated by space (e.g. 001 002)" << endl << endl;
}

int main() {
    welcome();
    loadCatalog("products.csv");
    loadSalesHistory(); // 启动时加载历史记录

    map<string, int> cart; 
    string input;

    while (true) {
        cout << "\n> ";
        if (!getline(cin, input)) break;
        if (input.empty()) continue;

        if (input == "checkout") {
            checkout(cart);
        } 
        else if (input == "drop") {
            cart.clear();
        } 
        else if (input == "print") {
            printCart(cart, false);
        } 
        else if (input == "newday") {
            newDay();
        }
        else if (input == "exit") {
            cout << "Exiting system...Goodbye!" << endl;
            break;
        }
        else if (input.substr(0, 5) == "sales") {
            // 处理 "sales" 或 "sales 1"
            stringstream ss(input);
            string cmd, dayStr;
            ss >> cmd;
            
            if (ss >> dayStr) {
                // 有参数，例如 "sales 1"
                try {
                    showSales(stoi(dayStr));
                } catch (...) {
                    cout << "Invalid day format." << endl;
                }
            } else {
                // 无参数，默认显示今天的记录 (currentDay)
                showSales(currentDay);
            }
        }
        else {
            // 处理商品条码 (支持空格分隔的批量输入)（同1.2）
            stringstream ss(input);
            string token;
            
            while (ss >> token) {
                if (token[0] == '-') {
                    // 减购
                    string id = token.substr(1);
                    if (cart.count(id)) {
                        cart[id]--;
                        if (cart[id] <= 0) cart.erase(id);
                    }
                } else {
                    // 加购
                    if (catalog.count(token)) {
                        cart[token]++;
                    }
                    else {
                        cout << "ERROR: Item not found!" << endl;
                    }
                }
            }
        }
    }

    return 0;
}