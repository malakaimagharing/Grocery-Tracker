#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <cctype> // For tolower function

using namespace std;

// Class: GroceryTracker
// Purpose: Tracks grocery item frequencies from an input file,
//          provides a menu for lookup, displaying data, and creating a backup.
class GroceryTracker {
private:
    // Map to store item frequency data (key: item, value: count)
    map<string, int> itemCount;
    // File name for backing up the frequency data
    string backupFileName;

    // Helper: Converts a given string to lowercase for case-insensitive operations
    string toLower(const string& str) const {
        string lowerStr = str;
        transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

public:
    // Constructor: Initializes backup file name
    GroceryTracker() : backupFileName("frequency.dat") {}

    // Loads records from the specified file and tallies item counts.
    // Each line of the file is considered an individual item.
    void loadRecords(const string& fileName) {
        ifstream inFile(fileName);
        if (!inFile) {
            cout << "Error: Could not open " << fileName << endl;
            return;
        }
        string line;
        while (getline(inFile, line)) {
            // Only process non-empty lines; convert to lowercase for consistency.
            if (!line.empty()) {
                string item = toLower(line);
                itemCount[item]++;
            }
        }
        inFile.close();
    }

    // Saves the item counts to a backup file.
    // The backup file includes a header and formatted item-frequency data.
    void backupData() const {
        ofstream outFile(backupFileName);
        if (!outFile) {
            cout << "Error: Could not create backup file " << backupFileName << endl;
            return;
        }
        outFile << "Grocery Tracker Backup Data\n";
        outFile << "----------------------------\n";
        for (const auto& entry : itemCount) {
            outFile << setw(20) << left << entry.first << " : " << entry.second << "\n";
        }
        outFile.close();
        cout << "Data successfully backed up to " << backupFileName << "\n";
    }

    // Looks up and displays the frequency of a specific grocery item.
    // The lookup is case-insensitive.
    void lookupItemFrequency() const {
        string query;
        cout << "Enter item name to search: ";
        getline(cin, query);
        query = toLower(query);
        int frequency = itemCount.count(query) ? itemCount.at(query) : 0;
        cout << "Frequency for \"" << query << "\": " << frequency << "\n";
    }

    // Displays a comprehensive list of all grocery items and their frequencies.
    void displayFrequencies() const {
        cout << "\nItem Frequencies:\n";
        cout << "-----------------\n";
        for (const auto& entry : itemCount) {
            cout << setw(20) << left << entry.first << " : " << entry.second << "\n";
        }
    }

    // Displays a text-based histogram where the number of asterisks represents the frequency.
    void displayHistogram() const {
        cout << "\nFrequency Histogram:\n";
        cout << "--------------------\n";
        for (const auto& entry : itemCount) {
            cout << setw(20) << left << entry.first << " ";
            for (int i = 0; i < entry.second; ++i) {
                cout << "*";
            }
            cout << " (" << entry.second << ")\n";
        }
    }

    // Runs the interactive menu, providing user options for data lookup, display, and exit.
    void run() {
        int option = 0;
        do {
            cout << "\nMenu Options:\n"
                << "1. Look up an item frequency\n"
                << "2. Display all item frequencies\n"
                << "3. Display frequency histogram\n"
                << "4. Exit\n"
                << "Choose an option (1-4): ";
            cin >> option;
            cin.ignore(); // Clear leftover newline from input buffer

            switch (option) {
            case 1:
                lookupItemFrequency();
                break;
            case 2:
                displayFrequencies();
                break;
            case 3:
                displayHistogram();
                break;
            case 4:
                // Confirm exit before backing up data
            {
                char confirm;
                cout << "Are you sure you want to exit? (y/n): ";
                cin >> confirm;
                cin.ignore(); // Clear newline
                if (tolower(confirm) == 'y') {
                    backupData();
                    cout << "Exiting program. Goodbye!\n";
                }
                else {
                    option = 0;  // Cancel exit and return to menu
                }
            }
            break;
            default:
                cout << "Invalid option. Please try again.\n";
                break;
            }
        } while (option != 4);
    }
};

// Main function: Sets up the GroceryTracker and starts the program.
int main() {
    const string inputFile = "CS210_Project_Three_Input_File.txt";
    GroceryTracker tracker;
    tracker.loadRecords(inputFile);
    tracker.run();
    return 0;
}