#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <cmath>
 
using namespace std;

namespace Color {
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string WHITE   = "\033[37m";
}
 
inline string colorize(const string& text, const string& code) {
    return code + text + Color::RESET;
}

inline string money(double amount) {
    ostringstream oss;
    oss << fixed << setprecision(2) << amount;
    return oss.str();
}

struct Stock {
    string symbol;
    string name;
    double price;
    double previousClose;
    double volatility; 
};
 
struct Holding {
    int shares = 0;
    double totalCost = 0.0; 
};
 
struct Transaction {
    int tick;
    string type;    
    string symbol;
    int qty;
    double price;
    double total;
};
 
struct Order {
    string type;     
    string symbol;
    int qty;
    double triggerPrice;
    bool active = true;
};
 
struct Portfolio {
    double balance = 10000.0;
    double realizedPL = 0.0;
    map<string, Holding> holdings;     
    vector<Transaction> history;
    vector<Order> orders;
};

int getValidInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.eof()) {
            cout << "\nNo more input available. Exiting.\n";
            exit(0);
        }
        if (cin.fail() || value < minVal || value > maxVal) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << colorize("Invalid input. Please enter a number between "
                 + to_string(minVal) + " and " + to_string(maxVal) + ".\n", Color::RED);
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}
 
double getValidDouble(const string& prompt, double minVal) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.eof()) {
            cout << "\nNo more input available. Exiting.\n";
            exit(0);
        }
        if (cin.fail() || value < minVal) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << colorize("Invalid input. Please enter a valid number >= "
                 + to_string(minVal) + ".\n", Color::RED);
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}
 
string getValidLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}
 
const string USER_FILE = "users.dat";
 
map<string, string> loadUsers() {
    map<string, string> users;
    ifstream in(USER_FILE);
    string user, pass;
    while (in >> user >> pass) {
        users[user] = pass;
    }
    return users;
}
 
void saveUser(const string& username, const string& password) {
    ofstream out(USER_FILE, ios::app);
    out << username << " " << password << "\n";
}
 
string loginSystem() {
    map<string, string> users = loadUsers();
 
    cout << colorize("=====================================\n", Color::CYAN);
    cout << colorize("           LOGIN / REGISTER\n", Color::CYAN + Color::BOLD);
    cout << colorize("=====================================\n", Color::CYAN);
 
    while (true) {
        cout << "\n1. Login\n2. Register\n3. Exit\n";
        int choice = getValidInt("Enter choice: ", 1, 3);
 
        if (choice == 3) {
            cout << "Goodbye!\n";
            exit(0);
        }
 
        string username = getValidLine("Username: ");
        string password = getValidLine("Password: ");
 
        if (choice == 1) { 
            auto it = users.find(username);
            if (it != users.end() && it->second == password) {
                cout << colorize("\nLogin successful! Welcome back, " + username + ".\n", Color::GREEN);
                return username;
            } else {
                cout << colorize("Invalid username or password.\n", Color::RED);
            }
        } else { // Register
            if (users.count(username)) {
                cout << colorize("That username already exists. Try logging in instead.\n", Color::RED);
            } else {
                saveUser(username, password);
                users[username] = password; // keep in-memory copy in sync with the file
                cout << colorize("Account created! You can now log in.\n", Color::GREEN);
            }
        }
    }
}
 
vector<Stock> initStocks() {
    return {
        {"TECH", "TechCorp",      150.0, 150.0, 4.0},
        {"BANK", "Bankwell Inc.",  80.0,  80.0, 2.0},
        {"ENGY", "Energen",       60.0,  60.0, 3.0},
        {"FOOD", "FreshFoods",    45.0,  45.0, 1.5},
        {"SPCE", "SpaceVenture", 200.0, 200.0, 8.0}
    };
}
 
Stock* findStock(vector<Stock>& stocks, const string& symbol) {
    for (auto& s : stocks) {
        if (s.symbol == symbol) return &s;
    }
    return nullptr;
}

void generateMarketNews(vector<Stock>& stocks) {
    // 40% chance a news event happens on any given tick
    if (rand() % 100 >= 40) return;
 
    static const vector<string> goodNews = {
        "reports record quarterly earnings!",
        "announces a major new product launch!",
        "secures a huge new contract!",
        "gets upgraded by top analysts!"
    };
    static const vector<string> badNews = {
        "faces a regulatory investigation.",
        "misses earnings expectations.",
        "recalls a major product line.",
        "loses a key executive unexpectedly."
    };
 
    int idx = rand() % stocks.size();
    Stock& s = stocks[idx];
    bool positive = (rand() % 2 == 0);
 
    double impactPercent = (5 + rand() % 10) / 100.0; // 5% - 14% move
    if (positive) {
        s.price += s.price * impactPercent;
        cout << colorize("\n[MARKET NEWS] ", Color::YELLOW + Color::BOLD)
             << s.name << " (" << s.symbol << ") "
             << colorize(goodNews[rand() % goodNews.size()], Color::GREEN) << "\n";
    } else {
        s.price -= s.price * impactPercent;
        cout << colorize("\n[MARKET NEWS] ", Color::YELLOW + Color::BOLD)
             << s.name << " (" << s.symbol << ") "
             << colorize(badNews[rand() % badNews.size()], Color::RED) << "\n";
    }
 
    if (s.price < 1.0) s.price = 1.0;
}

void updatePrices(vector<Stock>& stocks) {
    for (auto& s : stocks) {
        s.previousClose = s.price;
        double randomFactor = ((rand() % 2001) / 1000.0) - 1.0; // -1.0 to 1.0
        double change = randomFactor * s.volatility;
        s.price += change;
        if (s.price < 1.0) s.price = 1.0; 
    }
}

void executeSell(Portfolio& pf, vector<Stock>& stocks, Order& order, int tick, const string& txType) {
    Stock* s = findStock(stocks, order.symbol);
    if (!s) return;
 
    Holding& h = pf.holdings[order.symbol];
    int qty = min(order.qty, h.shares);
    if (qty <= 0) { order.active = false; return; }
 
    double avgCost = (h.shares > 0) ? (h.totalCost / h.shares) : 0.0;
    double proceeds = qty * s->price;
    double costBasis = qty * avgCost;
 
    pf.balance += proceeds;
    pf.realizedPL += (proceeds - costBasis);
    h.shares -= qty;
    h.totalCost -= costBasis;
 
    pf.history.push_back({tick, txType, order.symbol, qty, s->price, proceeds});
    cout << colorize("\n[ORDER TRIGGERED] ", Color::MAGENTA + Color::BOLD)
         << txType << " executed: sold " << qty << " shares of " << order.symbol
         << " at $" << fixed << setprecision(2) << s->price << "\n";
 
    order.active = false;
}
 
void executeLimitBuy(Portfolio& pf, vector<Stock>& stocks, Order& order, int tick) {
    Stock* s = findStock(stocks, order.symbol);
    if (!s) return;
 
    double cost = order.qty * s->price;
    if (cost > pf.balance) return; 
 
    pf.balance -= cost;
    Holding& h = pf.holdings[order.symbol];
    h.shares += order.qty;
    h.totalCost += cost;
 
    pf.history.push_back({tick, "LIMIT_BUY", order.symbol, order.qty, s->price, cost});
    cout << colorize("\n[ORDER TRIGGERED] ", Color::MAGENTA + Color::BOLD)
         << "LIMIT BUY executed: bought " << order.qty << " shares of " << order.symbol
         << " at $" << fixed << setprecision(2) << s->price << "\n";
 
    order.active = false;
}
 
void checkOrders(Portfolio& pf, vector<Stock>& stocks, int tick) {
    for (auto& order : pf.orders) {
        if (!order.active) continue;
        Stock* s = findStock(stocks, order.symbol);
        if (!s) continue;
 
        if (order.type == "STOP_LOSS" && s->price <= order.triggerPrice) {
            executeSell(pf, stocks, order, tick, "STOP_LOSS");
        } else if (order.type == "TAKE_PROFIT" && s->price >= order.triggerPrice) {
            executeSell(pf, stocks, order, tick, "TAKE_PROFIT");
        } else if (order.type == "LIMIT_SELL" && s->price >= order.triggerPrice) {
            executeSell(pf, stocks, order, tick, "LIMIT_SELL");
        } else if (order.type == "LIMIT_BUY" && s->price <= order.triggerPrice) {
            executeLimitBuy(pf, stocks, order, tick);
        }
    }
    pf.orders.erase(remove_if(pf.orders.begin(), pf.orders.end(),
                     [](const Order& o){ return !o.active; }),
                     pf.orders.end());
}

void printHeader(const string& username, int tick) {
    cout << colorize("\n=====================================\n", Color::CYAN);
    cout << colorize("      ADVANCED TRADING SIMULATOR\n", Color::CYAN + Color::BOLD);
    cout << colorize("=====================================\n", Color::CYAN);
    cout << "User: " << colorize(username, Color::YELLOW) << "   |   Market Tick: " << tick << "\n";
}
 
void printMarketOverview(const vector<Stock>& stocks) {
    cout << colorize("\n----------- MARKET OVERVIEW -----------\n", Color::BLUE + Color::BOLD);
    cout << left << setw(8) << "SYM" << setw(16) << "NAME" << setw(12) << "PRICE" << "CHANGE\n";
    for (const auto& s : stocks) {
        double change = s.price - s.previousClose;
        double pct = (s.previousClose != 0) ? (change / s.previousClose * 100.0) : 0.0;
        string changeStr = (change >= 0 ? "+" : "") ;
        ostringstream oss;
        oss << fixed << setprecision(2) << change << " (" << pct << "%)";
        string colored = colorize(changeStr + oss.str(), change >= 0 ? Color::GREEN : Color::RED);
 
        cout << left << setw(8) << s.symbol << setw(16) << s.name
             << "$" << fixed << setprecision(2) << setw(11) << s.price
             << colored << "\n";
    }
    cout << colorize("----------------------------------------\n", Color::BLUE);
}
 
void showPortfolio(const Portfolio& pf, const vector<Stock>& stocks) {
    cout << colorize("\n------------- PORTFOLIO -------------\n", Color::MAGENTA + Color::BOLD);
    cout << "Cash Balance: $" << fixed << setprecision(2) << pf.balance << "\n\n";
 
    double totalMarketValue = 0.0;
    double totalUnrealizedPL = 0.0;
 
    cout << left << setw(8) << "SYM" << setw(10) << "SHARES" << setw(12)
         << "AVG COST" << setw(12) << "PRICE" << setw(14) << "MKT VALUE" << "UNREALIZED P/L\n";
 
    for (const auto& s : stocks) {
        auto it = pf.holdings.find(s.symbol);
        if (it == pf.holdings.end() || it->second.shares == 0) continue;
 
        const Holding& h = it->second;
        double avgCost = h.totalCost / h.shares;
        double mktValue = h.shares * s.price;
        double unrealized = mktValue - h.totalCost;
 
        totalMarketValue += mktValue;
        totalUnrealizedPL += unrealized;
 
        string plStr = (unrealized >= 0 ? "+$" : "-$");
        ostringstream oss;
        oss << fixed << setprecision(2) << (unrealized >= 0 ? unrealized : -unrealized);
 
        cout << left << setw(8) << s.symbol << setw(10) << h.shares
             << "$" << fixed << setprecision(2) << setw(11) << avgCost
             << "$" << setw(11) << s.price
             << "$" << setw(13) << mktValue
             << colorize(plStr + oss.str(), unrealized >= 0 ? Color::GREEN : Color::RED) << "\n";
    }
 
    double totalEquity = pf.balance + totalMarketValue;
 
    cout << "\nTotal Market Value of Holdings: $" << fixed << setprecision(2) << totalMarketValue << "\n";
    cout << "Total Portfolio Equity:         $" << totalEquity << "\n";
    cout << "Realized P/L (from past sells): "
         << colorize((pf.realizedPL >= 0 ? "+$" : "-$") + money(abs(pf.realizedPL)),
                      pf.realizedPL >= 0 ? Color::GREEN : Color::RED) << "\n";
    cout << "Unrealized P/L (open positions): "
         << colorize((totalUnrealizedPL >= 0 ? "+$" : "-$") + money(abs(totalUnrealizedPL)),
                      totalUnrealizedPL >= 0 ? Color::GREEN : Color::RED) << "\n";
    cout << colorize("--------------------------------------\n", Color::MAGENTA);
}
 
void showTransactionHistory(const Portfolio& pf) {
    cout << colorize("\n---------- TRANSACTION HISTORY ----------\n", Color::CYAN + Color::BOLD);
    if (pf.history.empty()) {
        cout << "No transactions yet.\n";
    } else {
        cout << left << setw(6) << "TICK" << setw(14) << "TYPE" << setw(8)
             << "SYM" << setw(8) << "QTY" << setw(12) << "PRICE" << "TOTAL\n";
        for (const auto& t : pf.history) {
            string color = (t.type == "BUY" || t.type == "LIMIT_BUY") ? Color::GREEN
                          : (t.type == "STOP_LOSS") ? Color::RED : Color::YELLOW;
            string typeCol = t.type;
            typeCol.resize(14, ' ');
            cout << left << setw(6) << t.tick << colorize(typeCol, color)
                 << setw(8) << t.symbol << setw(8) << t.qty
                 << "$" << fixed << setprecision(2) << setw(11) << t.price
                 << "$" << t.total << "\n";
        }
    }
    cout << colorize("------------------------------------------\n", Color::CYAN);
}
 
void showOrders(const Portfolio& pf) {
    cout << colorize("\n------------- ACTIVE ORDERS -------------\n", Color::YELLOW + Color::BOLD);
    if (pf.orders.empty()) {
        cout << "No active orders.\n";
    } else {
        for (const auto& o : pf.orders) {
            cout << o.type << " | " << o.symbol << " | qty: " << o.qty
                 << " | trigger price: $" << fixed << setprecision(2) << o.triggerPrice << "\n";
        }
    }
    cout << colorize("------------------------------------------\n", Color::YELLOW);
}

void buyShares(Portfolio& pf, vector<Stock>& stocks, int tick) {
    printMarketOverview(stocks);
    string symbol = getValidLine("Enter stock symbol to buy: ");
    transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper);
 
    Stock* s = findStock(stocks, symbol);
    if (!s) {
        cout << colorize("Unknown stock symbol.\n", Color::RED);
        return;
    }
 
    int qty = getValidInt("How many shares do you want to buy? ", 1, 1000000);
    double cost = qty * s->price;
 
    if (cost > pf.balance) {
        cout << colorize("Not enough balance! Needed $" + money(cost) + "\n", Color::RED);
        return;
    }
 
    pf.balance -= cost;
    Holding& h = pf.holdings[symbol];
    h.shares += qty;
    h.totalCost += cost;
 
    pf.history.push_back({tick, "BUY", symbol, qty, s->price, cost});
    cout << colorize("Purchased " + to_string(qty) + " shares of " + symbol + " for $"
         + money(cost) + "\n", Color::GREEN);
}
 
void sellShares(Portfolio& pf, vector<Stock>& stocks, int tick) {
    printMarketOverview(stocks);
    string symbol = getValidLine("Enter stock symbol to sell: ");
    transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper);
 
    Stock* s = findStock(stocks, symbol);
    if (!s) {
        cout << colorize("Unknown stock symbol.\n", Color::RED);
        return;
    }
 
    Holding& h = pf.holdings[symbol];
    if (h.shares <= 0) {
        cout << colorize("You don't own any shares of " + symbol + ".\n", Color::RED);
        return;
    }
 
    int qty = getValidInt("How many shares do you want to sell? ", 1, h.shares);
 
    double avgCost = h.totalCost / h.shares;
    double proceeds = qty * s->price;
    double costBasis = qty * avgCost;
 
    pf.balance += proceeds;
    pf.realizedPL += (proceeds - costBasis);
    h.shares -= qty;
    h.totalCost -= costBasis;
 
    pf.history.push_back({tick, "SELL", symbol, qty, s->price, proceeds});
    cout << colorize("Sold " + to_string(qty) + " shares of " + symbol + " for $"
         + money(proceeds) + "\n", Color::GREEN);
}
 
void placeOrderMenu(Portfolio& pf, vector<Stock>& stocks) {
    printMarketOverview(stocks);
    cout << "\n1. Stop Loss  (auto-sell if price falls to X)\n";
    cout << "2. Take Profit (auto-sell if price rises to X)\n";
    cout << "3. Limit Buy   (auto-buy if price falls to X)\n";
    cout << "4. Limit Sell  (auto-sell if price rises to X)\n";
    cout << "5. Cancel / Back\n";
    int choice = getValidInt("Enter choice: ", 1, 5);
    if (choice == 5) return;
 
    string symbol = getValidLine("Enter stock symbol: ");
    transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper);
    Stock* s = findStock(stocks, symbol);
    if (!s) {
        cout << colorize("Unknown stock symbol.\n", Color::RED);
        return;
    }
 
    if (choice == 1 || choice == 4) {
        Holding& h = pf.holdings[symbol];
        if (h.shares <= 0) {
            cout << colorize("You don't own any shares of " + symbol + " to place this order.\n", Color::RED);
            return;
        }
    }
 
    int qty = getValidInt("Quantity: ", 1, 1000000);
    double trigger = getValidDouble("Trigger price: $", 0.01);
 
    string type = (choice == 1) ? "STOP_LOSS" : (choice == 2) ? "TAKE_PROFIT"
                : (choice == 3) ? "LIMIT_BUY" : "LIMIT_SELL";
 
    pf.orders.push_back({type, symbol, qty, trigger, true});
    cout << colorize("Order placed: " + type + " " + to_string(qty) + " " + symbol
         + " @ $" + money(trigger) + "\n", Color::GREEN);
}

void runSimulator(const string& username) {
    srand(time(0));
 
    vector<Stock> stocks = initStocks();
    Portfolio pf;
    int tick = 0;
    int choice;
 
    do {
        printHeader(username, tick);
        printMarketOverview(stocks);
 
        cout << "\n1. Buy Shares";
        cout << "\n2. Sell Shares";
        cout << "\n3. View Portfolio";
        cout << "\n4. Next Market Tick";
        cout << "\n5. Transaction History";
        cout << "\n6. Place Order (Stop Loss / Take Profit / Limit)";
        cout << "\n7. View Active Orders";
        cout << "\n8. Exit";
        cout << "\n";
 
        choice = getValidInt("Enter choice: ", 1, 8);
 
        switch (choice) {
            case 1: buyShares(pf, stocks, tick); break;
            case 2: sellShares(pf, stocks, tick); break;
            case 3: showPortfolio(pf, stocks); break;
            case 4: {
                tick++;
                updatePrices(stocks);
                generateMarketNews(stocks);
                checkOrders(pf, stocks, tick);
                cout << colorize("\nMarket tick " + to_string(tick) + " complete.\n", Color::CYAN);
                break;
            }
            case 5: showTransactionHistory(pf); break;
            case 6: placeOrderMenu(pf, stocks); break;
            case 7: showOrders(pf); break;
            case 8:
                cout << colorize("\nThanks for using the Trading Simulator, " + username + "!\n", Color::CYAN);
                break;
        }
 
    } while (choice != 8);
}
 
int main() {
    string username = loginSystem();
    runSimulator(username);
    return 0;
}
