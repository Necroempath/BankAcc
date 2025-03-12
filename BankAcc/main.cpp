#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <chrono>
#include "BankAccount.h"
#include "Customer.h"

enum TransactionType
{
	TOPUP,
	WITHDRAW,
	TRANSFER
};

void WriteTransaction(const size_t balance, const TransactionType type, const size_t amount)
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::tm* now_tm = std::localtime(&now_c);

	FILE* file = fopen("log.txt", "a");

	char typeName[20];

	switch (type)
	{
	case TOPUP:
		strncpy(typeName, "[Top_Up]", sizeof(typeName));
		break;
	case WITHDRAW:
		strncpy(typeName, "[Withdraw]", sizeof(typeName));
		break;
	case TRANSFER:
		strncpy(typeName, "[Transfer]", sizeof(typeName));
		break;
	}

	fprintf(file, "%s%d%c%d%c%d%s", "Date: [", now_tm->tm_mday, '.', now_tm->tm_mon + 1, '.', now_tm->tm_year + 1900, "]  ");
	fprintf(file, "%s%d%c%d%c%d%s", "Time: [", now_tm->tm_hour, ':', now_tm->tm_min, ':', now_tm->tm_sec, "] -- ");
	fprintf(file, "%s%s%s", "Transaction type: ", typeName, "  ");
	fprintf(file, "%s%d%s", "Transaction value: [", amount, "] -- ");
	fprintf(file, "%s%d%s", "Balance: [", balance, "]\n");

	fclose(file);
}

char* ReadLog()
{
	FILE* file = fopen("log.txt", "r");

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* log = new char[size] {};
	char buffer[256]{};

	while (fgets(buffer, sizeof(buffer), file))
	{
		strcat(log, buffer);
	}

	fclose(file);

	return log;
}

void PrintMenu()
{
	std::cout << "\n-----MENU-----\n";
	std::cout << "\n1. New customer";
	std::cout << "\n2. Top up balance";
	std::cout << "\n3. Withdraw from account";
	std::cout << "\n4. Transfer funds";
	std::cout << "\n5. Display info";
	std::cout << "\n6. View log";
	std::cout << "\n7. Exit\n";
}

void FillCustomer(char* name, char* surname, int& balance)
{
	std::cout << "\nEnter name*:\t";
	std::cin >> name;

	std::cout << "\nEnter surname (optional):\t";
	std::cin >> surname;

	std::cout << "\nEnter balance*:\t";
	std::cin >> balance;
}

void FillName(const unsigned short option, char* name, char* surname)
{
	switch (option)
	{
	case 1:
		std::cout << "\nEnter new name:\t";
		std::cin >> name;
		break;
	case 2:
		std::cout << "\nEnter new surname:\t";
		std::cin >> surname;
		break;
	}
}

bool isValidName(char* name)
{
	return strlen(name) > 2;
}

int ValidFields(const char* name, const char* surname, const int balance)
{
	if (strlen(name) > 2 && balance >= 0)
	{
		if (strlen(surname) > 2) return 0;
		else return 1;
	}

	return -1;
}

void PrintEditMenu()
{
	std::cout << "\n1. Change name";
	std::cout << "\n2. Change surname";
	std::cout << "\n3. Return\n";
}

void ProfileEdit(Customer& customer, const unsigned short option, const char* name, char* surname)
{
	switch (option)
	{
	case 1:
		customer.ChangeName(name);
		break;

	case 2:
		customer.ChangeSurname(surname);
		break;
	}
}

int main()
{
	Customer customer;
	BankAccount undefinedAcc;
	char* log;
	bool exit = false;
	int valid_fileds;

	while (!exit)
	{
		PrintMenu();

		short option;
		std::cin >> option;
		std::cin.ignore();

		switch (option)
		{
		case 1:
			char name[20];
			char surname[20];
			int balance;

			FillCustomer(name, surname, balance);

			valid_fileds = ValidFields(name, surname, balance);
			if (valid_fileds >= 0)
			{
				if (valid_fileds > 0) customer = Customer(balance, name);
				else customer = Customer(balance, name, surname);

				std::cout << "\nSuccess: New customer created!\n";
				break;
			}

			std::cout << "\Error: Invalid 'balance' and/or 'name' data!\n";
			break;

		case 2:
			int amount_topUp;
			std::cout << "Enter amount:\t";
			std::cin >> amount_topUp;

			if (amount_topUp <= 0) {
				std::cout << "\nError: Invalid amount data. Amount most be positive integer!\n";
				break;
			}

			if (customer.TopUp(amount_topUp)) std::cout << "\nTransaction successfully completed\n";
			WriteTransaction(customer.GetBalance(), TOPUP, amount_topUp);
			break;

		case 3:
			int amount_withdraw;
			std::cout << "Enter amount:\t";
			std::cin >> amount_withdraw;

			if (amount_withdraw <= 0) {
				std::cout << "\nError: Invalid amount data. Amount most be positive integer!\n";
				break;
			}

			if (customer.Withdraw(amount_withdraw))
			{
				std::cout << "\nTransaction successfully completed\n";
				WriteTransaction(customer.GetBalance(), WITHDRAW, amount_withdraw);
			}
			else std::cout << "\nError: Insufficient funds in account\n";
			break;

		case 4:
			int amount_trans;
			std::cout << "Enter amount:\t";
			std::cin >> amount_trans;

			if (customer.FundsTransfer(undefinedAcc, amount_trans)) {
				std::cout << "\nTransaction succesfully completed\n";
				WriteTransaction(customer.GetBalance(), TRANSFER, amount_trans);
				break;
			}
			else if (!customer.FundsTransfer(undefinedAcc, amount_trans)) {
				std::cout << "\nError: Isufficcient funds in the account\n";
				break;
			}
			else {
				std::cout << "\nError: Impossible transfer to account of self\n";
				break;
			}
			break;

		case 5:
			customer.DisplayInfo();
			PrintEditMenu();

			unsigned short option;
			std::cin >> option;

			if (option >= 3) break;

			char _name[20];
			char _surname[20];

			FillName(option, name, surname);

			if (isValidName(option == 1 ? name : surname)) {
				std::cout << "\nSuccess: " << (option == 1 ? "Name" : "Surname") << " has been changed\n";
				ProfileEdit(customer, option, name, surname);
				break;
			}

			std::cout << "\nError: Invalid " << (option == 1 ? " Name" : "Surname") << " data\n";
			break;

		case 6:
			log = ReadLog();
			std::cout << "\n" << log << "\n";
			delete[] log;
			break;

		case 7:
			exit = true;
			break;

		default:
			std::cout << "\nError: Invalid menu option input.\n";
		}
	}

	return 0;
}

