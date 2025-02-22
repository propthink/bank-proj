#include <string>
#include <cstdint>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <memory>
#include <unordered_set>
#include <random>

//
std::string generateTimestamp()
{
	// get the current time as a time_point
	auto current_time = std::chrono::system_clock::now();

	// convert the current time to time_t
	auto time_t = std::chrono::system_clock::to_time_t( current_time );

	// create a tm struct to hold the broken-down time
	std::tm tm_struct { };

	// use std::localtime_s to fill the tm struct with local time
	localtime_s( &tm_struct, &time_t );

	// create a stringstream to build the timestamp
	std::ostringstream string_stream;

	string_stream << std::put_time( &tm_struct, "%Y-%m-%d %H:%M:%S" );

	// capture the formatted string from the stringstream
	std::string timestamp_str = string_stream.str();

	return timestamp_str;
}

//
class Transaction
{
	public:

		Transaction( uint32_t account_number, int64_t amount ); // initialize

		void print() const; // print

	private:

		uint32_t m_account_number; //

		int64_t m_amount; //

		std::string m_desc; //

		std::string m_timestamp; //
};

// initialize
Transaction::Transaction( uint32_t account_number, int64_t amount )

	: m_account_number( account_number ), m_amount( amount )
{
	// generate description
	if( amount == 0 )
	{
		// invalid transaction

	} else
		{
			m_desc = ( amount < 0 ) ? "Withdraw" : "Deposit";
		}
	// generate timestamp
	m_timestamp = generateTimestamp();
}

// print
void Transaction::print() const
{
	std::cout << "ACCOUNT #: " << m_account_number;

	std::locale original_locale = std::cout.getloc(); // save current format

	std::cout.imbue( std::locale( "en_US.UTF-8" ) ); // format output

	//std::cout << " | AMOUNT: " << ( m_amount < 0 ? "-$" : "$" )

		//<< std::put_money( std::abs( m_amount ) );

	std::cout << " | AMOUNT: " << ( m_amount < 0 ? "$(" : "$" )
		
		<< std::put_money( std::abs( m_amount ) )
		
		<< ( m_amount < 0 ? ")" : "" );

	std::cout.imbue( original_locale ); // restore the original format

	std::cout << " | DESC: " << m_desc;

	std::cout << " | TIME: " << m_timestamp << '\n';
}

//
class TransactionNode
{
	public:

		TransactionNode( const Transaction& transaction ); // initialize

		std::unique_ptr< Transaction > m_transaction; //

		std::unique_ptr< TransactionNode > m_next; //

		TransactionNode* m_prev; //
};

// initialize
TransactionNode::TransactionNode( const Transaction& transaction )

	: m_transaction( std::make_unique< Transaction >( transaction ) ),

	m_next( nullptr ), m_prev( nullptr )
{
	//
}

//
class TransactionHistory
{
	public:

		TransactionHistory(); // initialize

		// add a new transaction at the end of the list
		void addTransaction( const Transaction& transaction );

		// print the entire transaction history
		void print() const;

	private:

		std::unique_ptr< TransactionNode > m_head; // head of the list

		TransactionNode* m_tail; // tail of the list
};

// initialize
TransactionHistory::TransactionHistory()

	: m_head( nullptr ), m_tail( nullptr )
{
	//
}

// add a new transaction at the end of the list
void TransactionHistory::addTransaction( const Transaction& transaction )
{
	// initialize new transaction node
	std::unique_ptr< TransactionNode > new_node =

		std::make_unique< TransactionNode >( transaction );

	if( !m_tail ) // if the list is empty
	{
		m_head = std::move( new_node ); // m_head takes ownership

		m_tail = m_head.get(); // m_tail points to the last node

	} else
		{
			m_tail -> m_next = std::move( new_node ); // move ownership to m_next

			m_tail -> m_next -> m_prev = m_tail; // set previous pointer

			m_tail = m_tail -> m_next.get(); // update tail
		}
}

// print the entire transaction history
void TransactionHistory::print() const
{
	// check if the list is empty
	if( !m_head )
	{
		// there is nothing to print
		return;
	}
	// start from the head of the list
	TransactionNode* current_transaction = m_head.get();

	// iterate through the list and print each transaction
	while( current_transaction )
	{
		current_transaction -> m_transaction -> print(); // print transaction details

		current_transaction = current_transaction -> m_next.get(); // step
	}
}

//
static std::unordered_set< uint32_t > generated_account_numbers;

//
uint32_t generateAccountNumber()
{
	std::random_device random_device; //

	std::mt19937 random_generator( random_device() ); //

	std::uniform_int_distribution<> rng_distribution( 100000000, 999999999 ); //

	uint32_t new_account_number;

	do {

		new_account_number = rng_distribution( random_generator ); //

	} while( generated_account_numbers.find( new_account_number )

		!= generated_account_numbers.end() ); //

	generated_account_numbers.insert( new_account_number ); //

	return new_account_number;
}

//
class IAccount
{
	public:
	
		virtual ~IAccount() = default; // deallocate

		virtual uint32_t getAccountNumber() const = 0; //

		virtual int64_t getAccountBalance() const = 0; //
};

//
class BAccount : public IAccount
{
	public:

		BAccount( uint32_t account_number, int64_t current_balance = 0,
			
			TransactionHistory&& transaction_history = TransactionHistory() ); //

		virtual ~BAccount(); //

		virtual uint32_t getAccountNumber() const override; //

		virtual int64_t getAccountBalance() const override; //

	private:

		uint32_t m_account_number; //

		int64_t m_current_balance; //

		TransactionHistory m_transaction_history {}; //

		// link to the user
};

// initialize
BAccount::BAccount( uint32_t account_number, int64_t current_balance,

	TransactionHistory&& transaction_history )

	: m_account_number( account_number ),

	m_current_balance( current_balance ),

	m_transaction_history( std::move( transaction_history ) ) { }

//
BAccount::~BAccount() { }

//
uint32_t BAccount::getAccountNumber() const
{
	return m_account_number;
}

//
int64_t BAccount::getAccountBalance() const
{
	return m_current_balance;
}

//
int main()
{
	//
}