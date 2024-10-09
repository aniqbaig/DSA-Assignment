#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
using namespace std;

struct Process {
    int process_id;
    int execution_time;
    int remaining_time;
    Process* next;

    Process(int id, int exec_time) {
        process_id = id;
        execution_time = exec_time;
        remaining_time = exec_time;
        next = nullptr;
    }
};

class Scheduler {
private:
    Process* head;
    int time_per_cycle;

public:
    Scheduler(int tpc) : head(nullptr), time_per_cycle(tpc) {}

    // Add a process to the circular linked list
    void addProcess(int process_id, int execution_time) {
        Process* newProcess = new Process(process_id, execution_time);
        if (head == nullptr) {
            head = newProcess;
            head->next = head; // Circular linked list
        } else {
            Process* temp = head;
            while (temp->next != head) {
                temp = temp->next;
            }
            temp->next = newProcess;
            newProcess->next = head;
        }
    }

    // Display initial processes
    void displayInitialProcesses() {
        if (head == nullptr) return;

        cout << "Initial Processes: [";
        Process* temp = head;
        do {
            cout << "(P" << temp->process_id << ", " << temp->execution_time << ")";
            temp = temp->next;
            if (temp != head) cout << ", ";
        } while (temp != head);
        cout << "]\n";
    }

    // Remove a completed process from the circular linked list
    void removeProcess(Process* prev, Process* current) {
        if (current == head && current->next == head) {
            // Only one process in the list
            head = nullptr;
        } else {
            if (current == head) {
                // Update head if we're removing the head process
                head = head->next;
            }
            prev->next = current->next; // Remove current process
        }
        delete current;
    }

    // Run the scheduling algorithm with the option to add a new process after a specific cycle
    void simulate() {
        if (head == nullptr) return;

        Process* current = head;
        Process* prev = nullptr;
        int cycle = 1;

        // Display the CPU time per process per cycle
        cout << "CPU Time per Process per Cycle: " << time_per_cycle << "\n";

        while (head != nullptr) {
            cout << "Cycle " << cycle << ": ";

            Process* temp = head;
            do {
                if (temp->remaining_time > 0) {
                    cout << "P" << temp->process_id << " (Remaining: " << temp->remaining_time << ") ";
                }
                temp = temp->next;
            } while (temp != head);
            cout << "\n";

            // Execute current process
            if (current->remaining_time > 0) {
                if (current->remaining_time > time_per_cycle) {
                    current->remaining_time -= time_per_cycle;
                } else {
                    current->remaining_time = 0;
                }
                cout << "P" << current->process_id << " -> (Remaining: " << current->remaining_time << ")\n";

                // Remove completed process
                if (current->remaining_time == 0) {
                    cout << "P" << current->process_id << " completes.\n";
                    if (prev == nullptr) {
                        removeProcess(current, current);
                        current = head;
                    } else {
                        Process* temp = current->next;
                        removeProcess(prev, current);
                        current = temp;
                    }
                } else {
                    prev = current;
                    current = current->next;
                }
            }

            // After cycle 2, add a new process (for example)
            if (cycle == 2) {
                addProcess(4, 9); // New process P4 arrives
                cout << "New process arrives: P4 (Remaining: 9)\n";
            }

            cycle++;
        }

        cout << "All processes completed.\n";
    }
};

// Function to manually add initial processes
void initializeProcesses(Scheduler& scheduler) {
    scheduler.addProcess(1, 10);
    scheduler.addProcess(2, 5);
    scheduler.addProcess(3, 8);
}

int main() {
    Scheduler scheduler(3); // Time per cycle is 3 units

    // Initialize predefined processes
    initializeProcesses(scheduler);

    // Display initial processes
    scheduler.displayInitialProcesses();

    // Simulate the scheduling algorithm
    scheduler.simulate();

    return 0;
}

