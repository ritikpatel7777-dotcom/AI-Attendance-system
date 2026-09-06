#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cstdint>
#include <chrono>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

// ============================================================
//              AI ATTENDANCE SYSTEM
// ============================================================
// Features:
// 1. Admin Login
// 2. Student Registration
// 3. Duplicate Student ID Prevention
// 4. Student Image Registration
// 5. Basic Image Identification
// 6. Automatic Attendance
// 7. Duplicate Attendance Prevention
// 8. Today's Attendance
// 9. Student Attendance History
// 10. Attendance Percentage
// 11. File Based Data Storage
// ============================================================


// ============================================================
//                      DATA STRUCTURES
// ============================================================

struct Student
{
    string id;
    string name;
    string roll;
    string department;
    string contact;
    string imagePath;
};

struct Attendance
{
    string studentId;
    string name;
    string date;
    string time;
    string status;
};


// ============================================================
//                    UTILITY FUNCTIONS
// ============================================================

string getCurrentDate()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    stringstream ss;
    ss << put_time(&localTime, "%d-%m-%Y");

    return ss.str();
}


string getCurrentTime()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    stringstream ss;
    ss << put_time(&localTime, "%I:%M:%S %p");

    return ss.str();
}


void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


bool isBlank(const string& text)
{
    return text.find_first_not_of(" \t\r\n") == string::npos;
}


// ============================================================
//                    FILE MANAGER CLASS
// ============================================================

class FileManager
{
private:

    const string dataFolder = "data";
    const string imageFolder = "images/registered";
    const string reportFolder = "reports";

    const string studentFile = "data/students.txt";
    const string attendanceFile = "data/attendance.txt";


public:

    FileManager()
    {
        initialize();
    }


    void initialize()
    {
        try
        {
            fs::create_directories(dataFolder);
            fs::create_directories(imageFolder);
            fs::create_directories(reportFolder);

            if (!fs::exists(studentFile))
            {
                ofstream file(studentFile);
            }

            if (!fs::exists(attendanceFile))
            {
                ofstream file(attendanceFile);
            }
        }
        catch (const exception& e)
        {
            cout << "File system error: "
                 << e.what() << endl;
        }
    }


    // ---------------- SAVE STUDENT ----------------

    bool saveStudent(const Student& student)
    {
        ofstream file(studentFile, ios::app);

        if (!file)
        {
            cout << "Error: Cannot open student data file.\n";
            return false;
        }

        file << student.id << "|"
             << student.name << "|"
             << student.roll << "|"
             << student.department << "|"
             << student.contact << "|"
             << student.imagePath << "\n";

        return true;
    }


    // ---------------- LOAD STUDENTS ----------------

    vector<Student> loadStudents()
    {
        vector<Student> students;

        ifstream file(studentFile);

        if (!file)
        {
            return students;
        }

        string line;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);

            Student student;

            getline(ss, student.id, '|');
            getline(ss, student.name, '|');
            getline(ss, student.roll, '|');
            getline(ss, student.department, '|');
            getline(ss, student.contact, '|');
            getline(ss, student.imagePath, '|');

            if (!student.id.empty())
            {
                students.push_back(student);
            }
        }

        return students;
    }


    // ---------------- SAVE ATTENDANCE ----------------

    bool saveAttendance(const Attendance& attendance)
    {
        ofstream file(attendanceFile, ios::app);

        if (!file)
        {
            cout << "Error: Cannot open attendance file.\n";
            return false;
        }

        file << attendance.studentId << "|"
             << attendance.name << "|"
             << attendance.date << "|"
             << attendance.time << "|"
             << attendance.status << "\n";

        return true;
    }


    // ---------------- LOAD ATTENDANCE ----------------

    vector<Attendance> loadAttendance()
    {
        vector<Attendance> records;

        ifstream file(attendanceFile);

        if (!file)
        {
            return records;
        }

        string line;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);

            Attendance attendance;

            getline(ss, attendance.studentId, '|');
            getline(ss, attendance.name, '|');
            getline(ss, attendance.date, '|');
            getline(ss, attendance.time, '|');
            getline(ss, attendance.status, '|');

            if (!attendance.studentId.empty())
            {
                records.push_back(attendance);
            }
        }

        return records;
    }
};


// ============================================================
//                 ADMIN AUTHENTICATION
// ============================================================

class AdminAuthentication
{
private:

    const string username = "admin";
    const string password = "admin123";


public:

    bool login()
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "             ADMIN LOGIN\n";
        cout << "========================================\n";

        for (int attempt = 1; attempt <= 3; attempt++)
        {
            string user;
            string pass;

            cout << "Username: ";
            cin >> user;

            cout << "Password: ";
            cin >> pass;

            if (user == username && pass == password)
            {
                cout << "\nLogin Successful!\n";
                return true;
            }

            cout << "\nInvalid Username or Password.\n";

            if (attempt < 3)
            {
                cout << "Attempts remaining: "
                     << 3 - attempt << "\n\n";
            }
        }

        cout << "\nAccess Denied.\n";

        return false;
    }
};


// ============================================================
//                    IMAGE MATCHER
// ============================================================
//
// IMPORTANT:
// This is a BASIC image-file matching system.
// It does NOT perform real facial recognition.
//
// It compares the complete file contents using a hash.
// Actual OpenCV face recognition can be added later.
// ============================================================

class ImageMatcher
{
private:

    uint64_t hashFile(const string& path)
    {
        ifstream file(path, ios::binary);

        if (!file)
        {
            return 0;
        }

        // FNV-1a 64-bit hash
        uint64_t hash =
            1469598103934665603ULL;

        const uint64_t prime =
            1099511628211ULL;

        char byte;

        while (file.get(byte))
        {
            hash ^= static_cast<unsigned char>(byte);
            hash *= prime;
        }

        return hash;
    }


public:

    bool filesMatch(
        const string& file1,
        const string& file2)
    {
        if (!fs::exists(file1) ||
            !fs::exists(file2))
        {
            return false;
        }

        uint64_t hash1 = hashFile(file1);
        uint64_t hash2 = hashFile(file2);

        return hash1 != 0 &&
               hash1 == hash2;
    }
};


// ============================================================
//                    STUDENT MANAGER
// ============================================================

class StudentManager
{
private:

    FileManager& fileManager;


public:

    StudentManager(FileManager& fm)
        : fileManager(fm)
    {
    }


    // ---------------- CHECK DUPLICATE ID ----------------

    bool studentIdExists(const string& id)
    {
        vector<Student> students =
            fileManager.loadStudents();

        for (const auto& student : students)
        {
            if (student.id == id)
            {
                return true;
            }
        }

        return false;
    }


    // ---------------- REGISTER STUDENT ----------------

    void registerStudent()
    {
        Student student;

        cout << "\n";
        cout << "========================================\n";
        cout << "          REGISTER STUDENT\n";
        cout << "========================================\n";

        clearInput();

        cout << "Student ID: ";
        getline(cin, student.id);

        if (isBlank(student.id))
        {
            cout << "Error: Student ID cannot be empty.\n";
            return;
        }

        if (studentIdExists(student.id))
        {
            cout << "Error: Student ID already exists.\n";
            return;
        }


        cout << "Student Name: ";
        getline(cin, student.name);


        cout << "Roll Number: ";
        getline(cin, student.roll);


        cout << "Department/Course: ";
        getline(cin, student.department);


        cout << "Contact Number: ";
        getline(cin, student.contact);


        cout << "Registered Image File Path: ";
        getline(cin, student.imagePath);


        if (isBlank(student.name) ||
            isBlank(student.roll) ||
            isBlank(student.department) ||
            isBlank(student.contact) ||
            isBlank(student.imagePath))
        {
            cout << "\nError: All fields are required.\n";
            return;
        }


        if (!fs::exists(student.imagePath))
        {
            cout << "\nError: Image file does not exist.\n";
            cout << "Please enter the complete image path.\n";
            return;
        }


        if (fileManager.saveStudent(student))
        {
            cout << "\n";
            cout << "========================================\n";
            cout << "       STUDENT REGISTERED\n";
            cout << "========================================\n";

            cout << "Student ID : "
                 << student.id << endl;

            cout << "Name       : "
                 << student.name << endl;

            cout << "Roll       : "
                 << student.roll << endl;

            cout << "Department : "
                 << student.department << endl;

            cout << "Contact    : "
                 << student.contact << endl;

            cout << "Image      : "
                 << student.imagePath << endl;
        }
    }


    // ---------------- VIEW STUDENTS ----------------

    void viewStudents()
    {
        vector<Student> students =
            fileManager.loadStudents();

        cout << "\n";
        cout << "========================================\n";
        cout << "          REGISTERED STUDENTS\n";
        cout << "========================================\n";


        if (students.empty())
        {
            cout << "No students registered.\n";
            return;
        }


        cout << left
             << setw(12) << "ID"
             << setw(22) << "Name"
             << setw(15) << "Roll"
             << setw(22) << "Department"
             << setw(16) << "Contact"
             << endl;

        cout << string(87, '-') << endl;


        for (const auto& student : students)
        {
            cout << left
                 << setw(12) << student.id
                 << setw(22) << student.name
                 << setw(15) << student.roll
                 << setw(22) << student.department
                 << setw(16) << student.contact
                 << endl;
        }
    }
};


// ============================================================
//                  AI IDENTIFICATION
// ============================================================

class AIIdentification
{
private:

    FileManager& fileManager;
    ImageMatcher matcher;


public:

    AIIdentification(FileManager& fm)
        : fileManager(fm)
    {
    }


    string identifyStudent(
        const string& inputImage)
    {
        vector<Student> students =
            fileManager.loadStudents();


        if (students.empty())
        {
            cout << "No registered students found.\n";
            return "";
        }


        if (!fs::exists(inputImage))
        {
            cout << "Error: Attendance image does not exist.\n";
            return "";
        }


        cout << "\n";
        cout << "----------------------------------------\n";
        cout << "       IDENTIFYING STUDENT...\n";
        cout << "----------------------------------------\n";


        for (const auto& student : students)
        {
            if (matcher.filesMatch(
                    inputImage,
                    student.imagePath))
            {
                cout << "\nStudent Identified!\n";

                cout << "Student ID : "
                     << student.id << endl;

                cout << "Name       : "
                     << student.name << endl;

                return student.id;
            }
        }


        cout << "\nUnknown Student Detected!\n";

        return "";
    }
};


// ============================================================
//                  ATTENDANCE MANAGER
// ============================================================

class AttendanceManager
{
private:

    FileManager& fileManager;
    AIIdentification& identification;


public:

    AttendanceManager(
        FileManager& fm,
        AIIdentification& ai)
        : fileManager(fm),
          identification(ai)
    {
    }


    // ---------------- DUPLICATE CHECK ----------------

    bool alreadyMarkedToday(
        const string& studentId)
    {
        vector<Attendance> records =
            fileManager.loadAttendance();

        string today =
            getCurrentDate();


        for (const auto& record : records)
        {
            if (record.studentId == studentId &&
                record.date == today &&
                record.status == "Present")
            {
                return true;
            }
        }

        return false;
    }


    // ---------------- MARK ATTENDANCE ----------------

    void markAttendance(
        const string& studentId)
    {
        vector<Student> students =
            fileManager.loadStudents();


        for (const auto& student : students)
        {
            if (student.id == studentId)
            {

                if (alreadyMarkedToday(studentId))
                {
                    cout << "\n";
                    cout << "Attendance already marked "
                         << "for "
                         << student.name
                         << " today.\n";

                    return;
                }


                Attendance attendance;

                attendance.studentId =
                    student.id;

                attendance.name =
                    student.name;

                attendance.date =
                    getCurrentDate();

                attendance.time =
                    getCurrentTime();

                attendance.status =
                    "Present";


                if (fileManager.saveAttendance(
                        attendance))
                {
                    cout << "\n";
                    cout << "========================================\n";
                    cout << "          ATTENDANCE MARKED\n";
                    cout << "========================================\n";

                    cout << "Student : "
                         << attendance.name
                         << endl;

                    cout << "ID      : "
                         << attendance.studentId
                         << endl;

                    cout << "Date    : "
                         << attendance.date
                         << endl;

                    cout << "Time    : "
                         << attendance.time
                         << endl;

                    cout << "Status  : "
                         << attendance.status
                         << endl;
                }

                return;
            }
        }


        cout << "Student not found.\n";
    }


    // ---------------- START ATTENDANCE ----------------

    void startAttendance()
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "          START ATTENDANCE\n";
        cout << "========================================\n";


        clearInput();

        string imagePath;


        cout << "Enter attendance image path: ";

        getline(cin, imagePath);


        if (isBlank(imagePath))
        {
            cout << "Error: Image path cannot be empty.\n";
            return;
        }


        string studentId =
            identification.identifyStudent(
                imagePath);


        if (!studentId.empty())
        {
            markAttendance(studentId);
        }
    }


    // ---------------- TODAY ATTENDANCE ----------------

    void viewTodayAttendance()
    {
        vector<Attendance> records =
            fileManager.loadAttendance();

        string today =
            getCurrentDate();


        cout << "\n";
        cout << "========================================\n";
        cout << "          TODAY'S ATTENDANCE\n";
        cout << "Date: " << today << endl;
        cout << "========================================\n";


        bool found = false;


        cout << left
             << setw(12) << "ID"
             << setw(22) << "Name"
             << setw(18) << "Time"
             << setw(12) << "Status"
             << endl;

        cout << string(64, '-') << endl;


        for (const auto& attendance : records)
        {
            if (attendance.date == today)
            {
                found = true;

                cout << left
                     << setw(12)
                     << attendance.studentId

                     << setw(22)
                     << attendance.name

                     << setw(18)
                     << attendance.time

                     << setw(12)
                     << attendance.status

                     << endl;
            }
        }


        if (!found)
        {
            cout << "No attendance records "
                 << "for today.\n";
        }
    }


    // ---------------- STUDENT HISTORY ----------------

    void viewStudentAttendance()
    {
        clearInput();

        string studentId;


        cout << "\nEnter Student ID: ";

        getline(cin, studentId);


        vector<Attendance> records =
            fileManager.loadAttendance();


        bool found = false;


        cout << "\n";
        cout << "========================================\n";
        cout << "       STUDENT ATTENDANCE HISTORY\n";
        cout << "========================================\n";


        cout << left
             << setw(15) << "Student ID"
             << setw(22) << "Name"
             << setw(15) << "Date"
             << setw(18) << "Time"
             << setw(12) << "Status"
             << endl;


        cout << string(82, '-') << endl;


        for (const auto& attendance : records)
        {
            if (attendance.studentId == studentId)
            {
                found = true;


                cout << left
                     << setw(15)
                     << attendance.studentId

                     << setw(22)
                     << attendance.name

                     << setw(15)
                     << attendance.date

                     << setw(18)
                     << attendance.time

                     << setw(12)
                     << attendance.status

                     << endl;
            }
        }


        if (!found)
        {
            cout << "No attendance records "
                 << "found for this student.\n";
        }
    }


    // ---------------- ATTENDANCE PERCENTAGE ----------------

    void attendancePercentage()
    {
        clearInput();

        string studentId;


        cout << "\nEnter Student ID: ";

        getline(cin, studentId);


        vector<Student> students =
            fileManager.loadStudents();

        vector<Attendance> records =
            fileManager.loadAttendance();


        string studentName;

        bool studentFound = false;


        for (const auto& student : students)
        {
            if (student.id == studentId)
            {
                studentName =
                    student.name;

                studentFound = true;

                break;
            }
        }


        if (!studentFound)
        {
            cout << "Student not found.\n";
            return;
        }


        int present = 0;


        vector<string> dates;


        for (const auto& attendance : records)
        {
            if (attendance.studentId == studentId &&
                attendance.status == "Present")
            {
                present++;


                if (find(
                        dates.begin(),
                        dates.end(),
                        attendance.date)
                    == dates.end())
                {
                    dates.push_back(
                        attendance.date);
                }
            }
        }


        int totalClasses =
            static_cast<int>(dates.size());


        double percentage = 0.0;


        if (totalClasses > 0)
        {
            percentage =
                (static_cast<double>(present)
                 / totalClasses) * 100.0;
        }


        cout << "\n";
        cout << "========================================\n";
        cout << "       ATTENDANCE PERCENTAGE\n";
        cout << "========================================\n";


        cout << "Student ID    : "
             << studentId << endl;

        cout << "Name          : "
             << studentName << endl;

        cout << "Total Classes : "
             << totalClasses << endl;

        cout << "Present       : "
             << present << endl;

        cout << "Percentage    : "
             << fixed
             << setprecision(2)
             << percentage
             << "%" << endl;
    }
};


// ============================================================
//                          MAIN
// ============================================================

int main()
{
    FileManager fileManager;

    AdminAuthentication admin;


    cout << "\n";
    cout << "========================================\n";
    cout << "        AI ATTENDANCE SYSTEM\n";
    cout << "========================================\n";


    // ADMIN LOGIN
    if (!admin.login())
    {
        return 0;
    }


    // CREATE OBJECTS
    StudentManager studentManager(
        fileManager);

    AIIdentification aiIdentification(
        fileManager);

    AttendanceManager attendanceManager(
        fileManager,
        aiIdentification);


    int choice;


    // MAIN MENU
    while (true)
    {
        cout << "\n\n";
        cout << "========================================\n";
        cout << "              MAIN MENU\n";
        cout << "========================================\n";

        cout << "1. Register Student\n";
        cout << "2. View Students\n";
        cout << "3. Start Attendance\n";
        cout << "4. View Today's Attendance\n";
        cout << "5. View Student Attendance\n";
        cout << "6. View Attendance Percentage\n";
        cout << "7. Exit\n";

        cout << "========================================\n";

        cout << "Enter your choice: ";


        if (!(cin >> choice))
        {
            cout << "\nInvalid input.\n";

            clearInput();

            continue;
        }


        switch (choice)
        {
            case 1:

                studentManager.registerStudent();

                break;


            case 2:

                studentManager.viewStudents();

                break;


            case 3:

                attendanceManager.startAttendance();

                break;


            case 4:

                attendanceManager.viewTodayAttendance();

                break;


            case 5:

                attendanceManager.viewStudentAttendance();

                break;


            case 6:

                attendanceManager.attendancePercentage();

                break;


            case 7:

                cout << "\n";
                cout << "Thank you for using "
                     << "AI Attendance System!\n";

                return 0;


            default:

                cout << "\nInvalid choice. "
                     << "Please try again.\n";
        }
    }


    return 0;
}