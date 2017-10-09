#pragma once
#include "CProduct.h"
#include <unordered_map>
#include <algorithm>
using namespace std;

enum VendingMachineStates {
	ACCEPTPAYMENT,
	DISPENSEPRODUCT,
	DISPENSEMONEY,
	DISPENSEINVALIDMONEY,
	MAINTENANCE
};

/*Assume we accept 5 types of money*/
enum MoneyEnum
{
	PENNY,
	NICKEL,
	DIME,
	QUARTER,
	DOLLAR,
	FAKECOIN //invalid money
};



/*This is the machine*/
class CVendingMachine {
	unordered_map<int, CProduct> productInventory; //our product database
	unordered_map<string, int> product_id_map; //mapping of product and pid 
	unordered_map<MoneyEnum, int> moneyInventory; //our money database
	unordered_map<MoneyEnum, float> money_value_map; //mapping of money and its value
	unordered_map<MoneyEnum, string> money_name_map; //just for display purpose

	float changeMoney;
	int selectedProductId;
	VendingMachineStates state;

	int totalProductCnt;

	//these variables store how many coins to give back to the customer
	int dollarCnt;
	int quarterCnt;
	int dimeCnt;
	int nickleCnt;
	int pennyCnt;
public:
	CVendingMachine() {
		//product - id mapping
		product_id_map["pepsi"] = 0;
		product_id_map["mtndew"] = 1;
		product_id_map["zevia"] = 2;
		product_id_map["fenta"] = 3;
		product_id_map["sprite"] = 4;

		//money - value mapping
		money_value_map[PENNY] = 0.01;
		money_value_map[NICKEL] = 0.05;
		money_value_map[DIME] = 0.1;
		money_value_map[QUARTER] = 0.25;
		money_value_map[DOLLAR] = 1;

		//money - name mapping
		money_name_map[PENNY] = "Penny";
		money_name_map[NICKEL] = "Nickel";
		money_name_map[DIME] = "Dime";
		money_name_map[QUARTER] = "Quarter";
		money_name_map[DOLLAR] = "Dollar";

		totalProductCnt = 0;

		ResetMachine();
	}

	void ResetMachine() {
		//initial states
		changeMoney = 0;
		selectedProductId = -1;
		state = ACCEPTPAYMENT;

		dollarCnt = 0;
		quarterCnt = 0;
		dimeCnt = 0;
		nickleCnt = 0;
		pennyCnt = 0;
	}

	/*Stock product and money*/
	void RefillProduct(CProduct &product) {
		int pid = product_id_map[product.GetName()];
		if (productInventory.find(pid) != productInventory.end())
			productInventory[pid].AddQuantity(product.GetQuantity());
		else
			productInventory[pid] = product;
		totalProductCnt += product.GetQuantity();

		state = ACCEPTPAYMENT;
	}
	void RefillMoney(MoneyEnum money, int quantity) {
		if (moneyInventory.find(money) != moneyInventory.end())
			moneyInventory[money] += quantity;
		else
			moneyInventory[money] = quantity;
		state = ACCEPTPAYMENT;
	}

	/*Show the produces and money*/
	void DisplayProduct() {
		cout << "Inventory: " << endl;
		for (unordered_map<int, CProduct>::iterator iter = productInventory.begin(); iter != productInventory.end(); iter++) {
			CProduct *product = &iter->second;
			cout /*<< iter->first << " " << setw(8)*/ << product->GetName() << "($" << product->GetPrice() << "):" << product->GetQuantity() << "    ";
		}
		cout << endl;
	}
	void DisplayMoney() {
		for (unordered_map<MoneyEnum, int>::iterator iter = moneyInventory.begin(); iter != moneyInventory.end(); iter++) {
			cout << money_name_map[iter->first] << ":" << iter->second << "    ";
		}
		cout << endl;
	}
	void DisplayMoneyInserted() {
		cout << "Money inserted: $" << changeMoney << endl;
	}


	/*State 1: pay - select product*/
	void InsertMoney(MoneyEnum money) {
		if (state == ACCEPTPAYMENT) {
			if (money >= PENNY && money <= DOLLAR) {
				moneyInventory[money]++;
				cout << "$" << money_value_map[money] << " inserted. ";
				changeMoney += money_value_map[money];
				cout << "($" << changeMoney << " in machine)" << endl;
			}
			else {
				cout << "Invalid money inserted." << endl;
				state = DISPENSEINVALIDMONEY;
				DispenseInvalidChange();
			}
		}
		else {
			cout << "No response." << endl;
		}
	}

	/*Cancel purchase and refund*/
	void Cancel() {
		cout << "Transaction cancelled." << endl;

		//compute how many coins to refund
		dollarCnt = DispenseCoin(DOLLAR);
		quarterCnt = DispenseCoin(QUARTER);
		dimeCnt = DispenseCoin(DIME);
		nickleCnt = DispenseCoin(NICKEL);
		pennyCnt = DispenseCoin(PENNY);

		DispenseChange();
	}

	void SelectProduct(string name) {
		if (state == ACCEPTPAYMENT) {
			//Check if the product is found 
			if (product_id_map.find(name) == product_id_map.end()) {
				cout << "Product not found! Please select other product." << endl;
				return;
			}

			//Check if the quantity is not zero
			selectedProductId = product_id_map[name];
			cout << name << "($" << productInventory[selectedProductId].GetPrice() << ") selected." << endl;
			if (productInventory[selectedProductId].GetQuantity() <= 0) {
				cout << "No enough quantity of this product. Please select other product." << endl;
				return;
			}

			//Check if inserted enough money
			if (changeMoney < productInventory[selectedProductId].GetPrice()) {
				cout << "Please insert more money. ($" << changeMoney << " in machine)" << endl;
				return;
			}

			
			//Check if there is enough change
			float currentMoney = changeMoney;
			changeMoney -= productInventory[selectedProductId].GetPrice();
			dollarCnt = DispenseCoin(DOLLAR);
			quarterCnt = DispenseCoin(QUARTER);
			dimeCnt = DispenseCoin(DIME);
			nickleCnt = DispenseCoin(NICKEL);
			pennyCnt = DispenseCoin(PENNY);
			if (changeMoney != 0) {
				changeMoney = currentMoney;
				cout << "No enough change. Cancel transaction." << endl;
				Cancel();
				return;
			}

			//can dispense product without problem
			DispenseProduct();
		}
		else {
			//the machine will not response unless it is in accept payment state
			cout << "No response." << endl;
		}
	}

	/*State 2: dispense product*/
	bool DispenseProductAPI() {
		//a function to mock the machine API which push product to customer
		return true;
	}
	void DispenseProduct() {
		if (!DispenseProductAPI()) {
			cout << "Machine Error!" << endl;
			state = DISPENSEMONEY;
			DispenseChange();
			state = MAINTENANCE;
			return;
		}

		cout << "Product(" << productInventory[selectedProductId].GetName() << ") dispensed successfully! ";
		productInventory[selectedProductId].AddQuantity(-1);
		cout << "(" << productInventory[selectedProductId].GetQuantity() << " left)"<<endl;

		totalProductCnt--;

		state = DISPENSEMONEY;
		DispenseChange();
	}

	/*State 3: dispense money*/
	//Greedy method, first dispense large amound coins
	void DispenseChange() {
		cout << "Coin dispensed: "
			<< dollarCnt << " dollar(s). "
			<< quarterCnt << " quarter(s). "
			<< dimeCnt << " dime(s). "
			<< nickleCnt << " nickle(s). "
			<< pennyCnt << " penny(s). ";

		cout << "($" << changeMoney << " in machine)" << endl;

		if (totalProductCnt == 0) state = MAINTENANCE;
		else ResetMachine();
	}
	int DispenseCoin(MoneyEnum coin) {
		float value = money_value_map[coin];
		int cnt = min((int)((changeMoney + 0.01f / 2) / value), moneyInventory[coin]);
		changeMoney -= cnt * value;
		if (changeMoney < 0.01f / 2) changeMoney = 0;
		moneyInventory[coin] -= cnt;
		return cnt;
	}

	/*State 4: dispense invalid money*/
	void DispenseInvalidChange() {
		cout << "Invalid money dispensed. ";
		state = ACCEPTPAYMENT;
		cout << "($" << changeMoney << " in machine)" << endl;
	}

};