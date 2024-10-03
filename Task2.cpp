#include <algorithm>
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
      numItems++;
      return;
    }

    last->next = newNode;
    newNode->prev = last;
    last = newNode;
    numItems++;
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

  friend Number operator+(const Number &n1, const Number &n2) {
    Number result;
    node *tempOne = n1.last;
    node *tempTwo = n2.last;
    int carry = 0;

    while (tempOne != nullptr || tempTwo != nullptr || carry > 0) {
      string partOne = (tempOne != nullptr) ? tempOne->data : "0";
      string partTwo = (tempTwo != nullptr) ? tempTwo->data : "0";

      string sumNode;
      int maxLength = max(partOne.size(), partTwo.size());

      partOne.insert(0, maxLength - partOne.size(), '0');
      partTwo.insert(0, maxLength - partTwo.size(), '0');

      for (int i = maxLength - 1; i >= 0; --i) {
        int digitOne = partOne[i] - '0';
        int digitTwo = partTwo[i] - '0';
        int digitSum = digitOne + digitTwo + carry;
        carry = digitSum / 10;
        sumNode.push_back((digitSum % 10) + '0');
      }

      if (carry > 0) {
        sumNode.push_back(carry + '0');
      }

      reverse(sumNode.begin(), sumNode.end());

      node *newNode = new node();
      newNode->data = sumNode;

      if (result.first == nullptr) {
        result.first = newNode;
        result.last = newNode;
      } else {
        newNode->next = result.first;
        result.first->prev = newNode;
        result.first = newNode;
      }

      result.numItems++;

      if (tempOne != nullptr)
        tempOne = tempOne->prev;
      if (tempTwo != nullptr)
        tempTwo = tempTwo->prev;
    }

    return result;
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
  string num2 = getNumber();

  Number n1;
  Number n2;
  Number n3;

  storeNumber(&n1, num);
  storeNumber(&n2, num2);

  n1.PrintList();
  n2.PrintList();

  n3 = n1 + n2;
  n3.PrintList();

  return 0;
}