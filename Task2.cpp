#include <iostream>
#include <string>
using namespace std;

class node {
public:
  string data;
  node *prev = nullptr;
  node *next = nullptr;
};

class Number {
public:
  node *first = nullptr;
  node *last = nullptr;
  int numItems;
  node *Loc_ = nullptr;
  node *PLoc_ = nullptr;

  bool isEmpty() { return first == nullptr; }

  void PrintList() {
    if (!isEmpty()) {
      node *temp;
      temp = first;
      while (temp != nullptr) {
        cout << temp->data;
        temp = temp->next;
      }
      cout << endl;
    } else {
      cout << "Number is empty" << endl;
    }
  }

  void Search(string key) {
    if (isEmpty()) {
      return;
    }
    Loc_ = first;
    PLoc_ = nullptr;
    while (Loc_ == nullptr && Loc_->data < key) {
      PLoc_ = Loc_;
      Loc_ = Loc_->next;
    }

    if (Loc_ == nullptr && Loc_->data != key) {
      Loc_ = nullptr;
    }
  }

  void InsertAtEnd(string value) {
    node *newNode = new node();
    newNode->data = value;

    if (first == nullptr) {
      first = newNode;
      last = newNode;
      return;
    }

    last->next = newNode;
    newNode->prev = last;
    last = newNode;
  }

  void Delete(string value) {
    Search(value);
    if (Loc_ != nullptr) {
      if (PLoc_ == nullptr) {
        if (Loc_ == last) {
          first = nullptr;
          last = nullptr;
        } else {
          first = first->next;
          first->prev = nullptr;
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
};

bool correctString(string temp) {
  if (temp.length() == 0) {
    return false;
  }
  for (int i = 0; i < temp.length(); i++) {
    if (!((temp.at(i) >= '0') && (temp.at(i) <= '9'))) {
      return false;
    }
  }
  return true;
}

string getNumber() {
  string temp = "";
  cout << "Enter your number: ";
  bool firstTime = true;
  while (!correctString(temp)) {
    if (!firstTime) {
      cout << endl
           << "You have not entered a correct number. Try Again!" << endl;
      cout << "Enter your number: ";
    }
    firstTime = false;
    getline(cin, temp);
  }
  return temp;
}

void storeNumber(Number *n, string num) {
  if (num.length() < 9) {
    (*n).InsertAtEnd(num.substr(0, num.length()));
  } else {
    for (int i = 0; i < num.length() - (num.length() % 9); i += 9) {
      (*n).InsertAtEnd(num.substr(i, 9));
    }

    (*n).InsertAtEnd(num.substr(num.length() - (num.length() % 9),
                                num.length() - (num.length() % 9)));
  }
}

int main() {
  string num = getNumber();
  Number n1;

  storeNumber(&n1, num);

  n1.PrintList();

  return 0;
}