# AI Attendance System

AI Attendance System is a C++ based attendance management project designed to automate student attendance using image-based identification. The system allows an administrator to register students, store their personal information and registered images, identify students through image matching, and automatically record their attendance with date and time.

## Features

- Admin Login Authentication
- Student Registration
- Student ID Duplicate Prevention
- Student Image Registration
- Image-Based Student Identification
- Automatic Attendance Recording
- Duplicate Attendance Prevention
- View Today's Attendance
- View Individual Student Attendance History
- Attendance Percentage Calculation
- File-Based Data Storage
- Error Handling and Input Validation

## Technologies Used

- C++
- Object-Oriented Programming (OOP)
- STL
- File Handling
- Data Structures
- C++17 Filesystem
- Image File Processing
- Basic AI Identification Concepts

## How It Works

First, the administrator logs into the system and registers students with their Student ID, name, roll number, department, contact information, and image path.

When attendance is started, the system takes an image path as input and compares the image with registered student images. If a matching image is found, the student is identified and their attendance is automatically recorded with the current date and time.

The system also checks whether the student's attendance has already been recorded for the current day to prevent duplicate attendance.

## Project Structure

AI-Attendance-system/
├── Ai.cpp
├── data/
│   ├── students.txt
│   └── attendance.txt
├── images/
│   └── registered/
└── reports/

## Future Enhancements

- Real-time webcam integration
- OpenCV-based face detection
- Real-time face recognition
- Multiple face detection
- Unknown face detection
- CSV attendance reports
- Monthly attendance reports
- Low attendance warning
- Database integration
- GUI dashboard
- Attendance analytics

## Note

The current version uses image-file matching for student identification. It is a basic implementation of the identification concept and is not a complete facial recognition system. OpenCV-based real-time face recognition can be integrated in future versions.

## Author

Ritik Patel

B.Tech – Artificial Intelligence & Robotics
