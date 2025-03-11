#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Customer.h"
#include "BankAccount.h"

Customer::Customer() : _name("Unknown"), _surname("Unknown") {}

Customer::Customer(int balance, char* name, char* surname)
{
	_account.TopUp(balance);
	strncpy(_name, name, sizeof(_name));
	strncpy(_surname, surname, sizeof(_surname));
}

Customer::Customer(int balance, char* name) : _surname("Unknown")
{
	_account.TopUp(balance);
	strncpy(_name, name, sizeof(_name));
}

Customer::Customer(const Customer& customer)
{
	strncpy(_name, customer._name, sizeof(_name));
	strncpy(_surname, customer._surname, sizeof(_surname));
	_account = customer._account;
}

Customer::Customer(Customer&& customer) noexcept
{
	strncpy(_name, customer._name, sizeof(_name));
	strncpy(_surname, customer._surname, sizeof(_surname));
	_account = customer._account;
}

Customer& Customer::operator=(Customer& other)
{
	if (&other == this) return *this;

	Withdraw(GetBalance());
	TopUp(other.GetBalance());
	ChangeName(other._name);
	ChangeSurname(other._surname);

	return *this;
}

Customer& Customer::operator=(Customer&& other) noexcept
{
	if (&other == this) return *this;

	Withdraw(GetBalance());
	other.FundsTransfer(_account, other.GetBalance());
	ChangeName(other._name);
	ChangeSurname(other._surname);

	strncpy(other._name, "Unknown", sizeof(_name));
	strncpy(other._surname, "Unknown", sizeof(_surname));

	return *this;
}

bool Customer::TopUp(const int amount)
{
	if (amount > 0)
	{
		_account.TopUp(amount);
		return 1;
	}

	return 0;
}

bool Customer::Withdraw(const int amount)
{
	bool sufficcientFunds = FundsCheck(amount);

	if (sufficcientFunds)
	{
		_account.Withdraw(amount);
	}

	return sufficcientFunds;
}

bool Customer::FundsCheck(const size_t amount) const { return _account.GetBalance() >= amount; }

size_t Customer::GetBalance() const { return _account.GetBalance(); }

char* Customer::GetFullname() const
{
	const short length = strlen(_name) + strlen(_surname) + 1; //Why it doesn't work by static arr
	char* fullname = new char[length];

	strncpy(fullname, _name, strlen(fullname));
	fullname[strlen(_name)] = ' ';
	strncat(fullname, _surname, strlen(fullname) + 1);

	return fullname;
}

void Customer::ChangeName(char* name) { strncpy(_name, name, sizeof(_name)); }

void Customer::ChangeSurname(char* surname) { strncpy(_surname, surname, sizeof(_surname)); }

int Customer::FundsTransfer(BankAccount& account, int amount)
{
	if (&_account == &account) return -1;

	if (GetBalance() >= amount)
	{
		Withdraw(amount);
		account.TopUp(amount);
		return 1;
	}

	return 0;
}

void Customer::DisplayInfo() const
{
	std::cout << "\nName:\t\t" << _name;
	std::cout << "\nSurname:\t" << _surname;
	std::cout << "\nBalance:\t" << _account.GetBalance() << "\n";
}