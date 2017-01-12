#define BOOST_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include "../common/SQLiteUtility.hpp"
BOOST_AUTO_TEST_CASE(test_test)
{
	BOOST_CHECK_EQUAL(true, true);
}

std::string testDBFilePath = "./testdb.db";
std::string testDBTable = "test_table";

std::string col1 = "col1";
std::string col2 = "col2";

BOOST_AUTO_TEST_CASE(initilize_test)
{
	std::string dbFileName("./testdb.db");
	apoganatz::SQLite db(dbFileName);

	BOOST_CHECK(db.isOpen());
	//Check foreign key constraint.
	BOOST_CHECK(db.lastOperationIsSuccessful());

	std::string createTable = "CREATE TABLE " + testDBTable + "("
		+ "ID INT PRIMARY KEY NOT NULL,"
		+ "Name TEXT NOT NULL,"
		+ "Age INT NOT NULL"
		+ ");";

	BOOST_CHECK_EQUAL(db.executeSQL(createTable), true);
}

BOOST_AUTO_TEST_CASE(test_select)
{
	std::string dbFileName("./Expenditures.db");
	apoganatz::SQLite db(dbFileName);

	std::string sql = "SELECT * FROM Items;";

	apoganatz::SQLite::select_map* data = db.select(sql);

	delete data;

	sql = "bad sql";
	data = db.select(sql);
	BOOST_CHECK_EQUAL(db.lastOperationIsSuccessful(), false);
	delete data;
}

BOOST_AUTO_TEST_CASE(delete_test_DB)
{
	apoganatz::SQLite db(testDBFilePath);

	std::string deleteTable = "drop table " + testDBTable + ";";

	BOOST_CHECK_EQUAL(db.executeSQL(deleteTable), true);
}

