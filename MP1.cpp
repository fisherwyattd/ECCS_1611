// MP1.cpp - Wyatt Fisher - 26 August 2020
// This MP1 program will calculate the total cost of a customer's donut purchase.
// The program will ask for the number of fancy and regular donuts purchased and will calculate tax.
// The program will then ask for money the customer paid to calculate the correct amount of change that should be given.

#include<iostream>
#include<math.h>
using namespace std;

int main() {
	// initialize donut prices and tax
	double const COST_REG_DONUT = 0.75;
	double const COST_REG_DOZEN = 7.99;
	double const COST_FANCY_DONUT = 0.85;
	double const COST_FANCY_DOZEN = 8.49;
	double const SALES_TAX = 0.075;
	

	// initialize change values (will change totalChangeOwed into an int below... this will make sense later in the code)
	int const DOLLAR = 100;
	int const QUARTER = 25;
	int const DIME = 10;
	int const NICKEL = 5;
	int const PENNY = 1;


	// ask for number and types of donuts ordered.
	cout << "Number of regular donuts ordered: ";
	int numRegDonuts = 0; // integer value of the number of regular donuts
	cin >> numRegDonuts;

	cout << "Number of fancy donuts ordered: ";
	int numFancyDonuts = 0; // integer value of the number of fancy donuts
	cin >> numFancyDonuts;


	// calculate what customer owes for regular donuts
	int dozenRegDonuts = (numRegDonuts / 12); // calculate number of regular donut dozens
	int singleRegDonuts = (numRegDonuts % 12); // calculate remainder of single regular donuts
	float totalRegDonutCost = (dozenRegDonuts * COST_REG_DOZEN) + (singleRegDonuts * COST_REG_DONUT);


	// calculate what customer owes for fancy donuts
	int dozenFancyDonuts = (numFancyDonuts / 12); // calculate number of fancy donut dozens
	int singleFancyDonuts = (numFancyDonuts % 12); // calculate remainder of single fancy donuts
	float totalFancyDonutCost = (dozenFancyDonuts * COST_FANCY_DOZEN) + (singleFancyDonuts * COST_FANCY_DONUT);


	// calculate total donut cost and add tax
	float totalDonutCost = totalRegDonutCost + totalFancyDonutCost; // calculate total cost of donuts
	float customerOwes = roundf((totalDonutCost + (totalDonutCost * SALES_TAX)) * 100) / 100; // calculate/round customer's cost
	cout.precision(2); // set output at two decimal places
	cout << "Customer owes $" << fixed << customerOwes << endl;


	// ask for how much customer paid and calculate change
	cout << "Customer pays $";
	float customerPaid = 0.0;
	cin >> customerPaid;
	float totalChangeOwed = roundf((customerPaid - customerOwes) * 100) / 100; // calculate rounded change to give customer


	// convert totalChangeOwed into an int for easier manipulation below
	int intTotalChange = totalChangeOwed * 100;

	int dollarsOwed = intTotalChange / 100; // calculate dollars to be given to customer
	int changeStillOwed = intTotalChange - (dollarsOwed * DOLLAR);

	int quartersOwed = changeStillOwed / QUARTER; // calculate quarters to be given to customer
	changeStillOwed = changeStillOwed - (quartersOwed * QUARTER);

	int dimesOwed = changeStillOwed / DIME; // calculate dimes to be given to customer
	changeStillOwed = changeStillOwed - (dimesOwed * DIME);

	int nickelsOwed = changeStillOwed / NICKEL; // calculate nickels to be given to customer
	changeStillOwed = changeStillOwed - (nickelsOwed * NICKEL);

	int penniesOwed = changeStillOwed; // calculate pennies to be given to customer


	// print to console the amount of change owed (if any) and the money/coins that should be given to the customer
	if (customerPaid == roundf(customerOwes * 100) / 100) { // if the customer paid exactly what they owed
		cout << "Exact payment received - no change owed." << endl;
	}
	else { // if the customer needs change
		cout.precision(2); // print to two decimal places
		cout << "Change owed is $" << fixed << totalChangeOwed << " - ";

		if (dollarsOwed > 1) { // if the customer is owed more than one dollar
			cout << dollarsOwed << " dollars"; // print dollar with an s
			if (quartersOwed >= 1 || dimesOwed >= 1 || nickelsOwed >= 1 || penniesOwed >= 1) {
				// if more change needs to be given, print a comma
				cout << ", ";
			}
		}
		else if (dollarsOwed == 1) { // if the customer is owed exactly one dollar
			cout << dollarsOwed << " dollar"; // do not add an s
			if (quartersOwed >= 1 || dimesOwed >= 1 || nickelsOwed >= 1 || penniesOwed >= 1) {
				cout << ", ";
			}
		}

		if (quartersOwed > 1) {  // if the customer is owed more than one quarter
			cout << quartersOwed << " quarters"; // print quarter with an s
			if (dimesOwed >= 1 || nickelsOwed >= 1 || penniesOwed >= 1) {
				cout << ", ";
			}
		}
		else if (quartersOwed == 1) { // if the customer is owed exactly one quarter
			cout << quartersOwed << " quarter"; // do not add an s
			if (dimesOwed >= 1 || nickelsOwed >= 1 || penniesOwed >= 1) {
				cout << ", ";
			}
		}

		if (dimesOwed > 1) {  // if the customer is owed more than one dime
			cout << dimesOwed << " dimes"; // print dime with an s
			if (nickelsOwed >= 1 || penniesOwed >= 1) {
				cout << ", ";
			}
		}
		else if (dimesOwed == 1) { // if the customer is owed exactly one dime
			cout << dimesOwed << " dime"; // do not add an s
			if (nickelsOwed >= 1 || penniesOwed >= 1) {
				cout << ", ";
			}
		}

		if (nickelsOwed > 1) {  // if the customer is owed more than one nickel
			cout << nickelsOwed << " nickels"; // print nickel with an s
			if (penniesOwed >= 1) {
				cout << ", ";
			}
		}
		else if (nickelsOwed == 1) { // if the customer is owed exactly one nickel
			cout << nickelsOwed << " nickel"; // do not add an s
			if (penniesOwed >= 1) {
				cout << ", ";
			}
		}

		if (penniesOwed > 1) {  // if the customer is owed more than one penny
			cout << penniesOwed << " pennies"; // print penny with an s
		}
		else if (penniesOwed == 1) { // if the customer is owed exactly one penny
			cout << penniesOwed << " penny"; // do not add an s
		}

		cout << "." << endl;
	}
}