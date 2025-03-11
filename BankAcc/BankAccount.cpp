#include "BankAccount.h"

BankAccount::BankAccount() : _balance(0) {};

BankAccount::BankAccount(size_t balance) { _balance = balance > 0 ? balance : 0; }

BankAccount::BankAccount(const BankAccount& account) : _balance(account._balance) {}

BankAccount::BankAccount(BankAccount&& account) noexcept : _balance(account._balance) { account._balance = 0; }

BankAccount& BankAccount::operator=(const BankAccount& other)
{
	if (&other == this) return *this;

	_balance = other._balance;
	return *this;
}

BankAccount& BankAccount::operator=(BankAccount&& other) noexcept
{
	if (&other == this) return *this;

	_balance = other._balance;
	other._balance = 0;

	return *this;
}

size_t BankAccount::GetBalance() const { return _balance; }

void BankAccount::TopUp(const size_t amount) { _balance += amount; }

void BankAccount::Withdraw(const size_t amount) { _balance -= amount; }