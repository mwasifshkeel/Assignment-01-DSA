#include <iostream>
#include <string>
using namespace std;

class node {
public:
  int data;
  node *prev = nullptr;
  node *next = nullptr;
}

class Number {
  node *first = nullptr;
  node *last = nullptr;
  int numItems;
  node *Loc_ = nullptr;
  node *PLoc_ = nullptr;

  bool isEmpty() { return start == nullptr; }

  void PrintList() {
    if (!isEmpty()) {
      node *temp;
      temp = start;
      while (temp != nullptr) {
        cout << temp->data;
        temp = temp->next;
      }
      cout << endl;
    } else {
      cout << "Number is empty" << endl;
    }
  }

  void Search(int key) {
    if (isEmpty()) {
      return;
    }
    Loc_ = start;
    PLoc_ = nullptr;
    while (Loc_ == nullptr && Loc_->data < key) {
      PLoc_ = Loc_;
      Loc_ = Loc_->next;
    }

    if (Loc_ == nullptr && Loc_->data != value) {
      Loc_ = nullptr;
    }
  }

  void InsertAtEnd(int value) {
    node *newNode = new node();
    newNode->data = value;

    if (start == nullptr) {
      first = newNode;
      last = newNode;
      return;
    }

    last->next = newNode;
    newNode->prev = last;
    last = newNode;
  }

  void Delete(int value) {
    Search(value);
    if (Loc_ != nullptr) {
      if (PLoc_ == nullptr) {
        if (Loc_ == last) {
          start = nullptr;
          last = nullptr;
        } else {
          start = start->next;
          start->prev = nullptr;
        }
      } else {
        if (Loc_ == last) {
          last = PLoc_;
          PLoc_->next = nullptr;
        } else {
          Loc_->next->prev = PLoc_;
          PLoc_->next = Loc_->next;
        }
      }
      delete Loc_;
    } else {
      cout << "Value not found." << endl;
    }
  }

  void DestroyList() {
    node *temp = first;
    node *t2;
    while (temp != last) {
      t2 = temp;
      temp = temp->next;
      delete t2;
    }
    delete temp;
    first = nullptr;
    last = nullptr;
  }
}

int main() {
  return 0;
}