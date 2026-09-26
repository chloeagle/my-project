#ifndef COMMON_H
#define COMMON_H

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

// --- 数据结构 ---
struct Product {
    string name;
    double price;
};

struct SaleRecord {
    int day;
    int transId;
    string timeStr;
    string itemsStr;
    double total;
};

// --- 全局变量声明 (extern) ---
extern map<string, Product> catalog;       // 商品目录
extern vector<SaleRecord> salesHistory;    // 历史销售记录
extern int currentDay;                     // 当前天数
extern bool isAdminMode;                   // 是否处于管理员模式
extern string adminPassword;               // 管理员密码

extern const string SALES_FILE;
extern const string PRODUCTS_FILE;

// --- 通用工具函数声明 ---
string getCurrentTimeStr();
void loadCatalog(const string& filename);
void saveCatalog();
void loadSalesHistory(); // 启动时读取历史
void saveSalesRecord(const SaleRecord& record); // 结账时写入历史

#endif