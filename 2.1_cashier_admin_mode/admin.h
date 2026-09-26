#ifndef ADMIN_H
#define ADMIN_H

#include "common.h"

// 管理员功能函数声明
void handleAdminCommand(const string& input);
void adminSetPrice(const string& id, double newPrice);
void adminItemAdd(const string& id, const string& name, double price);
void adminItemDel(const string& id);
void adminViewPrices();

#endif