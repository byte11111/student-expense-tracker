/*
    Student Expense Tracker
    SY CSE Mini Project

    Features:
    1. Add Expense
    2. View All Expenses
    3. Search Expense
    4. Calculate Total Spending
    5. Category-wise Spending
    6. Delete Expense
    7. Exit

    Data is stored in expenses.csv
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

const string FILE_NAME = "expenses.csv";

struct Expense
{
    int id;
    string date;
    string category;
    string description;
    double amount;
};

class ExpenseTracker
{
private:
    vector<Expense> expenses;
    int nextId;

public:
    ExpenseTracker()
    {
        nextId = 1;
        loadFromFile();
    }

    // Load saved expenses from file
    void loadFromFile()
    {
        ifstream file(FILE_NAME);

        if (!file)
        {
            return;
        }

        string line;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string idText, date, category, description, amountText;

            getline(ss, idText, ',');
            getline(ss, date, ',');
            getline(ss, category, ',');
            getline(ss, description, ',');
            getline(ss, amountText, ',');

            try
            {
                Expense e;
                e.id = stoi(idText);
                e.date = date;
                e.category = category;
                e.description = description;
                e.amount = stod(amountText);

                expenses.push_back(e);

                if (e.id >= nextId)
                    nextId = e.id + 1;
            }
            catch (...)
            {
                // Ignore invalid lines in the file
            }
        }

        file.close();
    }

    // Save all expenses to file
    void saveToFile()
    {
        ofstream file(FILE_NAME);

        for (int i = 0; i < (int)expenses.size(); i++)
        {
            file << expenses[i].id << ","
                 << expenses[i].date << ","
                 << expenses[i].category << ","
                 << expenses[i].description << ","
                 << expenses[i].amount << endl;
        }

        file.close();
    }

    // Add a new expense
    void addExpense()
    {
        Expense e;
        e.id = nextId++;

        cout << "\nEnter date (DD-MM-YYYY): ";
        cin >> e.date;

        cout << "Enter category (Food/Travel/Books/etc.): ";
        cin >> e.category;

        cout << "Enter description: ";
        cin.ignore();
        getline(cin, e.description);

        // Do not allow comma because comma is used in the CSV file
        replace(e.description.begin(), e.description.end(), ',', ' ');

        while (true)
        {
            cout << "Enter amount: ";
            cin >> e.amount;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Please enter a valid number.\n";
            }
            else if (e.amount <= 0)
            {
                cout << "Amount must be greater than 0.\n";
            }
            else
            {
                break;
            }
        }

        expenses.push_back(e);
        saveToFile();

        cout << "\nExpense added successfully! ID: " << e.id << endl;
    }

    // Display all expenses
    void viewAll()
    {
        if (expenses.empty())
        {
            cout << "\nNo expenses recorded yet.\n";
            return;
        }

        cout << "\n-------------------- All Expenses --------------------\n";

        cout << left
             << setw(5) << "ID"
             << setw(14) << "Date"
             << setw(14) << "Category"
             << setw(25) << "Description"
             << setw(10) << "Amount" << endl;

        cout << string(68, '-') << endl;

        for (int i = 0; i < (int)expenses.size(); i++)
        {
            cout << left
                 << setw(5) << expenses[i].id
                 << setw(14) << expenses[i].date
                 << setw(14) << expenses[i].category
                 << setw(25) << expenses[i].description
                 << "Rs." << fixed << setprecision(2)
                 << expenses[i].amount << endl;
        }
    }

    // Search by category or description
    void searchExpense()
    {
        if (expenses.empty())
        {
            cout << "\nNo expenses recorded yet.\n";
            return;
        }

        string keyword;

        cout << "\nEnter keyword to search: ";
        cin.ignore();
        getline(cin, keyword);

        // Convert keyword to lowercase
        transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

        bool found = false;

        cout << "\nSearch Results:\n";
        cout << string(68, '-') << endl;

        for (int i = 0; i < (int)expenses.size(); i++)
        {
            string category = expenses[i].category;
            string description = expenses[i].description;

            transform(category.begin(), category.end(), category.begin(), ::tolower);
            transform(description.begin(), description.end(), description.begin(), ::tolower);

            if (category.find(keyword) != string::npos ||
                description.find(keyword) != string::npos)
            {
                cout << "ID: " << expenses[i].id
                     << " | Date: " << expenses[i].date
                     << " | Category: " << expenses[i].category
                     << " | Description: " << expenses[i].description
                     << " | Rs." << fixed << setprecision(2)
                     << expenses[i].amount << endl;

                found = true;
            }
        }

        if (!found)
        {
            cout << "No matching expense found.\n";
        }
    }

    // Calculate total spending
    void calculateTotal()
    {
        double total = 0;

        for (int i = 0; i < (int)expenses.size(); i++)
        {
            total = total + expenses[i].amount;
        }

        cout << "\nTotal number of expenses: " << expenses.size() << endl;
        cout << "Total spending: Rs."
             << fixed << setprecision(2) << total << endl;
    }

    // Show category-wise total
    void categoryWiseSpending()
    {
        if (expenses.empty())
        {
            cout << "\nNo expenses recorded yet.\n";
            return;
        }

        map<string, double> categoryTotal;

        for (int i = 0; i < (int)expenses.size(); i++)
        {
            categoryTotal[expenses[i].category] =
                categoryTotal[expenses[i].category] + expenses[i].amount;
        }

        cout << "\n--------- Category-wise Spending ---------\n";

        for (auto item : categoryTotal)
        {
            cout << left << setw(15) << item.first
                 << "Rs." << fixed << setprecision(2)
                 << item.second << endl;
        }
    }

    // Delete an expense using its ID
    void deleteExpense()
    {
        if (expenses.empty())
        {
            cout << "\nNo expenses recorded yet.\n";
            return;
        }

        int id;

        cout << "\nEnter ID of expense to delete: ";
        cin >> id;

        bool found = false;

        for (int i = 0; i < (int)expenses.size(); i++)
        {
            if (expenses[i].id == id)
            {
                expenses.erase(expenses.begin() + i);
                saveToFile();

                cout << "Expense deleted successfully.\n";
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "Expense with ID " << id << " not found.\n";
        }
    }
};

// Display main menu
void showMenu()
{
    cout << "\n============================================\n";
    cout << "          STUDENT EXPENSE TRACKER           \n";
    cout << "============================================\n";
    cout << "1. Add Expense\n";
    cout << "2. View All Expenses\n";
    cout << "3. Search Expense\n";
    cout << "4. Calculate Total Spending\n";
    cout << "5. Category-wise Spending\n";
    cout << "6. Delete Expense\n";
    cout << "7. Exit\n";
    cout << "============================================\n";
}

int main()
{
    ExpenseTracker tracker;
    int choice;

    do
    {
        showMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nPlease enter a number from 1 to 7.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                tracker.addExpense();
                break;

            case 2:
                tracker.viewAll();
                break;

            case 3:
                tracker.searchExpense();
                break;

            case 4:
                tracker.calculateTotal();
                break;

            case 5:
                tracker.categoryWiseSpending();
                break;

            case 6:
                tracker.deleteExpense();
                break;

            case 7:
                cout << "\nThank you for using Student Expense Tracker!\n";
                break;

            default:
                cout << "\nInvalid choice. Please select 1 to 7.\n";
        }

    } while (choice != 7);

    return 0;
}
