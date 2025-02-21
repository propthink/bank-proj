#include <string>
#include <cstdint>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <memory>

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

	std::cout.imbue( std::locale( "en_US.UTF-8" ) ); // format output

	std::cout << " | AMOUNT: " << ( m_amount < 0 ? "-$" : "$" )

		<< std::put_money( std::abs( m_amount ) );

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

		std::unique_ptr < TransactionNode > m_prev; //
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
	//
};

//
class IAccount
{
	//
};

//
class BAccount : public IAccount
{
	//
};

//
class CheckingAccount : public BAccount
{
	//
};

//
class SavingsAccount : public BAccount
{
	//
};

//
class AccountNode
{
	//
};

//
class AccountRegistry
{
	//
};

//
class UserProfile
{
	//
};

//
int main()
{
	//
}