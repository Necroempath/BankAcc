#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <chrono>

class BankAccount
{
	size_t _balance;

public:
	BankAccount() : _balance(0) {};

	explicit BankAccount(size_t balance) { _balance = balance > 0 ? balance : 0; }

	BankAccount(BankAccount& account) : _balance(account._balance) {}

	BankAccount(BankAccount&& account) : _balance(account._balance) { account._balance = 0; }

	size_t GetBalance() const { return _balance; }

	void TopUp(const size_t amount) { _balance += amount; }

	void Withdraw(const size_t amount) { _balance -= amount; }
};

class Customer
{
	BankAccount _account;
	char _name[20];
	char _surname[20];

	void TransactionLog(const short type, const size_t amount) const
	{
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::tm* now_tm = std::localtime(&now_c);

		FILE* file = fopen("log.txt", "a");

		fprintf(file, "%s%d%c%d%c%d%s", "Date: [", now_tm->tm_mday, '.', now_tm->tm_mon + 1, '.', now_tm->tm_year + 1900, "]  ");
		fprintf(file, "%s%d%c%d%c%d%s", "Time: [", now_tm->tm_hour, ':', now_tm->tm_min, ':', now_tm->tm_sec, "] -- ");
		fprintf(file, "%s%s%s", "Transaction type: ", type ? "[Withdraw]" : "[Top up]", "  ");
		fprintf(file, "%s%d%s", "Transaction value: [", amount, "] -- ");
		fprintf(file, "%s%d%s", "Balance: [", _account.GetBalance(), "]\n");

		fclose(file);
	}

public:
	Customer() : _name("Unknown"), _surname("Unknown") {}

	Customer(int balance, char* name, char* surname)
	{
		_account.TopUp(balance);
		strncpy(_name, name, sizeof(_name));
		strncpy(_surname, surname, sizeof(_surname));
	}

	explicit Customer(int balance, char* name) : _surname("Unknown")
	{
		_account.TopUp(balance);
		strncpy(_name, name, sizeof(_name));
	}

	Customer(Customer& customer)
	{
		strncpy(_name, customer._name, sizeof(_name));
		strncpy(_surname, customer._surname, sizeof(_surname));
	}

	bool TopUp(const int amount) 
	{ 
		if (amount > 0)
		{
			_account.TopUp(amount);
			TransactionLog(0, amount);
			return 1;
		}

		return 0;
	}

	bool Withdraw(const int amount)
	{
		if (FundsCheck(amount))
		{
			_account.Withdraw(amount);
			TransactionLog(1, amount);
		}

		return FundsCheck(amount);
	}

	bool FundsCheck(const size_t amount) const { return _account.GetBalance() >= amount; }

	size_t GetBalance() const { return _account.GetBalance(); }

	char* GetFullname() const
	{
		const short length = strlen(_name) + strlen(_surname) + 1;
		char* fullname = new char[length];

		strncpy(fullname, _name, strlen(fullname));
		fullname[strlen(_name)] = ' ';
		strncat(fullname, _surname, strlen(fullname) + 1);

		return fullname;
	}

	void ChangeName(char* name) { strncpy(_name, name, sizeof(_name)); std::cout << _name; }

	void ChangeSurname(char* surname) { strncpy(_surname, surname, sizeof(_surname)); }

	bool FundsTransfer(BankAccount account, int amount) 
	{ 
		if (account.GetBalance() >= amount) 
		{
			account.Withdraw(amount);
			this->TopUp(amount);
			return 1;
		}

		return 0;
	}

	void DisplayInfo() const
	{
		std::cout << "\nName:\t\t" << _name;
		std::cout << "\nSurname:\t" << _surname;
		std::cout << "\nBalance:\t" << _account.GetBalance() << "\n";
	}
};

void PrintMenu()
{
	std::cout << "\n-----MENU-----\n";
	std::cout << "\n1. New customer";
	std::cout << "\n2. Top up balance";
	std::cout << "\n3. Withdraw from account";
	std::cout << "\n4. Transfer funds";
	std::cout << "\n5. Display info";
	std::cout << "\n6. Exit\n";
}

Customer CreateCustomer()
{
	char name[20], surname[20];
	int balance;
	bool validName = false, validSurname = false, validBalance = false;

	std::cout << "\nEnter name*:\t";
	std::cin >> name;

	if (strlen(name) > 2) validName = true;

	std::cout << "\nEnter surname (optional):\t";
	std::cin >> surname;

	if (strlen(surname) > 2) validSurname = true;


	std::cout << "\nEnter balance*:\t";
	std::cin >> balance;

	if (balance >= 0) validBalance = true;

	if (validName && validBalance) {

		std::cout << "\nCustomer successfully created\n";

		if (validSurname) {

			Customer customer(balance, name, surname);
			return customer;
		}

		Customer customer(balance, name);
		return customer;
	}

	std::cout << "\nError: Invalid name and/or balance data!\n";
}

void ProfileEdit(Customer customer)
{
	std::cout << "\n1. Change name";
	std::cout << "\n2. Change surname";
	std::cout << "\n3. Return\n";

	short option;
	std::cin >> option;
	std::cin.ignore();

	switch(option)
	{
		case 1:
			char name[20];
			std::cout << "\nEnter name:\t";
			std::cin >> name;

			if (strlen(name) < 3) {
				std::cout << "\nError: Name length must be more than 3 chars!";
				break;
			}

			customer.ChangeName(name);
			break;

		case 2:
			char surname[20];
			std::cout << "\nEnter surname:\t";
			std::cin >> surname;

			if (strlen(surname) < 3) {
				std::cout << "\nError: Surname length must be more than 3 chars!";
				break;
			}

			customer.ChangeSurname(surname);
			break;

		default:
			break;
	}
}

int main()
{
	Customer customer;
	bool exit = false;

	while (!exit) 
	{
		PrintMenu();

		short option;
		std::cin >> option;
		std::cin.ignore();

		switch (option)
		{
		case 1:
			CreateCustomer();
			break;

		case 2:
			int amount_topUp;
			std::cout << "Enter amount:\t";
			std::cin >> amount_topUp;

			if (amount_topUp <= 0) {
				std::cout << "\nError: Invalid amount data. Amount most be posotive integer!\n";
				break;
			}

			if (customer.TopUp(amount_topUp)) std::cout << "\nTransaction successfully completed\n";
			break;

		case 3:
			int amount_withdraw;
			std::cout << "Enter amount:\t";
			std::cin >> amount_withdraw;

			if (amount_withdraw <= 0) {
				std::cout << "\nError: Invalid amount data. Amount most be posotive integer!\n";
				break;
			}

			if (customer.Withdraw(amount_withdraw)) std::cout << "\nTransaction successfully completed\n";
			else std::cout << "\nError: Insufficient funds in account\n";
			break;

		case 4:
			//BankAccount acc(100);

			//int amount_trans;
			//std::cout << "Enter amount:\t";
			//std::cin >> amount_trans;

			//customer.FundsTransfer(acc, amount_trans);

		case 5:
			customer.DisplayInfo();
			ProfileEdit(customer);
			break;

		case 6:
			exit = true;
			break;
		default:
			std::cout << "\nError: Invalid menu option input.\n";
		}
	}
	
	return 0;
}

