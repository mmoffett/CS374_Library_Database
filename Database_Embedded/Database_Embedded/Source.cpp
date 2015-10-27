#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

#define SQLSERVER


const int MAX_DATA = 100; //will limit the size of the data retrieved, by limiting the number of characters allowed in a data char array
void listTables(int queryNum);
pair<string, string> GetQuery(int choice);
void PrintMenu();
void CallFunctions(int choice);

int numColumns = 0;
vector<string> columnVec;

int main() {
	bool cont = true;
	while (cont == true)
	{
		PrintMenu();
		int k;
		int tempvar = 0;
		cin >> k;
		while (k > 20 /*number of queries*/)
		{
			cout << "Invalid input, try again!" << endl;
			cin >> k;
		}
		char c = NULL;
		while (c != 'n' && c != 'N') {
			if (tempvar == 0) {
				CallFunctions(k);
				cout << "Do You want to complete another query (y/n)?" << endl;
				cin >> c;
				while (c != 'y' && c != 'n' && c != 'N' && c != 'Y')
				{
					cout << "Invalid input, try again!" << endl;
					cin >> c;
				}
				tempvar++;
			}

			else {
				PrintMenu();
				cin >> k;
				while (k > 20 /*number of queries*/)
				{
					cout << "Invalid input, try again!" << endl;
					cin >> k;
				}
				CallFunctions(k);
				cout << "Do You want to complete another query (y/n)?" << endl;
				cin >> c;
				while (c != 'y' && c != 'n' && c != 'N' && c != 'Y')
				{
					cout << "Invalid input, try again!" << endl;
					cin >> c;
				}
				tempvar++;
			}

		}

		if (c == 'n' || c == 'N')
			cont = false;
	}

	return 0;
}
void PrintMenu()
{
	cout << "Choose an Option as a number only: " << endl;
	cout << left << "1:  Add author without publisher" << endl;
	cout << left << "2:  Add author with publisher" << endl;
	cout << left << "3:  Add books" << endl;
	cout << left << "4:  Add user" << endl;
	cout << left << "5: Search for books by author" << endl;
	cout << left << "6: Get the author of a book by title " << endl;
	cout << left << "7:  Search for a book by the title" << endl;
	cout << left << "8: Return Date Due if Book is Checked Out" << endl;
	cout << left << "9: Check Out Book" << endl;
	cout << left << "10: List damages to a book by title" << endl;
	cout << left << "11:  Search for books by genre " << endl;
	cout << left << "12: Give a Fine to a Library User" << endl;
	cout << left << "13:  Get info for user by library card id" << endl;
	cout << left << "14:  Report New Book Damages" << endl;
	cout << left << "15:  Pay a Fine" << endl;
	cout << left << "16: Return a Book to the Library" << endl;
	cout << left << "17: Find all unpaid fines for a user" << endl;
	cout << left << "18: Return all Books Checked Out" << endl;
	cout << left << "19: Return All Library Books" << endl;
	cout << left << "20: Return All Overdue Books" << endl;
	//...menu of options
}
void CallFunctions(int userQuery)
{

	switch (userQuery) {
	case 1:
		listTables(userQuery);
		break;
	case 2:
		listTables(userQuery);
		break;
	case 3:
		listTables(userQuery);
		break;
	case 4:
		listTables(userQuery);
		break;
	case 5:
		listTables(userQuery);
		break;
	case 6:
		listTables(userQuery);
		break;
	case 7:
		listTables(userQuery);
		break;
	case 8:
		listTables(userQuery);
		break;
	case 9:
		listTables(userQuery);
		break;
	case 10:
		listTables(userQuery);
		break;
	case 11:
		listTables(userQuery);
		break;
	case 12:
		listTables(userQuery);
		break;
	case 13:
		listTables(userQuery);
		break;
	case 14:
		listTables(userQuery);
		break;
	case 15:
		listTables(userQuery);
		break;
	case 16:
		listTables(userQuery);
		break;
	case 17:
		listTables(userQuery);
		break;
	case 18:
		listTables(userQuery);
		break;
	case 19:
		listTables(userQuery);
		break;
	case 20:
		listTables(userQuery);
		break;
		/*up to case n, where n exists in the positive integers */
	default: cout << "Invalid selection, please re-select.\n";
	}
}


/*
Function listTables
@param queryNum the query selected by the user to be passed into GetQuery
Prints the query and results of the query specified by the function GetQuery
return is void
*/
void listTables(int queryNum) {
	//sets up all of the objects needed to access a database
	RETCODE rc;
	HENV henv;
	HDBC hdbc;
	HSTMT hstmt;
	char szData[MAX_DATA];
	string stSQL;
	SDWORD cbData;

	SQLAllocEnv(&henv);
	SQLAllocConnect(henv, &hdbc);

	SQLCHAR szConnectOut[1024];
	SQLSMALLINT cchConnect;


#ifdef SQLSERVER //connects us to our server if it can
	string stConnect = "Driver={SQL Server};Server=CS1;";
	stConnect += "Database=Silence_in_the_Library;Trusted_Connection=yes;";
#endif
	pair<string, string> query = GetQuery(queryNum);
	stSQL = query.second; //calls GetQuery to get the string with the SQL query to be used
	string temp = query.first;

	if (temp[0] == '#') {
		// There are no columns, the query inserted something
	}
	else {
		for (int i = 0; i < temp.length(); i++) {
			if (temp[i] == '~') {
				temp[i] = ' ';
			}
		}
	}

	stringstream ss(temp);
	string t;
	while (ss >> t) {
		columnVec.push_back(t);
	}

	numColumns = atoi(columnVec[0].c_str());

	//tries to connect to the database
	rc = SQLDriverConnect(hdbc, NULL, (SQLCHAR *)stConnect.c_str(), stConnect.length(), szConnectOut, 1024, &cchConnect, SQL_DRIVER_NOPROMPT);
	//if the connection was not successful, print an error code and end the program
	if (!(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO))
	{
		SQLTCHAR state[255], error[255];
		SQLINTEGER code;
		SQLSMALLINT cb;
		SQLError(henv, hdbc, NULL, state, &code, error, 255, &cb);
		cout << error << endl;
		exit(0);
	}
	SQLAllocStmt(hdbc, &hstmt);
	cout << stSQL << endl; //print the query text to the console output
	rc = SQLExecDirect(hstmt, (SQLCHAR*)stSQL.c_str(), SQL_NTS); //run the query
	if (!(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)) //if the query failed, print an error message and exit
	{
		SQLTCHAR state[255], error[255];
		SQLINTEGER code;
		SQLSMALLINT cb;
		SQLError(henv, hdbc, hstmt, state, &code, error, 255, &cb);
		cout << error << endl;
		exit(0);
	}



	cout << fixed;
	cout << setprecision(2);

	int curr = 0;
	for (int i = 1; i <= numColumns; i++) {
		if (i == 1) {
			cout << left << setw(40) << columnVec[i];
		}
		else {
			cout << right << setw(40) << columnVec[i];
		}
	}


	cout << endl << endl;
	while (rc == SQL_SUCCESS) {

		rc = SQLFetch(hstmt);
		bool first = true;
		while (curr <= numColumns) {
			if (SQLGetData(hstmt, curr++, SQL_C_CHAR, szData, sizeof(szData), &cbData) == SQL_SUCCESS) {
				if (first) {
					std::cout << std::left << std::setw(20) << szData;
					first = false;
				}
				else {
					std::cout << std::right << std::setw(20) << szData;
				}

			}

		}

		curr = 0;
		std::cout << std::endl;
	}

	std::cout << "\n\n\n\n";

	//Cleanup the complex data structures that were initialized
	SQLFreeStmt(hstmt, SQL_DROP);
	SQLDisconnect(hdbc);
	SQLFreeConnect(hdbc);
	SQLFreeEnv(henv);
}

/*
Function GetQuery
@param choice query selected by user
@return a string containing the SQL for the query

*/
pair<string, string> GetQuery(int choice) { // maybe do some NULL exception handling for not NULL values
	pair<string, string> query;
	query.first = "";
	query.second = "";
	string stSQL;
	string input;
	switch (choice) {
	case 1:
		stSQL = "Add_Author_NoPublisher ";
		cout << "Enter Author First Name" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Author Last Name" << endl;
		cin >> input;
		stSQL += input;

		query.first = "#Added Author Without Publisher";
		query.second = stSQL;
		break;
	case 2:
		stSQL = "Add_Author_Publisher ";
		cout << "Enter Author First Name" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Author Last Name" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Publisher: " << endl;
		cin >> input;
		stSQL += input;

		query.first = "#Added Author With Publisher";
		query.second = stSQL;
		break;
	case 3:
		stSQL = "Add_Book ";
		cout << "Enter Author ID" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Genre ID" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Title " << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Pages " << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter ISBN " << endl;
		cin >> input;
		stSQL += input;

		query.first = "#Added Book";
		query.second = stSQL;
		break;
	case 4:
		stSQL = "Add_USER ";
		cout << "Enter User First Name" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter User Last Name" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter User Address " << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter User Phone " << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter User Email " << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter User Type " << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter User Drivers License " << endl;
		cin >> input;
		stSQL += input;

		query.first = "#Added User";
		query.second = stSQL;
		break;
	case 5:
		stSQL = "By_Author ";
		cout << "Enter Author " << endl;
		cin >> input;
		stSQL += input;

		query.first = "5~BookTitle~AuthorFirstName~AuthorLastName~Genre~Pages";
		query.second = stSQL;
		break;
	case 6:
		stSQL = "Author_of_Book ";
		cout << "Enter Book " << endl;
		cin >> input;
		stSQL += input;

		query.first = "2~AuthorFirstName~AuthorLastName";
		query.second = stSQL;
		break;
	case 7:
		stSQL = "Book_By_Title ";
		cout << "Enter Part or All of Book Title" << endl;
		cin >> input;
		stSQL += input;

		query.first = "5~BookTitle~AuthorFirstName~AuthorLastName~Genre~Pages";
		query.second = stSQL;
		break;
	case 8:
		stSQL = "Book_Checked_Out ";
		cout << "Enter Book ID" << endl;
		cin >> input;
		stSQL += input;

		query.first = "1~Date_Due";
		query.second = stSQL;
		break;
	case 9:
		stSQL = "Check_Out_Book ";
		cout << "Enter Book ID" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Card ID" << endl;
		cin >> input;
		stSQL += input;


		query.first = "#Added Book to Books_Checked_Out";
		query.second = stSQL;
		break;
	case 10:
		stSQL = "Find_Damages ";
		cout << "Enter Book ID" << endl;
		cin >> input;
		stSQL += input;


		query.first = "3~Damage_ID~Book_ID~Damages";
		query.second = stSQL;
		break;
	case 11:
		stSQL = "Genre_Info ";
		cout << "Enter Genre" << endl;
		cin >> input;
		stSQL += input;


		query.first = "4~Genre_ID~BookTitle~AuthorFirstName~AuthorLastName";
		query.second = stSQL;
		break;
	case 12:
		stSQL = "Give_Fine ";
		cout << "Enter Book ID" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Card ID" << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Days OverDue" << endl;
		cin >> input;
		stSQL += input;


		query.first = "#Give a User a Fine ";
		query.second = stSQL;
		break;
	case 13:
		stSQL = "Library_User_Info ";
		cout << "Enter Name: " << endl;
		cin >> input;
		stSQL += input;


		query.first = "#Return Library User Info by User Name ";
		query.second = stSQL;
		break;
	case 14:
		stSQL = "New_Damages ";
		cout << "Enter Book ID: " << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Damage Description: " << endl;
		cin >> input;
		stSQL += input;


		query.first = "#Add New Damage to Book";
		query.second = stSQL;
		break;
	case 15:
		stSQL = "Pay_Fine ";
		cout << "Enter Card ID: " << endl;
		cin >> input;
		stSQL += input;
		stSQL += ", ";
		cout << "Enter Book ID: " << endl;
		cin >> input;
		stSQL += input;


		query.first = "#Pay a Fine";
		query.second = stSQL;
		break;
	case 16:
		stSQL = "Return_Book ";
		cout << "Enter Card ID: " << endl;
		cin >> input;
		stSQL += input;

		query.first = "#Return a Book";
		query.second = stSQL;
		break;
	case 17:
		stSQL = "Unpaid_By_User ";
		cout << "Enter Card ID: " << endl;
		cin >> input;
		stSQL += input;


		query.first = "#Return Unpaid fines From Specific User";
		query.second = stSQL;
		break;
	case 18:
		stSQL = "SELECT * FROM Books_Out";

		query.first = "2~ID~Date_Out";
		query.second = stSQL;
		break;
	case 19:
		stSQL = "SELECT * FROM Library_Books";

		query.first = "5~BookTitle~Genre_ID~AuthorFirstName~AuthorLastName~Publisher";
		query.second = stSQL;
		break;
	case 20:
		stSQL = "SELECT * FROM Overdue_Books";

		query.first = "1~BookID";
		query.second = stSQL;
		break;
	default:
		cout << "Invalid input, will now exit...\n";
		break;
	}
	return query;


}