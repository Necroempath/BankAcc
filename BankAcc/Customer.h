#pragma once
#include "BankAccount.h"

class Customer
{
	BankAccount _account;
	char _name[20];
	char _surname[20];

public:

	Customer();

	Customer(int balance, char* name, char* surname);

	explicit Customer(int balance, char* name);

	Customer(const Customer& customer);

	Customer(Customer&& customer) noexcept;

	Customer& operator=(Customer& other);

	Customer& operator=(Customer&& other) noexcept;

	bool TopUp(const int amount);

	bool Withdraw(const int amount);

	bool FundsCheck(const size_t amount) const;

	size_t GetBalance() const;

	char* GetFullname() const;

	void ChangeName(char* name);

	void ChangeSurname(char* surname);

	int FundsTransfer(BankAccount& account, int amount);

	void DisplayInfo() const;

};

