#include <iostream>
using namespace std;

class process {
public:
  int process_id;
  float execution_time;
  float remaining_time;
  process *next;
};

class CircularLinkedList {
public:
  process *list = nullptr;
  int length = 0;
  process *loc = nullptr;
  process *ploc = nullptr;

  bool isEmpty() { return length == 0; }

  void PrintList() {
    if (!isEmpty()) {
      process *temp = list->next;
      do {
        cout << "Process ID: " << temp->process_id << " "
             << "Execution Time : " << temp->execution_time << " "
             << " Remaining Time : " << temp->remaining_time << " ";
        temp = temp->next;
      } while (temp != list->next);
      cout << endl;
    } else {
      cout << "List is Empty" << endl;
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
    process *newprocess = new process();
    newprocess->process_id = pid;
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
      cout << "Deleting Process: " << pid << endl;
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
};

int main() {
  CircularLinkedList test;
  test.PrintList();
  test.InsertAtEnd(1);
  test.PrintList();
}