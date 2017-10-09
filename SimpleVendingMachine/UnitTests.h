#pragma once

//This file stores unit tests
bool TestNormalTransaction(CVendingMachine &vendingMachine) {
	cout << "=======Test Normal Transaction=======" << endl;

	vendingMachine.InsertMoney(QUARTER);
	vendingMachine.InsertMoney(NICKEL);
	vendingMachine.InsertMoney(PENNY);
	vendingMachine.InsertMoney(DIME);
	vendingMachine.InsertMoney(QUARTER);
	vendingMachine.InsertMoney(QUARTER);
	vendingMachine.InsertMoney(QUARTER);
	vendingMachine.InsertMoney(DOLLAR);
	vendingMachine.SelectProduct("mtndew");

	vendingMachine.DisplayProduct();
	vendingMachine.DisplayMoney();
	cout << endl;
	return true;
}

bool TestInvalidMoney(CVendingMachine &vendingMachine) {
	cout << "=======Test Invalid Money=======" << endl;

	vendingMachine.InsertMoney(DOLLAR);
	vendingMachine.InsertMoney(DOLLAR);
	vendingMachine.InsertMoney(FAKECOIN);
	vendingMachine.SelectProduct("mtndew");

	vendingMachine.DisplayProduct();
	vendingMachine.DisplayMoney();
	cout << endl;
	return true;
}

bool TestInvalidProduct(CVendingMachine &vendingMachine) {
	cout << "=======Test Invalid Product=======" << endl;

	vendingMachine.InsertMoney(DOLLAR);
	vendingMachine.InsertMoney(QUARTER);
	vendingMachine.SelectProduct("randomproduct");
	vendingMachine.SelectProduct("pepsi");
	vendingMachine.InsertMoney(DOLLAR);
	vendingMachine.SelectProduct("pepsi");

	vendingMachine.DisplayProduct();
	vendingMachine.DisplayMoney();
	cout << endl;
	return true;
}

bool TestCancel(CVendingMachine &vendingMachine) {
	cout << "=======Test Cancel=======" << endl;
	vendingMachine.InsertMoney(DIME);
	vendingMachine.Cancel();

	vendingMachine.DisplayProduct();
	vendingMachine.DisplayMoney();
	cout << endl;
	return true;
}

bool TestNoQuantity(CVendingMachine &vendingMachine) {
	cout << "=======Test No Enough Quantity=======" << endl;
	vendingMachine.InsertMoney(DOLLAR);
	vendingMachine.SelectProduct("sprite");
	vendingMachine.SelectProduct("zevia");

	vendingMachine.DisplayProduct();
	vendingMachine.DisplayMoney();
	cout << endl;
	return true;
}

bool TestNoEnoughChanges(CVendingMachine &vendingMachine) {
	cout << "=======Test No Enough Changes=======" << endl;
	for (int i = 0; i < 6; i++) {
		vendingMachine.InsertMoney(DOLLAR);
		vendingMachine.InsertMoney(DOLLAR);
		vendingMachine.SelectProduct("fenta");
	}
	vendingMachine.DisplayProduct();
	vendingMachine.DisplayMoney();
	cout << endl;
	return true;
}

bool TestNoProductLeft(CVendingMachine &vendingMachine) {
	cout << "=======Test No Product Left=======" << endl;
	for (int i = 0; i < 4; i++) {
		vendingMachine.InsertMoney(DOLLAR);
		vendingMachine.InsertMoney(QUARTER);
		vendingMachine.InsertMoney(QUARTER);
		vendingMachine.SelectProduct("pepsi");
	}
	for (int i = 0; i < 2; i++) {
		vendingMachine.InsertMoney(DOLLAR);
		vendingMachine.SelectProduct("zevia");
	}
	for (int i = 0; i < 2; i++) {
		vendingMachine.InsertMoney(DOLLAR);
		vendingMachine.InsertMoney(DOLLAR);
		vendingMachine.SelectProduct("fenta");
	}
	vendingMachine.DisplayProduct();
	vendingMachine.DisplayMoney();
	cout << endl;
	return true;
}