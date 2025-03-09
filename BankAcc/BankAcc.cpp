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

	Customer(size_t balance, char* name, char* surname)
	{
		_account.TopUp(balance);
		strncpy(_name, name, sizeof(_name));
		strncpy(_surname, surname, sizeof(_surname));
	}

	explicit Customer(size_t balance, char* name) : _surname("Unknown")
	{
		_account.TopUp(balance);
		strncpy(_name, name, sizeof(_name));
	}

	Customer(Customer& customer)
	{
		strncpy(_name, customer._name, sizeof(_name));
		strncpy(_surname, customer._surname, sizeof(_surname));
	}

	bool TopUp(const size_t amount) 
	{ 
		if (amount > 0)
		{
			_account.TopUp(amount);
			TransactionLog(0, amount);
			return 1;
		}

		return 0;
	}

	bool Withdraw(const size_t amount)
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

	void ChangeName(char* name) { strncpy(_name, name, sizeof(_name)); }

	void ChangeSurname(char* surname) { strncpy(_surname, surname, sizeof(_surname)); }

	bool FundsTransfer(BankAccount account, size_t amount) 
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
	std::cout << "\n4. Display info";
	std::cout << "\n5. Exit\n";
}

Customer CreateCustomer()
{
	char name[20], surname[20];
	size_t balance;
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

		if (validSurname) {

			Customer customer(balance, name, surname);
			return customer;
		}

		Customer customer(balance, name);
		return customer;
	}

	std::cout << "Error: Invalid name and/or balance data!";
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

			size_t amount;
			std::cout << "Enter amount:\t";
			std::cin >> amount;

			if (amount <= 0) {
				std::cout << "\nError: Invalid amount data. Amount most be posotive integer!\n";
				break;
			}

			if (customer.TopUp(amount)) std::cout << "\nTransaction successfully completed\n";
			break;

		case 3:

			size_t amount;
			std::cout << "Enter amount:\t";
			std::cin >> amount;

			if (amount <= 0) {
				std::cout << "\nError: Invalid amount data. Amount most be posotive integer!\n";
				break;
			}

		case 4:
			customer.DisplayInfo();
			break;
		case 5:
			exit = true;
		default:
			std::cout << "\nError: Invalid menu option input.\n";
		}
	}
	

	char name[20] = "John";
	char surname[20] = "Strong";
	char sur[20] = "Weak";
	char nam[20] = "Clown";
	Customer customer(500, name, surname);

	customer.TopUp(35);


	return 0;
}

