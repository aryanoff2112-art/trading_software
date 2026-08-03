# 📈 Advanced Trading Simulator (C++)

A console-based stock market trading simulator written in modern C++.

The simulator allows users to trade virtual stocks, maintain portfolios, place automated orders, and experience dynamic market price changes with simulated news events.

---

## Features

### Authentication
- Login system
- User Registration
- Local account storage

### Stock Market
- Multiple companies
- Live price fluctuations
- Volatility simulation
- Market news events
- Price history

### Trading
- Buy Stocks
- Sell Stocks
- Portfolio Management
- Cash Balance Tracking
- Realized Profit/Loss
- Unrealized Profit/Loss

### Advanced Orders

Supports:

- Stop Loss
- Take Profit
- Limit Buy
- Limit Sell

Orders execute automatically when market conditions are met.

### Portfolio

Displays

- Holdings
- Average Cost
- Market Value
- Total Equity
- Profit/Loss

### Transaction History

Keeps complete record of every transaction.

### Colorful Console

ANSI colored output for better readability.

---

## Technologies

- C++
- STL
- File Handling
- OOP
- Maps
- Vectors
- Random Number Generation

---

## Folder Structure

```
src/
    main.cpp

data/
    users.dat (generated automatically)
```

---

## How to Run

### Compile

```bash
g++ src/main.cpp -o TradingSimulator
```

Windows

```bash
TradingSimulator.exe
```

Linux / Mac

```bash
./TradingSimulator
```

---

## Sample Stocks

| Symbol | Company |
|---------|----------|
| TECH | TechCorp |
| BANK | Bankwell Inc. |
| ENGY | Energen |
| FOOD | FreshFoods |
| SPCE | SpaceVenture |

---

## Future Improvements

- Save Portfolio
- Candlestick Charts
- Watchlist
- Dividend System
- Multiple User Portfolios
- CSV Export
- Graphical Interface
- Multiplayer Trading
- Online Market Data
- Risk Analysis

---

## Author

Aryan Upadhyay

---

## License

MIT License
