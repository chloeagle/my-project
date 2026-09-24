#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Product {
    string barcode;  
    string name;     
    double price;   
};

vector<Product> loadProductsFromCSV(const string& filename) {
    vector<Product> products;
    ifstream file(filename);
    
    string line;
    getline(file, line); 
    
    while (getline(file, line)) {
        stringstream ss(line);
        Product p;
        
        
        getline(ss, p.barcode, ',');
        getline(ss, p.name, ',');
        
        string priceStr;
        getline(ss, priceStr, ',');
        p.price = stod(priceStr); 
        
        products.push_back(p);
    }
    
    file.close();
    return products;
}

bool findProductByBarcode(const vector<Product>& products, const string& barcode, Product& result) {
    for (const auto& p : products) {
        if (p.barcode == barcode) {
            result = p;
            return true;
        }
    }
    return false;
}

void displayProduct(const Product& p) {
    cout << left << setw(15) << p.name 
         << fixed << setprecision(2) << p.price << endl;
}

void displayAllProducts(const vector<Product>& products) {
    cout << left << setw(15) << "Item" 
         << setw(8) << "No." 
         << "Pri." << endl;
    cout << "--------------------------" << endl;
    
    for (const auto& p : products) {
        cout << left << setw(15) << p.name 
             << setw(8) << p.barcode 
             << " " << fixed << setprecision(2) << p.price << endl;
    }
}

int main() {
    string filename = "products.csv";
    vector<Product> products = loadProductsFromCSV(filename);

    cout << "      Convenience Store POS System" << endl;
    cout << "========================================" << endl;
    cout << "Available commands:" << endl;
    cout << "  <barcode>            Query a single product, e.g. 001" << endl;
    cout << "  <code1> <code2> ..   Query multiple products, e.g. 001 003" << endl;
    cout << "  prices               List all products" << endl;
    cout << "  exit                 Exit the program" << endl;
    cout << "========================================" << endl;
    
    string input;
    while (true) {
        cout << "> ";
        if (!getline(cin, input)) break;
        
        if (input == "exit") {
            break;
        }
        
        if (input == "prices") {
            displayAllProducts(products);
            continue;
        }
        
        stringstream ss(input);
        string barcode;
        
        while (ss >> barcode) {
            Product p;
            if (findProductByBarcode(products, barcode, p)) {
                displayProduct(p);
            } else {
                cout << "ERROR: code not found" << endl;
            }
        }
    }
    
    return 0;
}