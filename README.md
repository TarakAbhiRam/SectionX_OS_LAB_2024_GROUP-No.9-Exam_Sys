# Exam-Sys-Team9
OSminiproject-MCQ-Based Exam Platform that can be used for conducting online exams
# MCQ-Based Examination Platform

## **Overview**
This project is a multi-user platform designed to facilitate online examinations. It provides functionalities for students and teachers, enabling seamless registration, login, exam-taking, and result generation.

The platform is implemented using C++ with a client-server architecture. It includes features like secure communication, concurrency handling, question management, and result processing.

---

## **Key Features**
1. **Student Functionality:**
   - Register with roll number and department.
   - Login securely to access exams.
   - Attempt MCQs with shuffled questions.
   - View marks and leaderboard after completion.

2. **Teacher Functionality:**
   - Register with teacher ID and department.
   - Login securely to set exam questions.
   - Modify and view question banks.
   - Access leaderboards for performance tracking.

3. **Server Features:**
   - Handles multiple clients using sockets and concurrency.
   - Ensures secure communication and error handling.
   - Synchronizes data for exams, results, and leaderboards.

4. **Question Management:**
   - Questions stored and retrieved efficiently.
   - Teachers can add, view, or modify questions.
   - Students receive shuffled questions during exams.

5. **Result Processing:**
   - Grades calculated based on correct responses.
   - Leaderboard generated for students and teachers.

---

## **Technologies and Tools Used**
- **Programming Language:** C++
- **System Libraries:** `<sys/socket.h>`, `<unistd.h>`, `<vector>`
- **Concurrency Management:** Semaphores for synchronization.
- **File Handling:** Used for persistent storage of user data and questions.
- **Sockets:** For communication between clients and server.
- **Version Control:** Git for collaboration.

---

## **System Architecture**
The platform follows a client-server model with clearly defined responsibilities:
- **Server:**
  - Manages user registrations and logins.
  - Sends exam questions to clients.
  - Processes responses and calculates results.
- **Client:**
  - Handles user input (registration, login, answering questions).
  - Communicates with the server to send/receive data.

---

## **Project Structure**
```
.
├── Makefile               # Build automation
├── README.md              # Project documentation
├── server/
│   ├── server.cpp         # Server-side main logic
│   ├── server_func_implementation.cpp  # Function implementations
│   └── Templates/template.h # UI menus and templates
├── client/
│   ├── client.cpp         # Client-side main logic
│   └── client_helpers.cpp # Helper functions for client operations
└── data/
    ├── users/             # User data storage
    └── questions/         # Question banks
```

---

## **How to Build and Run**

### Prerequisites:
- C++ compiler (e.g., g++)
- Git

### Steps:
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. Build the project:
   ```bash
   make
   ```

3. Run the server:
   ```bash
   ./build/server
   ```

4. Run the client:
   ```bash
   ./build/client
   ```

---

## **Team Responsibilities**

### **Tarak and Anirban**
- Implemented server-side communication.
- Handled concurrency and synchronization using semaphores.
- Ensured secure communication and error handling.

### **Sahithi, Manoj, and Sagnick**
- Developed client-side UI for login and registration.
- Built user authentication with secure validation.
- Implemented navigation for students and teachers.

### **Suchetha and Arpon**
- Enabled teachers to set and modify exam questions.
- Handled question storage and retrieval for both teachers and students.

### **Jessica and Devansh**
- Developed grading logic and result generation.
- Built leaderboard functionality for tracking performance.

---

## **Acknowledgments**
We extend our gratitude to our mentors and teammates for their efforts in bringing this project to life. Special thanks to [Tarak Suhas Abhiram Puppala](https://github.com/TarakAbhiRam) for leading the project and providing technical guidance.

