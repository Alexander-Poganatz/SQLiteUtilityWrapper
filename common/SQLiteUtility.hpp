#if !defined SQLITEUTILITY_HPP
#define SQLITEUTILITY_HPP
//Do whatever include
#include "sqlite3.h"
#include <string>
#include <map>
#include <vector>

namespace apoganatz
{
	/*
	Struct name: SQLSelectData
	Purpose: Provide a struct for SQLite select callback data.
	Date: January 10, 2017
	Author: Alexander Poganatz
	*/
	template<class string_type>
	struct SQLSelectData
	{
		std::vector<string_type> coloumnNames;
		std::vector<std::vector<string_type>> coloumnValues;
	};
	/*
	Class name: SQLite
	Purpose: To provide safe and easy to use c++ code around the sqlite engine
	Author: Alexander Poganatz
	Date: January 10
	*/
	class SQLite
	{
	public:
		using string_type = std::string;
		//The data type for returning data. It is a map with the key being the coloumn name
		//and the list being the values for the coloumn.
		using select_map = SQLSelectData<string_type>;
	private:
		sqlite3 *dbPointer;
		bool dbIsOpen;
		int operationIntFlag;
		bool lastOperationSuccessfull;
		string_type privateCurrentErrorMessage;

	public:
		SQLite() = delete;
		SQLite(string_type const& path);
		SQLite(SQLite const &) = delete;
		~SQLite();

		//Check if the data base is open, it is set in the constructor.
		inline bool isOpen() { return dbIsOpen; }
		//Checks if the last operation is successful.
		inline bool lastOperationIsSuccessful() { return lastOperationSuccessfull; }
		//Return the current error message.
		inline string_type currentErrorMessage() { return privateCurrentErrorMessage; }
		//Return a map of data.
		select_map* select(string_type const& sql);
		//Execute sql and return if is successful.
		bool executeSQL(string_type const& sql);

		SQLite& operator =(SQLite const&) = delete;
	private:
		static int select_callback(void* data, int argc, char** argv, char **colNames);
	};


}


#endif