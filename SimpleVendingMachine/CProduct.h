#pragma once
#include<string>
using namespace std;

/*One type of products in the inventory*/
class CProduct {
	string name;
	int quantity;
	float price;
public:
	CProduct() :name(""), quantity(0), price(0) { }

	CProduct(string Name, int Quantity, float Price) :
		name(Name), quantity(Quantity), price(Price) {
	}

	string GetName() { return name; }
	int GetQuantity() { return quantity; }
	float GetPrice() { return price; }

	void AddQuantity(int Quantity) { quantity += Quantity; }

	//These 2 functions are not used in the main function
	void UpdateName(string Name) { name = Name; }
	void UpdatePrice(float Price) { price = Price; }

};