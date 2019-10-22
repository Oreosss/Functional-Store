//------------------------------------------------------------------------------------------------------------------------
//                                             CS215--003 PROJECT 2
//------------------------------------------------------------------------------------------------------------------------
//Author:Oreoluwa Olukotun
//Date:10/6/2019
//Description: This program simulates an online store
// Assistance: I recieved help from the following:
//		Eli Rice - helped me with creating some of the functions.		
//------------------------------------------------------------------------------------------------------------------------




#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include <Math.h>
#include <fstream>
using namespace std;

const int READ_ERROR = -1;
const int MAX_INV_ITEMS = 10;
const int MAX_ORDERS = 7;
const int MAX_ORDER_ITEMS = 5;

//----------------------------------------------------------------------------------
//                                         STRUCTS
//-------------------------------------------------------------------------------------
// describes a single item in the inventory, and on an order
struct item {
	string prodCode;		// product code: length 12, no spaces
	string description;		// product description: max length 28, has spaces
	double price;			// price of the product, max 999.99
};

// describes a customer order or "basket"
struct order {
	long   orderNumber;		// unique order number for this order
	string custName;		// customer name
	double totalPrice;		// price of all items purchased
	item items[MAX_ORDER_ITEMS];// list of items purchased
	int numItems;			// number of items purchased
};

//--------------------------------------------------FUNCTIONS-------------------------------------

//--------------------------------------------------------------------------------------------
//                                       ReadInventory 
//---------------------------------------------------------------------------------------------
void readInventory(item inv[], int & numberOfInvItems, int & lastOrderNum) {
	ifstream f; //reads input file

	// open the inventory file
	f.open("inventory.txt");
	if (f.fail()) {
		cout << "readFile:: error opening inventory.txt\n";
		numberOfInvItems = READ_ERROR;
		return;
	}

	// read number of items from first line
	f >> numberOfInvItems >> lastOrderNum;

	// for each item, read the data for the item
	for (int i = 0; i < numberOfInvItems; i++) {
		f >> inv[i].prodCode >> inv[i].price;
		f.ignore(); // finished reading integer, getline() on string is next
		getline(f, inv[i].description);
	}
	f.close(); // closes file
} // readInventory()

//--------------------------------------------------------------------------------------------
//                                       IsValidOption
//---------------------------------------------------------------------------------------------
bool IsValidOption(char FirstLetter, string valOpts)
{
	for (int i = 0; i < valOpts.length(); i++) //loops for as long as string 'valOpts is
	{
		if (valOpts[i] == FirstLetter) // checks if firstletter is the same as one of the valid options
		{
			return true;
		}
		
	}
	return false;
}// end IsValidOption

//--------------------------------------------------------------------------------------------
//                                       getMainOption 
//getMainOption function (includes validation loop, logo and set user entry)
//---------------------------------------------------------------------------------------------
char getMainOption()
{
	string UserEntry; 
	char FirstLetter;
	string valOpts = "IOLX"; // valid options
	

	// prints logo
	cout << "+-----------------------------------------------+" << endl;
	cout << "+             OREO'S ONLINE STORE               +" << endl;
	cout << "+            By Oreoluwa Olukotun               +" << endl;
	cout << "+-----------------------------------------------+" << endl;
	
	// prints options
	cout << "I - List our Inventory  " << endl;
	cout << "O - Make an Order       " << endl;
	cout << "L - List all orders made" << endl;
	cout << "X - Exit                " << endl;
	cout << "Enter an option: ";
	if (cin.peek() == '\n') cin.ignore(); // ignores spaces if user inputs a string with apaces
	cin >> UserEntry;
	FirstLetter = toupper(UserEntry[0]); //capitalizes first letter of the string and converts to char

	IsValidOption(FirstLetter, valOpts);

	while (!IsValidOption(FirstLetter, valOpts))
	{
		cout << "Invalid option. Enter I, O, L or X!" << endl;
		cout << "Enter an option: ";
		cin >> UserEntry;
		FirstLetter = toupper(UserEntry[0]);
	}

	return FirstLetter; // passed to main
}//end getMainOption


//--------------------------------------------------------------------------------------------
//                                       displayList 
// prints out list of items, including array num, product num, price
//---------------------------------------------------------------------------------------------

void displayList(item inv[], int  numberOfInvItems) // when given just partial array always include variable that shows how many items are in the list
{
	// prints out list read from file
	for (int i = 0; i < numberOfInvItems; i++)
	{
		cout << right << fixed << setw(3) << i << "  " ; 
		cout << left << fixed << setw(12) << inv[i].prodCode << "  " << "$";
		cout << right << fixed << setw(6) << setprecision(2) << inv[i].price << "  ";
		cout << inv[i].description << endl;
	}
} // end displayList()


//--------------------------------------------------------------------------------------------
//                                       displayInventory 
//prints out product logo & calls on display list function to print out inventory when option is 'I'
//---------------------------------------------------------------------------------------------

void displayInventory(item inv[], int  numberOfInvItems) // prints out logo and calls on display list to show products
{
	//prints out logo
	cout << "\n";
	cout << "+--------------------------------------------------------+" << endl;
	cout << "+                         Products                       +" << endl;
	cout << "+--------------------------------------------------------+" << endl;
	cout << " #   PRODUCT CODE   PRICE   PRODUCT DESCRIPTION" << endl;
	cout << "---  ------------  -------  ------------------------------" << endl;

	displayList(inv, numberOfInvItems); // invokes display list function
	cout << "Number of items in inventory: " << numberOfInvItems << endl;
	cout << "\n";

}// end displayInventory()


//--------------------------------------------------------------------------------------------
//                                      displayOrder
// prints out order number and customer
//---------------------------------------------------------------------------------------------
void displayOrder(order Orders[], int numOrders)
{
	//prints out order number and  customers name
	cout << "ORDER: " << Orders[numOrders].orderNumber << "  " << Orders[numOrders].custName << endl;

	displayList(Orders[numOrders].items, Orders[numOrders].numItems); // passes items and number of items . [Can add one to make sure index number displayed on order list starts from 1]

	cout << " Total             $" << right << fixed << setw(6) << setprecision(2) << Orders[numOrders].totalPrice << "  " << endl;

	
	
} // end displayOrder()

//--------------------------------------------------------------------------------------------
//                                      startOrder
//initializes a basket 
//---------------------------------------------------------------------------------------------
void startOrder(order Orders[], int & numOrders, int & lastOrderNum) //ampere sign with those variables because function modifies it PBR
{
	
	++lastOrderNum; // increments order number for each transaction
	++numOrders; // increments the number of orders in the array of orders
	Orders[numOrders].orderNumber = lastOrderNum; // assigns order number of basket to the last order number
	Orders[numOrders].numItems = 0; // initializes number of items in basket
	Orders[numOrders].totalPrice = 0.0; // initializes total price
	
	cout << "Order Number:        " << Orders[numOrders].orderNumber << endl;
	cout << "Enter customer name: ";
	cin >> Orders[numOrders].custName; // assigns user entry to customer name 
	

}// end StartOrder()

//--------------------------------------------------------------------------------------------
//                                     orderItem
// handles ordering of a single item . by adding items to a basket and keeping track of total price
//---------------------------------------------------------------------------------------------
bool orderItem(item inv[], int  numberOfInvItems, order Orders[], int & numOrders)
{
	Orders[numOrders].numItems = 0; // this is the index for  number of items
	int i=0;
	cout << "Enter item number <Enter -1 to end> : "; // getting user entry for item on list
	cin >> i; 
	Orders[numOrders].totalPrice = 0.0;

	// validation loop. validates user entry to ensure between <-1 & numofInvitems>
	while ((i < READ_ERROR) || (i >= numberOfInvItems)) 
	{
		cout << "Invalid entry. Enter number -1 to " << (numberOfInvItems-1) << endl;
		cout << "Enter item number <Enter -1 to end> : ";
		cin >> i;
	}
	

	if (i == READ_ERROR) // checks if user wants to quit
	{
		return true;
		
	}
		
	else 
	{
		
		
			while (Orders[numOrders].numItems < MAX_ORDER_ITEMS) // ensures user does not have more than max order items in basket
			{
				// assigns selected 'item in inv' members to 'item in basket' members
				Orders[numOrders].items[Orders[numOrders].numItems].price = inv[i].price;
				Orders[numOrders].items[Orders[numOrders].numItems].prodCode = inv[i].prodCode;
				Orders[numOrders].items[Orders[numOrders].numItems].description = inv[i].description;
				Orders[numOrders].totalPrice += inv[i].price;

				cout << Orders[numOrders].items[Orders[numOrders].numItems].description << " added to your basket. Current total is $ " << Orders[numOrders].totalPrice << endl;

				Orders[numOrders].numItems++; // increases the items in the cart



				cout << "Enter item number <Enter -1 to end> : ";
				cin >> i;

				while ((i < READ_ERROR) || (i >= numberOfInvItems)) // equal to or greater than so it excludes 4
				{
					cout << "Invalid entry. Enter number -1 to " << (numberOfInvItems - 1) << endl;
					cout << "Enter item number <Enter -1 to end> : ";
					cin >> i;
				}

				if (i == READ_ERROR) // so it catches the -1 while in the loop
				{
					return true;

				}
			}// end while loop 
		

		cout << "Sorry, your cart is full. The maximum number of items per order is " << MAX_ORDER_ITEMS << endl; // prints out when the cart is full
		cout << endl;
		return true;
		
		
	}

		return false;
} 

//--------------------------------------------------------------------------------------------
//                                       makeOrder
// calls on all functions that hep to create an order 
//---------------------------------------------------------------------------------------------
void makeOrder(item inv[], int  numberOfInvItems, order Orders[], int & numOrders, int & lastOrderNum)
{


	if (numOrders >= (MAX_ORDERS - 1)) // checks to ensure user has not made more than max orders. (max_orders -1 ) because initializing of numOrder starts from 0
	{
		cout << "\n";
		cout << "Sorry, we can not take more orders today." << endl;
		cout << "\n";
	}

	else
	{
		startOrder(Orders, numOrders, lastOrderNum); // invokes startOrder
		displayInventory(inv, numberOfInvItems); // invokes display inventory
		orderItem(inv, numberOfInvItems, Orders, numOrders);// invokes numorders)
		cout << "Thank you for your order" << endl;
		cout << "\n";

		displayOrder(Orders, numOrders);//invokes displayorder
		cout << "\n";
		system("pause");
	}
	
	
	
}
		
//--------------------------------------------------------------------------------------------
//                                       listOrders
// prints out all orders made by calling on displayList function
//---------------------------------------------------------------------------------------------
void listOrders(order Orders[], int numOrders)
{
	cout << "\n";
	//prints logo
	cout << "+--------------------------------------------------------+" << endl;
	cout << "+                         Orders                         +" << endl;
	cout << "+--------------------------------------------------------+" << endl;

	

	if (numOrders == 0) // when no order has been made
	{
		cout << " Total Number of Orders  = " << numOrders << endl;
		cout << "\n";
	}

	else
	{
		for (int i = 1; i <= numOrders; i++) // starts from one cause theres no order in index 0
		{
			displayOrder(Orders, i); // calling on display order for each order in the orders array
			cout << "\n";
		}

		

		cout << " Total Number of Orders  = " << numOrders << endl;
		cout << "\n";
	}

}

//--------------------------------------------------------------------------------------------
//                                      writeOrders
// writes out all orders to a txt file.
//---------------------------------------------------------------------------------------------
void writeOrders(order Orders[], int numOrders)
{
	
	ofstream fout;

	fout.open("orders.txt"); //opens file

	if (fout.fail()) // checks to see if file opened successfully
	{
		cout << "readFile:: error opening inventory.txt\n";
		return;
	}

	else 
	{
		fout << numOrders << endl; // prints out numOrders on first line

		for (int i = 1; i <= numOrders; i++) // loop for number of orders made
		{
			fout << Orders[i].orderNumber << "  " << Orders[i].numItems << "  " << Orders[i].custName << endl;
			for (int j = 0; j < Orders[i].numItems; j++) // loop for number of items in orders
			{
				fout << left << fixed << setw(12) << Orders[i].items[j].prodCode << "  " // i represents Order Number , j represents item number
					 << right << fixed << setw(6) << setprecision(2) << Orders[i].items[j].price << "  "
					 << Orders[i].items[j].description << endl;
			}
		}
	}
	fout.close();
}

//--------------------------------------------------------------------------------------------
//                                       main
//calls on functions based on users entry
//---------------------------------------------------------------------------------------------
int main()
{
	item inv[MAX_INV_ITEMS]; // partial array inventory
	order Orders[MAX_ORDERS]; // partial array order
	int numberOfInvItems = 0; // number of inventory items read from input file
	int numOrders = 0; // number of Orders in the array of Orders
	int lastOrderNum = 0; // gotten from file , last order number made previous time program was executed
	char Option;



	readInventory(inv, numberOfInvItems, lastOrderNum); // populates inventory list
	Option = getMainOption();



	while (Option != 'X')
	{

		if (Option == 'I') // displays inventory
		{
			displayInventory(inv, numberOfInvItems);
		}

		if (Option == 'O') // makes order
		{
			makeOrder(inv, numberOfInvItems, Orders, numOrders, lastOrderNum);

		}

		if (Option == 'L') // lists all orders made
		{
			listOrders(Orders, numOrders);


		}

	   Option = getMainOption();
	}

	if (Option == 'X') // writes all orders made to a file
	{
		writeOrders(Orders, numOrders);
		cout << "Your file has been written to orders.txt" << endl;
		cout << "\n"; 
		cout << "Thank you for shopping with us " << endl;
	}

	

	system ("pause");
	return (0);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//NOTES FOR ME:
// For variables that will be modified in a function be sure to pass by reference that is include ampere '&' sign when passing

