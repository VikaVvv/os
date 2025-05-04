#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>

const int MAX_MSG_LEN = 20;

struct SharedData {
    sem_t input_ready;
    sem_t output_ready;
    sem_t mutex;
    char message[MAX_MSG_LEN + 1];
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_name>\n";
        return 1;
    }

    // Открываем разделяемую память
    int shm_fd = shm_open("/shared_mem", O_RDWR, 0666);
    SharedData* shared = (SharedData*)mmap(NULL, sizeof(SharedData), 
                                         PROT_READ | PROT_WRITE, 
                                         MAP_SHARED, shm_fd, 0);

    while (true) {
        std::cout << "\nEnter 1 to send message\nEnter 0 to exit\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            sem_wait(&shared->output_ready);
            sem_wait(&shared->mutex);

            std::cout << "Enter message (max " << MAX_MSG_LEN << " chars): ";
            std::string msg;
            std::cin >> msg;
            msg.copy(shared->message, MAX_MSG_LEN);
            shared->message[msg.length()] = '\0';

            sem_post(&shared->mutex);
            sem_post(&shared->input_ready);
        } else if (choice == 0) {
            break;
        }
    }

    munmap(shared, sizeof(SharedData));
    return 0;
}