#include<iostream>
#include<iomanip>
#include "CVendingMachine.h"
#include "UnitTests.h"
using namespace std;

/*****Readme*******************************************
File structure:
CProduct.h: contains the class for products in the vending machine
CVendingMachine.h: contains the class for vending machine
UnitTests.h: some unit tests to validate the code
main.cpp: entrance of the program, instantiate and initial the vending machine, contains a while loop to mock customer behavior
*********************************************************/

/*****Assignment Description**********************************
Design an API for a vending machine that dispenses items of your choice using object oriented design principles. 
Please provide a main.cpp with your API. 
You may use any programming language, but I would prefer if you use C++ and the code compiles. 
List any assumptions in your design. 
I am not looking for an exhaustive design here, and you may skip over internal the implementation of some your member functions.
************************************************************/

/*****Design: Five vending machine states**************************
State 1: Standby and accept payment state
-If invalid payment(ex: insert Canadian coin), go to dispense invalid payment state
-If press cancel button, go to dispense money state
-If press select product button:
--If not enough quantity, stay at this state
--If unknown input, stay at this state
--If money not enough, stay at this state
--If no enough changes, go to dispense money state
--Go to dispense product state
State 2: Dispense product state: dispense product, then go to dispense money state
-If no product left, go to maintainance state
-If product can not be dispensed due to machine error, go to maintainance state
State 3: Dispense money state: dispense the changes, then go to accept payment state
State 4: Dispense invalid money state: dispense unacceptable money, then go to accept payment state
State 5: Maintainance state: first dispense all changes, then wait for repair or restock. If dealt with, go to accept payment state
*********************************************************/

/*****Data structure*************************************
For a system like vending machine with a few products, can use a simple in-memory database.
Assume our system deals with one customer at a time.
Assume each product is associated with an unique id number. This number is initialized in the vending maching class and no need to update.
Because all data store in one machine so we can use a simple key-value pair database.(NoSQL)
To get constant query time, use hashmap to store product information, key is product id, value is a CProduct object
[pid] = [CProduct]
Also keep changes in the inventory.
[money] = [quantities]
*********************************************************/

/*****Algorithm******************************************
When dispensing the change coins, in order to dispense less coin, use greedy method, from larger amount to smaller amount.
*********************************************************/

/*****Test***********************************************
A unit test file is provided to cover most of the code.
Because it is a single component system, no need for integration test or hermetic test
*********************************************************/

/*****Performance evaluation*******************************
Refill product/money costs linear time.
Insert money/select product/dispense costs constant time.
For a vending machine, the performance bottleneck is not at the software end. no need cache.
*********************************************************/

/*****Other assumptions*******************************
In this program I assume all machine-related behaviors are completed instantly.
(Such as dispense product)
However, it may take time to process in reality. 
So some of the functions should be written as call back functions or asynchronous functions.
I ignore this difference and focus on the state machine logic.
*********************************************************/

void Initialization(CVendingMachine &vendingMachine) {
	vendingMachine.RefillProduct(CProduct("pepsi", 5, 1.5));
	vendingMachine.RefillProduct(CProduct("mtndew", 2, 1.75));
	vendingMachine.RefillProduct(CProduct("zevia", 3, 1.0));
	vendingMachine.RefillProduct(CProduct("fenta", 6, 1.25));
	vendingMachine.RefillProduct(CProduct("sprite", 0, 1.5));
	
	vendingMachine.RefillMoney(PENNY, 10);
	vendingMachine.RefillMoney(NICKEL, 10);
	vendingMachine.RefillMoney(DIME, 10);
	vendingMachine.RefillMoney(QUARTER, 10);
	vendingMachine.RefillMoney(DOLLAR, 10);
}

void ProcessUserCommand(CVendingMachine &vendingMachine, int cmd) {
	switch (cmd) {
		case 0:
			break;
		case 1:
			vendingMachine.InsertMoney(PENNY);
			break;
		case 2:
			vendingMachine.InsertMoney(NICKEL);
			break;
		case 3:
			vendingMachine.InsertMoney(DIME);
			break;
		case 4:
			vendingMachine.InsertMoney(QUARTER);
			break;
		case 5:
			vendingMachine.InsertMoney(DOLLAR);
			break;
		case 6:
			vendingMachine.InsertMoney(FAKECOIN);
			break;
		case 7:
			vendingMachine.SelectProduct("pepsi");
			break;
		case 8:
			vendingMachine.SelectProduct("mtndew");
			break;
		case 9:
			vendingMachine.SelectProduct("zevia");
			break;
		case 10:
			vendingMachine.SelectProduct("fenta");
			break;
		case 11:
			vendingMachine.SelectProduct("sprite");
			break;
		case 12:
			vendingMachine.Cancel();
		default:
			break;
	}

	cout << "--------------------------------------------" << endl;
	vendingMachine.DisplayProduct();
	vendingMachine.DisplayMoney();
	vendingMachine.DisplayMoneyInserted();
	cout << "--------------------------------------------" << endl;
}

void RunUnitTest(CVendingMachine &vendingMachine) {
	TestNormalTransaction(vendingMachine);
	TestInvalidMoney(vendingMachine);
	TestInvalidProduct(vendingMachine);
	TestCancel(vendingMachine);
	TestNoQuantity(vendingMachine);
	TestNoEnoughChanges(vendingMachine);
	TestNoProductLeft(vendingMachine);
}

int main() {
	CVendingMachine vendingMachine;

	//Run unit tests, make sure every function works
	Initialization(vendingMachine);
	RunUnitTest(vendingMachine);

	//Mock behavior of the customer
	Initialization(vendingMachine);
	int userCommand;
	while (true) {
		cout << "Enter your choice:" << endl;
		cout << "(0. display inventory" << endl;

		cout << "(1: insert penny." << endl;
		cout << "(2: insert nickel." << endl;
		cout << "(3: insert dime." << endl;
		cout << "(4: insert quarter." << endl;
		cout << "(5: insert dollar." << endl;
		cout << "(6: insert fake coin." << endl;

		cout << "(7: select Pepsi." << endl;
		cout << "(8: select MtnDew." << endl;
		cout << "(9: select Zevia." << endl;
		cout << "(10: select Fenta." << endl;
		cout << "(11: select Sprite." << endl;

		cout << "(12: cancel." << endl;
		cout << "(13: leave." << endl;

		cin >> userCommand;
		if (13 == userCommand) break;
		else ProcessUserCommand(vendingMachine, userCommand);
	}

	return 0;
}
