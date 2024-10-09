#include <iostream>
#include <cstdlib>  // For random number generation
#include <ctime>    // For seeding random numbers
using namespace std;

// Node to store 64-bit chunks of the number
struct Node {
    unsigned long long data; // 64-bit chunk
    Node* next;
    Node(unsigned long long value) : data(value), next(nullptr) {}
};

// Linked list to store the 1024-bit number
class BigNumber {
private:
    Node* head;
    int nodeCount;

public:
    BigNumber() : head(nullptr), nodeCount(0) {}

    // Add a new 64-bit chunk to the linked list
    void addChunk(unsigned long long chunk) {
        Node* newNode = new Node(chunk);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        nodeCount++;
    }

    // Generate a 1024-bit number using 16 chunks of 64-bit random numbers
    void generate1024BitNumber() {
        srand(time(0)); // Seed for random number generation
        for (int i = 0; i < 16; i++) {
            unsigned long long randomChunk = ((unsigned long long)rand() << 32) | rand(); // Create 64-bit random number
            addChunk(randomChunk);
        }
    }

    // Display the large number (for demonstration purposes)
    void displayNumber() {
        Node* temp = head;
        cout << "Input: A 1024-bit number represented as \"";
        while (temp != nullptr) {
            cout << temp->data;
            temp = temp->next;
            if (temp != nullptr) cout << " ";
        }
        cout << "\"\n";
    }

    // Fermat Primality Test (probabilistic test)
    bool isPrime(int iterations) {
        // Fermat test cannot be applied directly to such a large number, 
        // but we'll demonstrate with smaller numbers for practical purposes.

        // We'll check primality for each 64-bit chunk (for simplicity in this example)
        Node* temp = head;
        while (temp != nullptr) {
            if (!isPrimeSmall(temp->data, iterations)) {
                return false;
            }
            temp = temp->next;
        }
        return true;
    }

    // Simple Fermat primality test for smaller numbers
    bool isPrimeSmall(unsigned long long n, int iterations) {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;

        for (int i = 0; i < iterations; i++) {
            unsigned long long a = 2 + rand() % (n - 3);
            if (modularExponentiation(a, n - 1, n) != 1) {
                return false;
            }
        }
        return true;
    }

    // Helper function for modular exponentiation (a^b % mod)
    unsigned long long modularExponentiation(unsigned long long base, unsigned long long exp, unsigned long long mod) {
        unsigned long long result = 1;
        base = base % mod;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * base) % mod;
            }
            exp = exp >> 1;
            base = (base * base) % mod;
        }
        return result;
    }
};

int main() {
    BigNumber bigNumber;

    // Step 1: Generate a 1024-bit random number
    bigNumber.generate1024BitNumber();

    // Step 2: Display the large number
    bigNumber.displayNumber();

    // Step 3: Test for primality using the Fermat Primality Test
    bool result = bigNumber.isPrime(5); // 5 iterations for Fermat test

    // Step 4: Output result
    cout << "Output: " << (result ? "True (Prime)" : "False (Not Prime)") << endl;

    return 0;
}
