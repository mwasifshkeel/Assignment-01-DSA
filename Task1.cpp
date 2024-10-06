#include <iostream>
#include <random> //For random execution time allotment
#include <string>

using namespace std;

// Processes will be nodes
class process
{
public:
  int process_id;     // ID of the process
  int execution_time; // Time needed to execute
  int remaining_time; // Time left till completion of execution
  process *next;      // Address of next process
};

// List to hold processes
class CircularLinkedList
{
public:
  process *list = nullptr;        // Points to end process of the circular list
  int length = 0;                 // Number of processes in list
  process *loc = nullptr;         // Location of process being searched
  process *ploc = nullptr;        // Location of process previous to process being searched
  const static int CPU_TIME = 10; // Fixed CPU Time for each process

  bool isEmpty() { return length == 0; } // Checks if list is empty

  void PrintList()
  {
    if (!isEmpty())
    {
      process *temp = list->next; // Starts at first node

      bool isFirst = true;
      do
      {
        if (!isFirst) // Do not print , if it was the first process being printed
        {
          cout << ", ";
        }
        cout << "P" << temp->process_id << " "    // Prints process id
             << (temp->remaining_time <= CPU_TIME // execution time
                     ? "(Completes)"
                     : "(Remaining: " + to_string(temp->remaining_time) + ")"); // Remaining time
        temp = temp->next;                                                      // Moves on to next process
        isFirst = false;
      } while (temp != list->next); // Continues until first node is reached
      cout << endl;
    }
    else
    {
      cout << "No More Processes" << endl; // List is empty
    }
  }

  void Search(int processID)
  {
    loc = nullptr;  // Initialize loc
    ploc = nullptr; // and ploc to null
    if (isEmpty())
    {
      return;
    }
    loc = list->next;                                  // start from first node
    ploc = list;                                       // keep track of previous node
    while (loc != list && loc->process_id < processID) // Loop over the list until end is reached or theoretical position of value in list is found
    {
      ploc = loc;
      loc = loc->next;
    }
    if (loc == list && loc->process_id < processID) // if loc becomes last node and value is less than process id means value was not found
    {
      ploc = list;
      loc = nullptr; // Value not found
      return;
    }
    if (loc->process_id != processID) // That value does not exist in list
    {
      loc = nullptr;
    }
  }

  void InsertAtEnd(int processID)
  {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(10, 100); // Generate random number b/w 10 & 100
    process *newprocess = new process();              // Create the new process
    newprocess->process_id = processID;
    newprocess->execution_time = dist(mt);
    newprocess->remaining_time = newprocess->execution_time;
    if (isEmpty())
    {
      newprocess->next = newprocess; // If list is empty it becomes first and last node;
      list = newprocess;
    }
    else
    {
      newprocess->next = list->next; // It points to first node
      list->next = newprocess;       // Last node points to new node
      list = newprocess;             // New node becomes last node
    }
    length++; // Process added
  }

  void Delete(int processID)
  {
    Search(processID);  // Get loc and ploc of processID
    if (loc != nullptr) // Check if it exists
    {
      if (list->next != list)
      {
        ploc->next = loc->next; // Previous pointer should point to the node it was pointing to
        if (loc == list)
        {
          list = ploc; // if this node was last then ploc will be new last node
        }
        delete loc; // Free memory
      }
      else
      {
        delete list;    // It is only node in list
        list = nullptr; // Reset list pointer
      }
      length--; // Process removed
    }
    else
    {
      cout << "Process not found. Deletion not possible." << endl;
    }
  }

  void DestroyList() // Delete all the nodes and reset pointer
  {
    if (!isEmpty())
    {
      process *temp;
      while (list->next != list)
      {
        temp = list->next;
        list->next = list->next->next;
        delete temp;
      }
      delete list;
      list = nullptr;
    }
  }

  void executeProcesses() // Scheduling Algorithm
  {
    process *current;
    int cycle = 1; // Initialize cycle to 1

    while (!isEmpty()) // As long as list is not empty
    {
      current = list->next;                 // Initialize current to first node
      while (!isEmpty() && current != list) // Loop until list is not empty and last node not reached
      {
        current->remaining_time -= CPU_TIME; // Execute process for CPU_TIME

        if (current->remaining_time <= 0) // If remaining time becomes zero or less
        {
          int processID = current->process_id; // Store ID of current process
          current = current->next;             // Move to next process
          Delete(processID);                   // Delete the process whose execution was over
        }
        else
        {
          current = current->next; // Move to next process
        }
      }
      current->remaining_time -= CPU_TIME; // Last node is skipped due to while loop

      if (current->remaining_time <= 0) // Check if last node execution is over
      {
        int processID = current->process_id;
        Delete(processID);
      }

      cout << "Cycle " << cycle << ": "; // Display state of processes after cycle
      PrintList();

      // Optional Task 2
      if (cycle == 2) // Randomly add process in Cycle 2
      {
        InsertAtEnd(5);
        cout << "New process arrives: P" << list->process_id
             << " (Remaining: " << list->remaining_time << ")" << endl;
      }

      cycle++; // Increment cycle
    }
  }
};

int main()
{
  CircularLinkedList scheduler; // Make a scheduler list

  // Insert processes
  scheduler.InsertAtEnd(1);
  scheduler.InsertAtEnd(2);
  scheduler.InsertAtEnd(3);
  scheduler.InsertAtEnd(4);

  // Display initial state of processes in list
  cout << "Initial Processes: ";
  scheduler.PrintList();

  // Display CPU Time for each process
  cout << "CPU Time per Process per Cycle: " << CircularLinkedList::CPU_TIME
       << endl;

  // Start scheduler
  scheduler.executeProcesses();

  // Free memory
  scheduler.DestroyList();
}