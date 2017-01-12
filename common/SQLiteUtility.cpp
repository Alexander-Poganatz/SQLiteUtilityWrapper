//Do whatever include for your 
#include "SQLiteUtility.hpp"

namespace apoganatz
{
	SQLite::SQLite(SQLite::string_type const & path)
	{
		operationIntFlag = sqlite3_open(path.c_str(), &dbPointer);

		dbIsOpen = SQLITE_CANTOPEN != operationIntFlag;

		if (dbIsOpen)
		{
			privateCurrentErrorMessage = "";
			sqlite3_stmt* statement;
			operationIntFlag = sqlite3_prepare_v2(dbPointer, "PRAGMA foreign_keys = ON;", -1, &statement, 0);

			//This is usefull to make sure the statement worked during unit testing.
			lastOperationSuccessfull = SQLITE_OK == operationIntFlag;

			sqlite3_finalize(statement);
		}
		else
		{
			privateCurrentErrorMessage = path + " was unable to be opened.";
		}
	}

	SQLite::~SQLite()
	{
		sqlite3_close(dbPointer);
	}

	int SQLite::select_callback(void* data, int argc, char** argv, char **colNames)
	{
		select_map* mapData = (select_map*)data;

		//Only add coloumns once.
		if (mapData->coloumnNames.empty())
		{
			for (int i = 0; i < argc; ++i)
			{
				mapData->coloumnNames.push_back(colNames[i]);
				mapData->coloumnValues.push_back(std::vector<string_type>());
			}
		}
				

		for (int i = 0; i < argc; ++i)
		{
			mapData->coloumnValues[i].push_back(argv[i] ? argv[i] : "NULL");
		}
			//(*((select_map*)data))[colNames[i]].push_back(argv[i] ? argv[i] : "NULL");

		return 0;
	}

	SQLite::select_map* SQLite::select(string_type const& sql)
	{
		select_map* dataToReturn = new select_map;
		char* errorMessage = nullptr;
		operationIntFlag = sqlite3_exec(dbPointer, sql.c_str(), select_callback, (void*)dataToReturn, &errorMessage);
		if (operationIntFlag != SQLITE_OK)
		{
			privateCurrentErrorMessage.clear();
			privateCurrentErrorMessage.append(errorMessage);
			sqlite3_free(errorMessage);
			lastOperationSuccessfull = false;
		}

		return dataToReturn;
	}

	inline int execute_callback(void* data, int argc, char** argv, char **colNames) { return 0; }

	bool SQLite::executeSQL(string_type const& sql)
	{
		char* tempData = new char('a');

		char* errorMessage = nullptr;
		
		operationIntFlag = sqlite3_exec(dbPointer, sql.c_str(), select_callback, (void*)tempData, &errorMessage);
		if (operationIntFlag != SQLITE_OK)
		{
			privateCurrentErrorMessage.clear();
			privateCurrentErrorMessage.append(errorMessage);
			sqlite3_free(errorMessage);
			lastOperationSuccessfull = false;
		}

		delete tempData;
		//temp
		return true;
	}
}

