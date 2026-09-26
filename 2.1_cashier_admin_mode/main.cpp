#include "common.h"
#include "cashier.h"
#include "admin.h"

map<string, Product> catalog;
vector<SaleRecord> salesHistory;
int currentDay = 1;
bool isAdminMode = false;
string adminPassword = "admin123";
const string SALES_FILE = "sales.csv";
const string PRODUCTS_FILE = "products.csv";

int main() {
    //1.欢迎菜单
    cout << "========================================" << endl;
    cout << "        Welcome to POS System" << endl;
    cout << "========================================" << endl;
    cout << " Cashier Mode Commands:" << endl;
    cout << "  001 / -001               -> Add / Remove item" << endl;
    cout << "  print                    -> View cart" << endl;
    cout << "  drop                     -> Clear cart" << endl;
    cout << "  checkout                 -> Checkout & save" << endl;
    cout << "  sales [day]              -> View sales records" << endl;
    cout << "  newday                   -> Start a new day" << endl;
    cout << "  admin                    -> Enter Admin Mode" << endl;
    cout << "  exit                     -> Exit program" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Admin Mode Commands:" << endl;
    cout << "  setprice <id> <price>    -> Update price" << endl;
    cout << "  itemadd <id> <name> <p>  -> Add new item" << endl;
    cout << "  itemdel <id>             -> Delete item" << endl;
    cout << "  prices                   -> View all items" << endl;
    cout << "  back                     -> Return to Cashier Mode" << endl;
    cout << "========================================" << endl;
    cout << "Note: Multiple barcodes separated by space (e.g. 001 002)" << endl << endl;
  
    // 2. 加载数据
    loadCatalog(PRODUCTS_FILE);
    loadSalesHistory();

    map<string, int> cart; // 收银员购物车 (只在 main 中维护)
    string input;

    
    // 3. 主循环
    while (true) {
        // 根据模式切换提示符
        if (isAdminMode) cout << "admin> ";
        else cout << "> ";
       
        if (!getline(cin, input)) break;
        if (input.empty()) continue;

        if (input =="exit") {
            cout << "Exiting system... Goodbye!" << endl;
            exit(0);
        }

        if (isAdminMode) {
            // 交给管理员模块处理
            handleAdminCommand(input);
        } 
        else {
            // 收银员模式：先检查是不是想进管理员模式
            if (input == "admin") {
                cout << "Password: ";
                string pwd;
                getline(cin, pwd);
                if (pwd == adminPassword) {
                    isAdminMode = true;
                    cout << "Admin mode." << endl;
                } else {
                    cout << "Wrong password." << endl;
                }
            } 
            else {
                // 其他所有命令，交给收银员模块处理！
                handleCashierCommand(input, cart);
            }
        }
    }
    return 0;
}