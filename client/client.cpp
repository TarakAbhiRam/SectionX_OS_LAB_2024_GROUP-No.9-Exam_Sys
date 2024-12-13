// C program for the Client Side
#include "../Templates/template.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// inet_addr
#include <arpa/inet.h>
#include <unistd.h>

// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>
#define PORT 8080

using namespace std;

// Function to send data to server socket.
void *clienthread(void *args)
{
    printf("--- Initializing New Client Connection ---\n");

    // Creating and initializing a new client object
    Client new_client; // Calls client class constructor
    printf("Client object created successfully.\n");

    pthread_exit(NULL);
    return 0;
}

// Driver Code
int main()
{
    printf("--- Starting Client Program ---\n");

    pthread_t tid;

    printf("Creating client thread...\n");
    // Create a new thread for handling client operations
    pthread_create(&tid, NULL, clienthread, NULL);
    
    // Wait for the client thread to complete
    pthread_join(tid, NULL);

    printf("--- Client Program Execution Completed ---\n");

    return 0;
}
