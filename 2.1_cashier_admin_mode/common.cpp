#include "common.h"

// 获取当前时间 (HH:MM:SS)
string getCurrentTimeStr() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm *parts = localtime(&now_c);
    stringstream ss;
    ss << put_time(parts, "%H:%M:%S");
    return ss.str();
}

// 从 CSV 加载商品目录
void loadCatalog(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    string line;
    bool isFirstLine = true;
    while (getline(file, line)) {
        if (isFirstLine) { isFirstLine = false; continue; }
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, priceStr;
        if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, priceStr, ',')) {
            catalog[id] = {name, stod(priceStr)};
        }
    }
}

// 将内存中的商品目录保存回 CSV 文件
void saveCatalog() {
    ofstream file(PRODUCTS_FILE);
    if (!file.is_open()) { cerr << "Error: Unable to save catalog." << endl; return; }
    file << "barcode,name,Price" << endl;
    for (const auto& item : catalog) {
        file << item.first << "," << item.second.name << "," 
             << fixed << setprecision(2) << item.second.price << endl;
    }
}

// 从文件加载销售历史
void loadSalesHistory() {
    ifstream file(SALES_FILE);
    if (!file.is_open()) return;
    string line;
    bool isFirstLine = true;
    while (getline(file, line)) {
        if (isFirstLine) { isFirstLine = false; continue; }
        if (line.empty()) continue;
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
            if (record.day > currentDay) currentDay = record.day;
        }
    }
}

// 新增一条销售记录到文件
void saveSalesRecord(const SaleRecord& record) {
    ofstream file(SALES_FILE, ios::app);
    if (file.is_open()) {
        file.seekp(0, ios::end);
        if (file.tellp() == 0) file << "Day,No,Time,Items,Total" << endl;
        file << record.day << "," << record.transId << "," 
             << record.timeStr << "," << record.itemsStr << "," 
             << fixed << setprecision(2) << record.total << endl;
    }
}