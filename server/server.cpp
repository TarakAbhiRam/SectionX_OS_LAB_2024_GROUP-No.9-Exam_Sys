#include "../Templates/template.h"
#include <arpa/inet.h>

// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <string>
#define PORT 8080
using namespace std;

// semaphore variables
sem_t *student_regFileSemaphore;
sem_t *teacher_regFileSemaphore;
sem_t *readFileSemaphore;
sem_t *queFileSemaphores[4];
sem_t *resultFileSemaphores[4];
sem_t *readResultFile;

map<string,int>deptIndex ;
void initializeDeptIndex()
{
	deptIndex["CS"] = 0;
	deptIndex["ECE"] = 1;
	deptIndex["EEE"] = 2;
	deptIndex["MECH"] = 3;
}
Question deptQuestionBank[4];

void parseQuestionFiles()
{
	for(auto it:deptIndex)
	{
		addQuestionFromFile(it.first,deptQuestionBank[it.second]);
	}
}

void *clientConnection(void *param)
{
    cout << "\n--- New Client Connected to Server ---\n";
    int success_code = SUCCESSFUL_CODE;
    int newSocket = *((int *)param);
    bool endflag = false;
    pthread_t ptid = pthread_self();
    int choice;
    while (1)
    {
        // Receive a request code from the client
        recv(newSocket, &choice, sizeof(choice), 0);
        cout << "\n--- Received Client Request Code: " << choice << " ---\n";

        switch (choice)
        {
        case END_CONNECTION_CODE:
        {
            cout << "Client requested to end the connection.\n";
            endflag = true;
            break;
        }

        case REGISTRATION_CODE:
        {
            cout << "Registration process initiated.\n";
            char usertype;
            recv(newSocket, &usertype, sizeof(usertype), 0);
            if (usertype == 'S')
            {
                sem_wait(student_regFileSemaphore);
                server_side_student_registration(newSocket);
                sem_post(student_regFileSemaphore);
                cout << "Student registration completed.\n";
            }
            else
            {
                sem_wait(teacher_regFileSemaphore);
                server_side_teacher_registration(newSocket);
                sem_post(teacher_regFileSemaphore);
                cout << "Teacher registration completed.\n";
            }
            send(newSocket, &success_code, sizeof(success_code), 0);
            break;
        }
        case LOGIN_CODE:
        {
            cout << "Login process started.\n";
            sem_post(readFileSemaphore);
            server_side_login(newSocket);
            sem_post(readFileSemaphore);
            send(newSocket, &success_code, sizeof(success_code), 0);
            cout << "Login completed.\n";
            break;
        }
        case SET_QUESTION_CODE:
        {
            cout << "Question setting initiated.\n";
            char dept[10];
            recv(newSocket, &dept, sizeof(dept), 0);
            int index = deptIndex[dept];
            sem_wait(queFileSemaphores[index]);
            setQuestion(newSocket, dept, deptQuestionBank[index]);
            sem_post(queFileSemaphores[index]);
            cout << "Question setting completed for department: " << dept << ".\n";
            break;
        }
        case START_EXAM_CODE:
        {
            cout << "Starting exam process.\n";
            char dept[10];
            recv(newSocket, &dept, sizeof(dept), 0);
            int index = deptIndex[dept];
            char id[100];
            recv(newSocket, &id, sizeof(id), 0);
            deptQuestionBank[index].shuffleQuestions();
            int marksObtained = deptQuestionBank[index].startExam(newSocket);
            if (marksObtained == -1)
            {
                cout << "Exam terminated by the client.\n";
                break;
            }
            sem_wait(resultFileSemaphores[index]);
            updateResult(id, dept, marksObtained);
            sem_post(resultFileSemaphores[index]);
            cout << "Exam completed and result updated for student: " << id << ".\n";
            break;
        }
        case LEADERBOARD_CODE:
        {
            cout << "Fetching leaderboard.\n";
            char dept[10];
            recv(newSocket, &dept, sizeof(dept), 0);
            sem_wait(readResultFile);
            getLeaderboard(newSocket, dept);
            sem_post(readResultFile);
            cout << "Leaderboard sent for department: " << dept << ".\n";
            break;
        }
        case SEE_QUESTION_CODE:
        {
            cout << "Fetching questions for review.\n";
            char dept[10];
            recv(newSocket, &dept, sizeof(dept), 0);
            int index = deptIndex[dept];
            deptQuestionBank[index].sendQuestions(newSocket);
            cout << "Questions sent for department: " << dept << ".\n";
            break;
        }
        default:
            cout << "Invalid request code received.\n";
            break;
        }

        if (endflag)
        {
            cout << "--- Client Connection Ended ---\n";
            break;
        }
    }
    pthread_exit(&ptid);
}

int main()
{
    cout << "--- Server Starting ---\n";
    initializeDeptIndex();
    parseQuestionFiles();
    cout << "Department indices and question files initialized.\n";

    pthread_t thread[10000];
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    cout << "Initializing semaphores...\n";
    char semaphoreName[16];
    char resultSemaphoreName[25];
    student_regFileSemaphore = sem_open(SEMAPHORE_NAME1, O_CREAT, 0660, 1);
    teacher_regFileSemaphore = sem_open(SEMAPHORE_NAME2, O_CREAT, 0660, 1);
    readFileSemaphore = sem_open(SEMAPHORE_NAME3, O_CREAT, 0660, 1);
    readResultFile = sem_open(SEMAPHORE_NAME4, O_CREAT, 0660, 1);
    for (int i = 0; i < 4; i++)
    {
        sprintf(semaphoreName, "my_semaphore_%d", i);
        sprintf(resultSemaphoreName, "result_semaphore_%d", i);
        queFileSemaphores[i] = sem_open(semaphoreName, O_CREAT, 0660, 1);
        resultFileSemaphores[i] = sem_open(resultSemaphoreName, O_CREAT, 0660, 1);
    }
    cout << "Semaphores initialized.\n";

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        cerr << "Socket creation failed.\n";
        exit(0);
    }

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (::bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cerr << "Bind failed.\n";
        exit(0);
    }

    if (listen(serverSocket, 50) == 0)
        cout << "Server is listening on port " << PORT << "...\n";
    else
        cerr << "Error in listen.\n";

    int i = 0;

    while (1)
    {
        addr_size = sizeof(serverStorage);
        newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);
        if (newSocket < 0)
        {
            cerr << "Accept failed.\n";
            exit(0);
        }

        cout << "\n--- New Client Connected ---\n";

        if (pthread_create(&thread[i++], NULL, clientConnection, &newSocket) != 0)
        {
            cerr << "Failed to create client thread.\n";
        }
    }

    return 0;
}

