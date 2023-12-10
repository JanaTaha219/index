#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

vector<int> stdNos = {}; // public variables.
vector<vector<string>> index(50, vector<string>(2, ""));
fstream inputFile("Text.txt");
string position;
int counterP = 0, sizeP = 49;
fstream outputFile("index.txt");

void resize_index(vector<vector<string>>& arr) {
    static int count = 2;
    arr.resize((sizeP * count), vector<string>(2, ""));
    count++;
}

void print_index() {
    for (size_t i = 0; i < index.size(); ++i) {
        for (size_t j = 0; j < index[i].size(); ++j) {
            cout << index[i][j] << " ";
        }
        cout << endl;
    }
}

void update_index() {
    fstream outputFile("index.txt");
    outputFile.clear();
    outputFile.seekg(0);
    for (size_t i = 0; i < index.size(); i++) {
        for (size_t j = 0; j < index[i].size(); j++) {
            outputFile << index[i][j] << " ";
        }
        if (index[i][0] != "")
            outputFile << endl;
    }
    outputFile.close();
}

void print_data() {
    inputFile.clear();
    inputFile.seekg(0);
    string line;
    cout << "Data: \n";
    while (getline(inputFile, line)) {
        cout << line << endl;
    }
}

int is_in(const vector<vector<string>>& arr, string address) {
    for (int i = 0; i < arr.size(); i++)
        if (arr[i][0] == address) {
            //cout << address << endl;
            return i;
        }
    return -1;
}

int is_in(int stdno) {
    for (int id : stdNos)
        if (id == stdno)
            return 1;
    return 0;
}

void create_index_file() {
    /* if (!outputFile.is_open()) {
         cout << "Could not open the file." << endl;
         return;
     }*/
    if (counterP == sizeP)
        resize_index(index);

    fstream inputFile("Text.txt");
    fstream outputFile("index.txt");
    inputFile.clear();
    inputFile.seekg(0);
    int stdno;
    stdNos.clear();
    for (size_t i = 0; i < index.size(); ++i) {
        for (size_t j = 0; j < index[i].size(); ++j) {
            index[i][j] = "";
        }
    }
    string firstName, address;
    while (true) {
        position = to_string(inputFile.tellg());
        if (position == "-1")
            break;
        inputFile >> stdno >> firstName >> address;
        //cout << stdno << " " << firstName << " " << address << " " << position << endl;
        if (is_in(stdno) == 1) {
            cout << "student number must be uniqeu!";
            return;
        }
        stdNos.push_back(stdno);
        // cout << stdno << " " << firstName << " " << address << " " << position << endl;
        int loc = is_in(index, address);
        if (loc == -1) {
            index[counterP][0] = address;
            index[counterP][1] += position;
            counterP++;
        }
        else
            index[loc][1] += (" " + position);
    }
    for (const auto& row : index) {
        for (const string& cell : row) {
            outputFile << cell << " ";
        }
        outputFile << endl;
    }
    outputFile.close();
    // cout << "Index file is created." << endl;
}

void insert(int stdno, string name, string address) {
    if (is_in(stdno) == 1) {
        cout << "stdno already exist!";
        return;
    }
    fstream inputFile("Text.txt");
    if (counterP == sizeP)
        resize_index(index);
    stdNos.push_back(stdno);
    inputFile.clear();
    inputFile.seekg(0, ios::end);
    position = to_string(inputFile.tellg());
    // cout << stdno << " " << name << " " << address << "" << position << endl;
    inputFile << "                      " << endl << stdno << " " << name << " " << address;
    int loc = is_in(index, address);
    if (loc == -1) {
        index[counterP][0] = address;
        index[counterP][1] += position;
        counterP++;
    }
    else
        index[loc][1] += (" " + position);
    update_index();
    //cout << "Inserted!" << endl;
}

void delete_record(fstream& file, string record) {
    file.clear();
    file.seekg(0);
    record.erase(remove_if(record.begin(), record.end(), ::isspace), record.end());
    string line;
    string temp;
    vector<string> lines;
    while (getline(file, line)) {
        temp = line;
        temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
        if (temp != record) {
            lines.push_back(line);
        }
    }
    file.close();
    file.open("Text.txt", ios::out | ios::trunc);

    for (size_t i = 0; i < lines.size() - 1; i++) {
        file << lines[i] << endl;
    }
    string rec = lines[(lines.size() - 1)], num, nam, add;
    istringstream iss(rec);
    iss >> num;
    iss >> nam;
    iss >> add;
    rec = num + " " + nam + " " + add;
    file << rec;
    file.close();
}
void Delete(int stdno) {
    fstream inputFile("Text.txt");
    if (is_in(stdno) == 0) {
        cout << "Inavlid student number.";
        return;
    }
    inputFile.clear();
    inputFile.seekg(0);
    int stdNo;
    string name, address;
    string x = to_string(inputFile.tellg()), record;
    while (true) {
        inputFile >> stdNo >> name >> address;
        if (stdNo == stdno) {
            record = to_string(stdNo) + " " + name + " " + address;
            delete_record(inputFile, record);
            //cout << "Delete " << record << endl;
            break;
        }
    }
    std::vector<int>::iterator newEnd = std::remove(stdNos.begin(), stdNos.end(), stdno);
    stdNos.erase(newEnd, stdNos.end());
    create_index_file();
    //print_index();
    update_index();
}

void Delete(int stdno, string address) {
    int loc = is_in(index, address);
    if (is_in(stdno) == 0 || loc == -1) {
        cout << "Inavlid student number or address.";
        return;
    }
    fstream outputFile("index.txt");
    fstream inputFile("Text.txt");
    string line;
    vector<int> numbers;
    while (getline(outputFile, line)) {
        istringstream iss(line);
        string firstWord;
        iss >> firstWord;
        if (firstWord == address) {
            int num;
            while (iss >> num) {
                numbers.push_back(num);
            }
            for (int num : numbers) {
                // cout << "num=" << num << endl;
                inputFile.clear();
                inputFile.seekg(num, ios::beg);
                int x;
                string name;
                inputFile >> x >> name;
                //cout << "x=" << x << endl;
                if (x == stdno) {
                    string rec = to_string(x) + " " + name + " " + address;
                    //cout << "Delete " << rec << endl;
                    delete_record(inputFile, rec);
                    std::vector<int>::iterator newEnd = std::remove(stdNos.begin(), stdNos.end(), stdno);
                    stdNos.erase(newEnd, stdNos.end());
                    create_index_file();
                    // print_index();
                    update_index();
                    return;
                }
            }
        }
    }
    cout << "No student with the given number is in the given address!\n";
}

void update_name(int stdno, string name) {
    if (is_in(stdno) == 0) {
        cout << "stdno does not exist!" << endl;
        return;
    }
    fstream inputFile("Text.txt");
    string address, rec, line, firstWord;
    vector<string> lines;
    //cout << "record= " << record << endl;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        iss >> firstWord;
        iss >> address;
        iss >> address;
        if (firstWord == to_string(stdno)) {
            rec = firstWord + " " + name + " " + address + "                     ";
            lines.push_back(rec);
        }
        else
            lines.push_back(line);
    }
    inputFile.close();
    inputFile.open("Text.txt", ios::out | ios::trunc);
    for (size_t i = 0; i < lines.size() - 1; i++)
        inputFile << lines[i] << endl;
    string recc = lines[(lines.size() - 1)], num, nam, add;
    istringstream iss(recc);
    iss >> num;
    iss >> nam;
    iss >> add;
    recc = num + " " + nam + " " + add;
    inputFile << recc;
    inputFile.close();
}

void update_address(int stdno, string address) {
    if (is_in(stdno) == 0) {
        cout << "stdno does not exist!" << endl;
        return;
    }
    fstream inputFile("Text.txt");
    string name, rec, line, firstWord;
    vector<string> lines;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        iss >> firstWord;
        iss >> name;
        if (firstWord == to_string(stdno)) {
            rec = firstWord + " " + name + " " + address;
            lines.push_back(rec);
        }
        else
            lines.push_back(line);
    }
    inputFile.close();
    inputFile.open("Text.txt", ios::out | ios::trunc);
    for (size_t i = 0; i < lines.size() - 1; i++)
        inputFile << lines[i] << "                " << endl;
    string recc = lines[(lines.size() - 1)], num, nam, add;
    istringstream iss(recc);
    iss >> num;
    iss >> nam;
    iss >> add;
    recc = num + " " + nam + " " + add;
    inputFile << recc;
    inputFile.close();
    create_index_file();
    //print_index();
    update_index();
}

void update_name_and_address(int stdno, string name, string address) {
    update_name(stdno, name);
    update_address(stdno, address);
}

void reshape_file() {
    inputFile.open("Text.txt", ios::out | ios::trunc);
    inputFile.clear();
    inputFile.seekg(0, ios::beg);
    string line;
    vector<string> lines;
    while (getline(inputFile, line))
        lines.push_back(line);
    inputFile.clear();
    inputFile.seekg(0, ios::beg);
    for (size_t i = 0; i < lines.size() - 1; i++)
        inputFile << lines[i] << "                             " << endl;
    string recc = lines[(lines.size() - 1)], num, nam, add;
    istringstream iss(recc);
    iss >> num;
    iss >> nam;
    iss >> add;
    recc = num + " " + nam + " " + add;
    inputFile << recc;
    inputFile.close();
}
void update_name(int stdno, string new_name, string address) {
    int loc = is_in(index, address);
    if (is_in(stdno) == 0 || loc == -1) {
        cout << "Inavlid student number or address.";
        return;
    }
    fstream outputFile("index.txt");
    fstream inputFile("Text.txt");
    string line;
    int x;
    int cc = 0;
    string name;
    vector<int> numbers;
    string aaa, bbb;
    while (getline(outputFile, line)) {
        istringstream iss(line);
        string firstWord;
        iss >> firstWord;
        if (firstWord == address) {
            int num;
            while (iss >> num) {
                numbers.push_back(num);
            }
            for (int num : numbers) {
                inputFile.clear();
                inputFile.seekg(num, ios::beg);
                inputFile >> x >> name >> aaa;
                inputFile.seekg(num, ios::beg);
                if (x == stdno) {
                    cc = 1;
                    bbb = to_string(x) + " " + name + " " + bbb;
                    Delete(x, aaa);
                    string rec = "\n" + to_string(x) + " " + new_name + " " + address + "                      ";
                    insert(x, new_name, address);
                    break;
                }
            }
            if (cc == 0)
                cout << "No student with given number is in the given address!\n";
            return;
        }
    }
    inputFile.close();
    reshape_file();
}

void update_address(int stdno, string address, string new_address) {
    int loc = is_in(index, address);
    if (is_in(stdno) == 0 || loc == -1) {
        cout << "Inavlid student number or address.";
        return;
    }
    fstream outputFile("index.txt");
    fstream inputFile("Text.txt");
    string line;
    int x;
    string name;
    int cc = 0;
    vector<int> numbers;
    while (getline(outputFile, line)) {
        istringstream iss(line);
        string firstWord;
        iss >> firstWord;
        if (firstWord == address) {
            int num;
            while (iss >> num) {
                numbers.push_back(num);
            }
            for (int num : numbers) {
                inputFile.clear();
                inputFile.seekg(num, ios::beg);
                inputFile >> x >> name;
                inputFile.seekg(num, ios::beg);
                if (x == stdno) {
                    cc = 1;
                    string rec = to_string(x) + " " + name + " " + new_address;
                    //cout << rec;
                    Delete(x, address);
                    insert(x, name, new_address);
                    //inputFile << rec;
                    break;
                }
            }
            if (cc == 0)
                cout << "No student with given number is in the given address!\n";
            return;
        }
    }
    inputFile.close();
    //update_shape();
   // create_index_file();
    //update_index();
}

void show_options() {
    int number;
    cout << "1- Delete by student number only. \n";
    cout << "2- Delete by student number and address. \n";
    cout << "3- Insert. \n";
    cout << "4- Update name by student number. \n";
    cout << "5- Update adress by student number. \n";
    cout << "6- Update name and address by student number. \n";
    cout << "7- Update name by student number and address. \n";
    cout << "8- Update address by student number and address. \n";
    cout << "9- Update name and address by student number and address. \n";
    cout << "10- Exit. \n";
    cout << "Choose opration by its number \n";
    cin >> number;
    int stdno;
    string address, name, currAddress;
    switch (number) {
    case 1:
        cout << "Enter student number please.\n";
        cin >> stdno;
        Delete(stdno);
        break;
    case 2:
        cout << "Enter student number please.\n";
        cin >> stdno;
        cout << "Enter student address please.\n";
        cin >> address;
        Delete(stdno, address);
        break;
    case 3:
        cout << "Enter student number please.\n";
        cin >> stdno;
        cout << "Enter student name please.\n";
        cin >> name;
        cout << "Enter student address please.\n";
        cin >> address;
        insert(stdno, name, address);
        break;
    case 4:
        cout << "Enter student number please.\n";
        cin >> stdno;
        cout << "Enter student new name please.\n";
        cin >> name;
        update_name(stdno, name);
        break;
    case 5:
        cout << "Enter student number please.\n";
        cin >> stdno;
        cout << "Enter student new address please.\n";
        cin >> address;
        update_address(stdno, address);
        break;
    case 6:
        cout << "Enter student number please.\n";
        cin >> stdno;
        cout << "Enter student new name please.\n";
        cin >> name;
        cout << "Enter student new address please.\n";
        cin >> address;
        update_name_and_address(stdno, name, address);
        break;
    case 7:
        cout << "Enter student number please.\n";
        cin >> stdno;
        cout << "Enter student new name please.\n";
        cin >> name;
        cout << "Enter student current address please.\n";
        cin >> address;
        update_name(stdno, name, address);
        break;
    case 8:
        cout << "Enter student number please.\n";
        cin >> stdno;
        cout << "Enter student current address please.\n";
        cin >> currAddress;
        cout << "Enter student address please.\n";
        cin >> address;
        update_address(stdno, currAddress, address);
        break;
    case 9:
        cout << "Enter student number please.\n";
        cin >> stdno;
        cout << "Enter student name please.\n";
        cin >> name;
        cout << "Enter student current address please.\n";
        cin >> currAddress;
        cout << "Enter student address please.\n";
        cin >> address;
        update_address(stdno, currAddress, address);
        update_name(stdno, name, address);
        break;
    case 10:
        cout << "Exit \n";
        break;
    default:
        cout << "Error! \n";
        break;
    }
}

int main() {
    fstream inputFile("Text.txt");
    fstream outputFile("index.txt");
    reshape_file();
    create_index_file();
    show_options();
}