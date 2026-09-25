#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

struct Product {
    string name;
    double price;
};

map<string, Product> catalog;

void loadCatalog(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: could not open" << filename << endl;
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, priceStr;
        if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, priceStr, ',')) {
            catalog[id] = {name, stod(priceStr)};
        }
    }
    file.close();
}

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


void welcome() {
    cout << "========================================" << endl;
    cout << "        Welcome to POS System" << endl;
    cout << "========================================" << endl;
    cout << " Enter barcode (e.g. 001)   -> Add item" << endl;
    cout << " Enter -barcode (e.g. -001) -> Remove item" << endl;
    cout << " print                      -> View cart" << endl;
    cout << " drop                       -> Clear cart" << endl;
    cout << " checkout                   -> Checkout & clear" << endl;
    cout << " exit                       -> Exit program" << endl;
    cout << "========================================" << endl;
    cout << "Note: Multiple barcodes separated by space (e.g. 001 002)" << endl << endl;
}
int main() {
    welcome();
    loadCatalog("products.csv");

    map<string, int> cart;
    string input;

    while (true) {
        cout << "> ";
        getline(cin, input);
        
        if (input.empty()) continue;

        if (input == "checkout") {
            printCart(cart, true);
            cart.clear();
        }
        
        else if (input == "drop") {
            cart.clear();
        }
        
        else if (input == "print") {
            printCart(cart, false);
        }
        
        else if (input == "exit") {
            break;
        }
        else {
            stringstream ss(input);
            string token;
            while (ss >> token) {
            if (token[0] == '-') {
                string id = token.substr(1);
                if (cart.count(id)) {
                    cart[id]--;
                }
                    if (cart[id] <= 0) {
                        cart.erase(id);
                    }
            }
            else {
                if (catalog.count(token)) {
                    cart[token]++;
                }
            }
            }
        }
    }

    return 0;
}