#pragma once
class BankAccount
{
	size_t _balance;

public:
	BankAccount();

	explicit BankAccount(size_t balance);

	BankAccount(const BankAccount& account);

	BankAccount(BankAccount&& account) noexcept;

	BankAccount& operator=(const BankAccount& other);

	BankAccount& operator=(BankAccount&& other) noexcept;

	size_t GetBalance() const;

	void TopUp(const size_t amount);

	void Withdraw(const size_t amount);
};

