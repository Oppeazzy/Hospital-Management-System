
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <algorithm>  

using namespace std;

// ================= GLOBAL STRUCTS =================
struct User {
    string username;
    string password;
    string role; // admin or staff
};

struct Patient {
    string id;
    string name;
    int age;
    string gender;
    string diagnosis;
};

struct Appointment {
    string patientID;
    string doctorName;
    string date;
    string time;
};

struct Bill {
    string patientID;
    float amount;
    string description;
};

// ================= FUNCTION DECLARATIONS =================
void welcomeScreen();
void mainMenu(const User& currentUser);
void login();
void createAccount();
User parseUser(string line);
void addPatient();
void viewPatients();
void scheduleAppointment();
void viewAppointments();
void generateBill();
void viewBills();
void clearScreen();
string generatePatientID();
void pause();

// ================= FUNCTION DEFINITIONS =================
void welcomeScreen() {
    clearScreen();
    cout << "==============================\n";
    cout << "   HOSPITAL MANAGEMENT SYSTEM\n";
    cout << "==============================\n\n";
    cout << "1. Login\n";
    cout << "2. Create Account\n";
    cout << "3. Exit\n\n";
    cout << "Choose an option: ";
    int choice;
    cin >> choice;
    cin.ignore();
    switch(choice) {
        case 1: login(); break;
        case 2: createAccount(); break;
        case 3: exit(0); break;
        default: welcomeScreen(); break;
    }
}

void login() {
    clearScreen();
    string username, password;
    cout << "\n--- Login ---\n";
    cout << "Username: "; getline(cin, username);
    cout << "Password: "; getline(cin, password);

    ifstream file("users.txt");
    string line;
    while(getline(file, line)) {
        User user = parseUser(line);
        if(user.username == username && user.password == password) {
            cout << "\nLogin successful as " << user.role << "!\n";
            pause();
            mainMenu(user);
            return;
        }
    }
    cout << "\nInvalid credentials. Try again.\n";
    pause();
    welcomeScreen();
}

void createAccount() {
    clearScreen();
    string username, password, role;
    cout << "\n--- Create Account ---\n";
    cout << "Choose role (admin/staff): "; getline(cin, role);
    cout << "Username: "; getline(cin, username);
    cout << "Password: "; getline(cin, password);

    ofstream file("users.txt", ios::app);
    file << username << "," << password << "," << role << endl;
    file.close();
    cout << "\nAccount created successfully!\n";
    pause();
    welcomeScreen();
}

User parseUser(string line) {
    stringstream ss(line);
    User user;
    getline(ss, user.username, ',');
    getline(ss, user.password, ',');
    getline(ss, user.role);
    return user;
}

void mainMenu(const User& currentUser) {
    clearScreen();
    int choice;
    cout << "\n--- Main Menu ---\n";
    cout << "1. Add Patient\n";
    cout << "2. View Patients\n";
    cout << "3. Schedule Appointment\n";
    cout << "4. View Appointments\n";
    cout << "5. Generate Bill\n";
    cout << "6. View Bills\n";
    if (currentUser.role == "admin")
        cout << "7. Create Staff Account\n";
    cout << "0. Logout\n";
    cout << "\nChoose an option: ";
    cin >> choice;
    cin.ignore();

    switch(choice) {
        case 1: addPatient(); break;
        case 2: viewPatients(); break;
        case 3: scheduleAppointment(); break;
        case 4: viewAppointments(); break;
        case 5: generateBill(); break;
        case 6: viewBills(); break;
        case 7: if(currentUser.role == "admin") createAccount(); else mainMenu(currentUser); break;
        case 0: welcomeScreen(); break;
        default: mainMenu(currentUser); break;
    }
    pause();
    mainMenu(currentUser);
}

void addPatient() {
    clearScreen();
    Patient p;
    p.id = generatePatientID();
    cout << "\n--- Add Patient ---\n";
    cout << "Name: "; getline(cin, p.name);
    cout << "Age: "; cin >> p.age; cin.ignore();
    cout << "Gender: "; getline(cin, p.gender);
    cout << "Diagnosis: "; getline(cin, p.diagnosis);

    ofstream file("patients.csv", ios::app);
    file << p.id << "," << p.name << "," << p.age << "," << p.gender << "," << p.diagnosis << endl;
    file.close();
    cout << "\nPatient added with ID: " << p.id << endl;
}

void viewPatients() {
    clearScreen();
    cout << "\n--- All Patients ---\n\n";
    ifstream file("patients.csv");
    string line;
    while(getline(file, line)) {
        replace(line.begin(), line.end(), ',', ' ');
        cout << line << endl;
    }
}

void scheduleAppointment() {
    clearScreen();
    Appointment a;
    cout << "\n--- Schedule Appointment ---\n";
    cout << "Patient ID: "; getline(cin, a.patientID);
    cout << "Doctor Name: "; getline(cin, a.doctorName);
    cout << "Date (YYYY-MM-DD): "; getline(cin, a.date);
    cout << "Time (HH:MM): "; getline(cin, a.time);

    ofstream file("appointments.csv", ios::app);
    file << a.patientID << "," << a.doctorName << "," << a.date << "," << a.time << endl;
    cout << "\nAppointment scheduled!\n";
}

void viewAppointments() {
    clearScreen();
    cout << "\n--- Appointments ---\n\n";
    ifstream file("appointments.csv");
    string line;
    while(getline(file, line)) {
        replace(line.begin(), line.end(), ',', ' ');
        cout << line << endl;
    }
}

void generateBill() {
    clearScreen();
    Bill b;
    cout << "\n--- Generate Bill ---\n";
    cout << "Patient ID: "; getline(cin, b.patientID);
    cout << "Description: "; getline(cin, b.description);
    cout << "Amount: "; cin >> b.amount; cin.ignore();

    ofstream file("bills.csv", ios::app);
    file << b.patientID << "," << b.amount << "," << b.description << endl;
    cout << "\nBill generated!\n";
}

void viewBills() {
    clearScreen();
    cout << "\n--- All Bills ---\n\n";
    ifstream file("bills.csv");
    string line;
    while(getline(file, line)) {
        replace(line.begin(), line.end(), ',', ' ');
        cout << line << endl;
    }
}

string generatePatientID() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << "P" << 1900 + ltm->tm_year << ltm->tm_mon + 1 << ltm->tm_mday << ltm->tm_hour << ltm->tm_min << ltm->tm_sec;
    return ss.str();
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\nPress enter to continue...";
    cin.get();
}

// ================= MAIN =================
int main() {
    welcomeScreen();
    return 0;
}
