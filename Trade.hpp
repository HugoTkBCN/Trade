/*
** EPITECH PROJECT, 2020
** Trade
** File description:
** hpp
*/

#ifndef TRADE_HPP_
#define TRADE_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include <map>
#include <bits/stdc++.h>

class Trade
{
public:
    Trade();
    ~Trade();

    void run(void);

protected:
private:
    void Error(std::string);

    void loop(void);

    void getInput(void);
    void getSettings(void);
    std::string getInfo(std::vector<std::string>, std::string);

    void setFormat(void);
    void setSettings(void);
    void setStack(std::string);

    void appendCandles(std::string);

    bool buyMoney(std::vector<std::map<std::string, float>>, std::string, float, int, bool);
    void order();

    float getMiddleBB(std::vector<float>, int);
    float getAverage(std::vector<float>);
    float getStandartDev(std::vector<float>, int);
    std::vector<float> getCloseArr(std::vector<std::map<std::string, float>>);


    std::string _input = "";
    std::map<std::string, std::string> _settings;
    int _timeBank = 0;
    int _timePerMove = 0;
    std::string _playerNames = "";
    std::string _yourBot = "";
    int _candleInterval = 0;
    int _candlesTotal = 0;
    int _candlesGiven = 0;
    int _initialStack = 0;
    std::string _candleFormat = "";
    std::vector<std::string> _arrayCandleFormat;
    int _transactionFeePercent = 0;
    std::map<std::string, int> _format;
    int _BTCStack = 0;
    int _ETHStack = 0;
    int _USDTStack = 0;
    std::vector<std::map<std::string, float>> _BTC_ETH_Array;
    std::vector<std::map<std::string, float>> _USDT_ETH_Array;
    std::vector<std::map<std::string, float>> _USDT_BTC_Array;
};

#endif /* !TRADE_HPP_ */
