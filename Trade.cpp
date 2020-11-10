/*
** EPITECH PROJECT, 2020
** Trade
** File description:
** ccp
*/

#include "Trade.hpp"

Trade::Trade()
{
}

Trade::~Trade()
{
}

void Trade::Error(std::string str)
{
	std::cerr << str << std::endl;
    exit(84);
}

void Trade::getInput(void)
{
    if (!std::getline(std::cin, this->_input))
        exit(0);
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> vector;
    std::string str;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, str, delimiter))
        vector.push_back(str);
    return (vector);
}

void Trade::getSettings(void)
{
    std::vector<std::string> tmp;

    for (int i = 0; i < 10; i++) {
        this->getInput();
        tmp = split(this->_input, ' ');
        if (strcmp(tmp[0].c_str(), "settings") != 0 || tmp.size() < 3)
			Error("Bad formatted setting : " + this->_input);
        this->_settings[tmp[1]] = tmp[2];
    }
}

void Trade::setFormat(void)
{
    int checkNbrVal = 0;
	std::vector<std::string> candleArray = {"pair", "date", "high", "low", "open", "close", "volume"};

    this->_arrayCandleFormat = split(this->_candleFormat, ',');
	if (this->_arrayCandleFormat.size() != 7)
		Error("Error with candle_format : " + this->_candleFormat);
	for (std::string tmpFormat : this->_arrayCandleFormat)
		for (std::string tmp : candleArray)
			if (tmpFormat == tmp) {
				this->_format[tmp] = checkNbrVal;
				checkNbrVal += 1;
                auto itr = std::find(candleArray.begin(), candleArray.end(), tmp);
                if (itr != candleArray.end())
					candleArray.erase(itr);
            }
	if (checkNbrVal != 7)
		Error("Error with candle_format : " + this->_candleFormat);
}

void Trade::setSettings()
{
    try {
    	this->_candleInterval = std::stoi(this->_settings["candle_interval"]);
    	this->_candlesTotal = std::stoi(this->_settings["candles_total"]);
    	this->_candlesGiven = std::stoi(this->_settings["candles_given"]);
    	this->_initialStack = std::stoi(this->_settings["initial_stack"]);
    	this->_candleFormat = this->_settings["candle_format"];
    	this->_transactionFeePercent = std::stof(this->_settings["transaction_fee_percent"]);
		this->_timeBank = std::stoi(this->_settings["timebank"]);
		this->_timePerMove = std::stoi(this->_settings["time_per_move"]);
		this->_playerNames = this->_settings["player_names"];
		this->_yourBot = this->_settings["your_bot"];
    } catch (int e) {
        Error("Bad settings");
	}
    this->setFormat();
}

void Trade::setStack(std::string str)
{
    std::vector<std::string> values = split(str, ',');
	std::vector<std::string> info;

	if (values.size() != 3)
		Error("Wrong value : " + str);
	for (std::string value : values) {
		info = split(value, ':');
		if (info.size() != 2)
			Error("Wrong value : " + value);
        else if (strcmp(info[0].c_str(), "BTC") == 0)
			this->_BTCStack = std::stof(info[1]);
		else if (strcmp(info[0].c_str(), "ETH") == 0)
			this->_ETHStack = std::stof(info[1]);
		else if (strcmp(info[0].c_str(), "USDT") == 0)
			this->_USDTStack = std::stof(info[1]);
		else
			Error("Unknow val : " + value);
    }
}

std::string Trade::getInfo(std::vector<std::string> array, std::string info)
{
    return (array[this->_format[info]]);
}

void Trade::appendCandles(std::string candles)
{
    std::vector<std::string> arrayCandles = split(candles, ';');

	for (std::string candle : arrayCandles) {
		std::vector<std::string> info = split(candle, ',');
        std::map<std::string, float> tmp;
		tmp["date"] = std::stof(this->getInfo(info, "date"));
		tmp["high"] = std::stof(this->getInfo(info, "high"));
		tmp["low"] = std::stof(this->getInfo(info, "low"));
		tmp["open"] = std::stof(this->getInfo(info, "open"));
		tmp["close"] = std::stof(this->getInfo(info, "close"));
		tmp["volume"] = std::stof(this->getInfo(info, "volume"));
		if (this->getInfo(info, "pair") == "BTC_ETH")
			this->_BTC_ETH_Array.push_back(tmp);
        if (this->getInfo(info, "pair") == "USDT_ETH")
			this->_USDT_ETH_Array.push_back(tmp);
        if (this->getInfo(info, "pair") == "USDT_BTC")
			this->_USDT_BTC_Array.push_back(tmp);
    }
}

float Trade::getAverage(std::vector<float> arr)
{
    double returnValue = 0.0;
    int n = arr.size();
       
    for ( int i=0; i < n; i++)
        returnValue += arr[i];
    return (returnValue / n);
}

float Trade::getMiddleBB(std::vector<float> arr, int period)
{
    std::vector<float> newList(arr.end() - period, arr.end());
    return (getAverage(newList));
}

float Trade::getStandartDev(std::vector<float> arr, int period)
{
    std::vector<float> newList(arr.end() - period, arr.end());
	float mean = getAverage(newList);
	float deviationSum = 0;
	
    for (float elem : newList)
		deviationSum += (abs(elem - mean))^2;
	return (sqrt(deviationSum / period));
}

std::vector<float> Trade::getCloseArr(std::vector<std::map<std::string, float>> chartData)
{
	std::vector<float> closeArr;
	std::vector<std::map<std::string, float>> tmp = chartData;

    tmp.pop_back();
	for (std::map<std::string, float> i : tmp)
		closeArr.push_back(i["close"]);
	return (closeArr);
}

bool Trade::buyMoney(std::vector<std::map<std::string, float>> chartData, std::string traidingPair, float moneyStack, int sellMoneyStack, bool hasBought)
{
	std::vector<float> closeArr = getCloseArr(chartData);
	float middleBB = getMiddleBB(closeArr, this->_candlesGiven);
	float standartDev = getStandartDev(closeArr, this->_candlesGiven);
	float highBB = middleBB + (2 * standartDev);
	float lowBB = middleBB - (2 * standartDev);
	float buyVal = ((lowBB - closeArr.back()) / 10) * moneyStack;
	float sellVal = ((closeArr.back() - highBB) / 10) * sellMoneyStack;

	if (closeArr.back() < lowBB && moneyStack > buyVal && buyVal > 0.001) {
		if (hasBought)
			std::cout << ";";
		std::cout << "buy " << traidingPair << " " << std::to_string(buyVal);
    } else if (closeArr.back() > highBB && sellMoneyStack > sellVal && sellVal > 0.9) {
		if (hasBought)
			std::cout << ";";
		std::cout << "sell " << traidingPair << " " << std::to_string(sellVal);
	    std::cerr << "should sell !" << std::endl;
    } else
		return (false);
	return (true);
}

void Trade::order()
{
    bool passBool = false;
    bool hasBought = this->buyMoney(this->_USDT_ETH_Array, "USDT_ETH", this->_USDTStack / this->_USDT_ETH_Array.back()["close"], this->_ETHStack, false);
	passBool = hasBought ? hasBought : passBool;
	hasBought = this->buyMoney(this->_BTC_ETH_Array, "BTC_ETH", this->_BTCStack / this->_BTC_ETH_Array.back()["close"], this->_ETHStack, passBool);
	passBool = hasBought ? hasBought : passBool;
	hasBought = this->buyMoney(this->_USDT_BTC_Array, "USDT_BTC", this->_USDTStack / this->_USDT_BTC_Array.back()["close"], this->_BTCStack, passBool);
	passBool = hasBought ? hasBought : passBool;
	passBool == false ? 
		std::cout << "pass" << std::endl :
		std::cout << "" << std::endl;
}

void Trade::loop(void)
{
    std::vector<std::string> arrayInput;

    while (true) {
        this->getInput();
        arrayInput = split(this->_input, ' ');
        if (arrayInput[0] == "update" && arrayInput[1] == "game" && arrayInput[2] == "next_candles")
            this->appendCandles(arrayInput[3]);
        else if (arrayInput[0] == "update" && arrayInput[1] == "game" && arrayInput[2] == "stacks")
            this->setStack(arrayInput[3]);
        else if (arrayInput[0] == "action" && arrayInput[1] == "order")
            this->order();
        else
            std::cerr << "Unknow cmd : " << this->_input << std::endl;
    }
}

void Trade::run()
{
    this->getSettings();
    this->setSettings();
    this->loop();
}