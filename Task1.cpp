#include <iostream>
#include <random>
#include <string>

using namespace std;

class process {
public:
  int process_id;
  int execution_time;
  int remaining_time;
  process *next;
};

class CircularLinkedList {
public:
  process *list = nullptr;
  int length = 0;
  process *loc = nullptr;
  process *ploc = nullptr;
  const static int CPU_TIME = 10;

  bool isEmpty() { return length == 0; }

  void PrintList() {
    if (!isEmpty()) {
      process *temp = list->next;

      bool isFirst = true;
      do {
        if (!isFirst) {
          cout << ", ";
        }
        cout << "P" << temp->process_id << " "
             << (temp->remaining_time <= CPU_TIME
                     ? "(Completes)"
                     : "(Remaining: " + to_string(temp->remaining_time) + ")");
        temp = temp->next;
        isFirst = false;
      } while (temp != list->next);
      cout << endl;
    } else {
      cout << "No More Processes" << endl;
    }
  }

  void Search(int pid) {
    loc = nullptr;
    ploc = nullptr;
    if (isEmpty()) {
      return;
    }
    loc = list->next;
    ploc = list;
    while (loc != list && loc->process_id < pid) {
      ploc = loc;
      loc = loc->next;
    }
    if (loc == list && loc->process_id < pid) {
      ploc = list;
      loc = nullptr;
      return;
    }
    if (loc->process_id != pid) {
      loc = nullptr;
    }
  }

  void InsertAtEnd(int pid) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(10, 100);
    process *newprocess = new process();
    newprocess->process_id = pid;
    newprocess->execution_time = dist(mt);
    newprocess->remaining_time = newprocess->execution_time;
    if (isEmpty()) {
      newprocess->next = newprocess;
      list = newprocess;
    } else {
      newprocess->next = list->next;
      list->next = newprocess;
      list = newprocess;
    }
    length++;
  }

  void Delete(int pid) {
    Search(pid);
    if (loc != nullptr) {
      if (list->next != list) {
        ploc->next = loc->next;
        if (loc == list) {
          list = ploc;
        }
        delete loc;
      } else {
        delete list;
        list = nullptr;
      }
      length--;
    } else {
      cout << "Process not found. Deletion not possible." << endl;
    }
  }

  void DestroyList() {
    if (!isEmpty()) {
      process *temp;
      while (list->next != list) {
        temp = list->next;
        list->next = list->next->next;
        delete temp;
      }
      delete list;
      list = nullptr;
    }
  }

  void executeProcesses() {
    process *current;
    int cycle = 1;

    while (!isEmpty()) {
      current = list->next;
      while (!isEmpty() && current != list) {
        current->remaining_time -= CPU_TIME;

        if (current->remaining_time <= 0) {
          int pid = current->process_id;
          current = current->next;
          Delete(pid);
        } else {
          current = current->next;
        }
      }
      current->remaining_time -= CPU_TIME;

      if (current->remaining_time <= 0) {
        int pid = current->process_id;
        Delete(pid);
      }

      cout << "Cycle " << cycle << ": ";
      PrintList();
      if (cycle == 2) {
        InsertAtEnd(5);
        cout << "New process arrives: P" << list->process_id
             << " (Remaining: " << list->remaining_time << ")" << endl;
      }

      cycle++;
    }
  }
};

int main() {
  CircularLinkedList test;
  test.InsertAtEnd(1);
  test.InsertAtEnd(2);
  test.InsertAtEnd(3);
  test.InsertAtEnd(4);
  cout << "Initial Processes: ";
  test.PrintList();
  cout << "CPU Time per Process per Cycle: " << CircularLinkedList::CPU_TIME
       << endl;
  test.executeProcesses();
  test.DestroyList();
}