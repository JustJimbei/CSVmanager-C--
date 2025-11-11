#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <string>
#include <cctype>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"


bool checkAllDigit(const string& str);

string validateDigit();

int main() 
{
    string names[100];
    string emails[100];
    string phoneNumber[100];
    int limiter;

    cout << "Enter the list limit" << endl;
    cout << "Limit: ";
    cin >> limiter;
    cin.ignore();

    fstream infoFile;
    infoFile.open("ArrangedbyName.csv", ios::out);

    if (!infoFile.is_open())
    {
        cerr << RED << "Error Accessing the File" << RESET << endl;
        return 1;
    }
    
    else if (infoFile.is_open())
    {    
            infoFile << "Names " << "," << "Emails " << "," << "Phone Numbers " << endl;

            cout << BLUE << "Enter the Name, Email and Phone Number" << RESET << endl;


            for (int i = 0; i < limiter; i++)
            {
                cout << "Name: ";
                getline(cin, names[i]);

                cout << "Email: ";
                cin >> emails[i];
                
                phoneNumber[i] = validateDigit();
                cin.ignore();
                cout << endl;
            }


            for (int i = 0; i < limiter; i++)
            {
                infoFile << names[i] << ","<< emails[i] << "," << phoneNumber[i] << endl;
            }
            
    }

    else {
        cerr << RED << "Unknown Error Occurred" << RESET << endl;
        return 1;
    }

    infoFile.close();

    this_thread::sleep_for(chrono::seconds(3));
    cout << GREEN << "File Successfully Saved" << RESET << endl;
    return 0;
}



bool checkAllDigit(const string& str)
{
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}


string validateDigit()
{
    string userInput;

    while (true)
    {
        cout << "Phone Number: ";
        cin >> userInput;

        if (checkAllDigit(userInput))
        {
            return userInput;
        }

        else 
        {
            cerr << RED << "Invalid Input! Numbers Only!" << RESET << endl;
        }
    }
}