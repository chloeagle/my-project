#ifndef CASHIER_H
#define CASHIER_H

#include "common.h"

// 收银员模式主入口 (处理一行输入)
void handleCashierCommand(const string& input, map<string, int>& cart);

#endif