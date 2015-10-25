#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>
using namespace std;

#define SQLSERVER

const int MAX_DATA = 100; //will limit the size of the data retrieved, by limiting the number of characters allowed in a data char array
void listProducts(float maxPrice);
string GetQuery(int choice);

int main() {
	listProducts(20.5);

	return 0;
}
void PrintMenu()
{
	cout << "Choose an Option as a number only: " << endl;
	//...menu of options
}


/*
Function listProducts
@param maxPrice the maximum shipping cost that will be passed to the function that builds a query (will be passed to GetQuery)
Prints the query and results of the query specified by the function GetQuery
return is void
*/
void listProducts(float maxPrice) {
	//sets up all of the objects needed to access a database
	RETCODE rc;
	HENV henv;
	HDBC hdbc;
	HSTMT hstmt;
	char szData[MAX_DATA];
	string stSQL;
	SDWORD cbData;
	char szPrice[10];
	int choice;

	_gcvt_s(szPrice, 10, maxPrice, 6); //converts the floating point value maxPrice to a string szPrice

	SQLAllocEnv(&henv);
	SQLAllocConnect(henv, &hdbc);

	SQLCHAR szConnectOut[1024];
	SQLSMALLINT cchConnect;

	//Why is the database type chosen with a preprocessor instruction instead of a normal if clause? What if I want to aceder a multiple types of databases?
	//It is not possible to change this at runtime, so I need to hardcode which type of database I will be using
	//FIXME: Need to change the database location address for each machine this is on
#ifdef SQLSERVER
	//Check out http://www.devlist.com/ConnectionStringsPage.aspx for example connection strings
	string stConnect = "Driver={SQL Server};Server=CS1;";
	stConnect += "Database=Silence_in_the_Library;Trusted_Connection=yes;";
#endif
	cout << "Which query would you like to access? \n";
	cin >> choice;
	stSQL = GetQuery(choice); //calls GetQuery to get the string with the SQL query to be used

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
	//if the query succeded, print the data from each of the first two columns of the query result, until all of the query result rows have been printed
	while (rc == SQL_SUCCESS) {
		rc = SQLFetch(hstmt);
		if (SQLGetData(hstmt, 1, SQL_C_CHAR, szData, sizeof(szData), &cbData) == SQL_SUCCESS)
			//cout << "'" << szData << "'" << " ordered from ";
			cout << szData << "\t";
		if (SQLGetData(hstmt, 2, SQL_C_CHAR, szData, sizeof(szData), &cbData) == SQL_SUCCESS)
			cout << szData << "\t";
		if (SQLGetData(hstmt, 3, SQL_C_CHAR, szData, sizeof(szData), &cbData) == SQL_SUCCESS)
			cout << szData << "\t";
		if (SQLGetData(hstmt, 4, SQL_C_CHAR, szData, sizeof(szData), &cbData) == SQL_SUCCESS)
			cout << szData << "\t";
		cout << endl;
	}
	//Cleanup the complex data structures that were initialized
	SQLFreeStmt(hstmt, SQL_DROP);
	SQLDisconnect(hdbc);
	SQLFreeConnect(hdbc);
	SQLFreeEnv(henv);
}

/*
Function GetQuery
@param szFee the maximum shipping cost
@return a string containing the SQL for the query

This Query, when executed will retrieve the name of the company and the last name of the employee
using the employee, company, and order databases,
inner-joined through orders on employee id and company id (to the respective primary keys in employee and company)
and keeping only the results with a shipping fee less than the shipping fee designated in the function parameter.
The results are then placed in ascending order according to their shipping fee
*/
string GetQuery(int choice) {
	string stSQL;
	switch (choice) {
	case 1:
		stSQL = "SELECT * ";
		stSQL += "FROM Book; ";
		break;
	case 2:
		stSQL = "SELECT MAX(P.[Target Level]), MIN(P.[Target Level]), AVG(P.[Target Level]) ";
		stSQL += "FROM Products AS P; ";
		break;
	case 3:
		stSQL = "SELECT P.[Product Name] COUNT(*) ";
		stSQL += "FROM Products AS P ";
		stSQL += "WHERE P.Discontinued = 1; ";
		break;
	case 4:
		stSQL = "SELECT P.[Product Name] ";
		stSQL += "FROM Products AS P ";
		stSQL += "WHERE P.[Product Name] LIKE '%Dried%'; ";
		break;
	case 5:
		stSQL = "SELECT P.[Product Name] ";
		stSQL += "FROM Products AS P ";
		stSQL += "WHERE P.Category = 'Beverages' AND P.Discontinued = 0; ";
		break;
	case 6:
		stSQL = "SELECT O.[Ship Name] ";
		stSQL += "FROM  Orders AS O ";
		stSQL += "WHERE O.[Shipping Fee] > 100;";
		break;
	case 7:
		stSQL = "SELECT E.[First Name], E.[Last Name], E.[Job Title] ";
		stSQL += "FROM Employees AS E; ";
		break;
	case 8:
		stSQL = "SELECT O.[Shipped Date], E.[First Name], E.[Last Name] ";
		stSQL += "FROM Orders AS O, Employees AS E ";
		stSQL += "WHERE O.[Employee ID] = E.ID; ";
		break;
	case 9:
		stSQL = "SELECT MAX(P.[List Price]), MIN(P.[List Price]), AVG(P.[List Price]) ";
		stSQL += "FROM Products AS P; ";
		break;
	case 10:
		stSQL = "SELECT P.Category, MAX(P.[List Price]), MIN(P.[List Price]), AVG(P.[List Price]) ";
		stSQL += "FROM Products AS P ";
		stSQL += "GROUP BY Category; ";
		break;
	default:
		cout << "Invalid input, will now exit...\n";
		break;
	}
	return stSQL;
}
/*string stSQL = "SELECT C.Company, E.[Last Name] ";
stSQL += "FROM Customers C, Employees E, Orders O ";
stSQL += "WHERE E.ID=O.[Employee ID] AND C.ID=O.[Customer ID] AND ";
stSQL += "O.[Shipping Fee] < ";
stSQL += szFee;
stSQL += " ORDER BY O.[Shipping Fee]";*/