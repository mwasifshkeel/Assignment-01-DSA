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
  int numItems = 0;
  node *Loc_ = nullptr;
  node *PLoc_ = nullptr;
  int length = 0;

  bool isEmpty() const { return first == nullptr; }

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

  string GetNumber() const {
    string num = "";
    if (!isEmpty()) {
      node *temp;
      temp = first;
      while (temp != nullptr) {
        num += temp->data;
        temp = temp->next;
      }
      return num;
    }
    return "";
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

  void InsertAtFront(string value) {
    node *newNode = new node();
    newNode->data = value;

    if (first == nullptr) {
      first = newNode;
      last = newNode;
      numItems++;
      length += value.length();
      return;
    }

    first->prev = newNode;
    newNode->next = first;
    first = newNode;
    numItems++;
    length += value.length();
  }

  void InsertAtEnd(string value) {
    node *newNode = new node();
    newNode->data = value;

    if (first == nullptr) {
      first = newNode;
      last = newNode;
      numItems++;
      length += value.length();
      return;
    }

    last->next = newNode;
    newNode->prev = last;
    last = newNode;
    numItems++;
    length += value.length();
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

    while (tempOne != nullptr || tempTwo != nullptr) {
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

      result.InsertAtFront(sumNode);

      if (tempOne != nullptr)
        tempOne = tempOne->prev;
      if (tempTwo != nullptr)
        tempTwo = tempTwo->prev;
    }

    return result;
  }

  friend Number operator+(const Number &n1, const int n2) {
    Number result;
    node *tempOne = n1.last;
    int carry = n2;

    while (tempOne != nullptr) {
      string partOne = (tempOne != nullptr) ? tempOne->data : "0";
      string sumNode;

      while (!partOne.empty()) {
        int digitOne = (partOne.size() > 0) ? partOne.back() - '0' : 0;
        int digitSum = digitOne + carry;

        carry = digitSum / 10;
        sumNode.push_back((digitSum % 10) + '0');

        partOne.pop_back();
      }

      reverse(sumNode.begin(), sumNode.end());

      tempOne = tempOne->prev;

      result.InsertAtFront(sumNode);
    }

    if (carry > 0) {
      string sumNode = "";
      sumNode.push_back(carry);

      node *newNode = new node();
      newNode->data = sumNode;

      result.InsertAtFront(sumNode);
    }

    return result;
  }

  friend Number operator*(const Number &n1, const Number &n2) {
    Number result;
    Number *temp = new Number[n2.length];
    node *tempTwo = n2.last;
    int z = 0;

    while (tempTwo != nullptr) {
      node *tempOne = n1.last;
      int carry = 0;
      string productNode = "";

      string partTwo = tempTwo->data;
      productNode = "";
      for (int i = partTwo.size() - 1; i >= 0; --i) {
        productNode = "";
        carry = 0;
        tempOne = n1.last;
        while (tempOne != nullptr) {
          string partOne = tempOne->data;
          for (int j = partOne.size() - 1; j >= 0; --j) {
            int digitOne = partOne[j] - '0';
            int digitTwo = partTwo[i] - '0';
            int digitProduct = digitOne * digitTwo + carry;
            carry = digitProduct / 10;
            productNode.push_back((digitProduct % 10) + '0');
          }
          tempOne = tempOne->prev;
        }
        if (carry > 0) {
          productNode.push_back(carry + '0');
        }

        reverse(productNode.begin(), productNode.end());
        if (z > 0) {
          int temp = z;
          while (temp > 0) {
            productNode.push_back('0');
            temp--;
          }
        }
        temp[z].InsertAtEnd(productNode);
        z++;
      }

      tempTwo = tempTwo->prev;
    }
    result = temp[0];
    for (int i = 1; i < n2.length; i++) {
      result = result + temp[i];
    }
    delete[] temp;
    return result;
  }

  friend Number operator*(const Number &n1, int n2) {
    Number result = n1;
    for (int i = 0; i < n2 - 1; i++) {
      result = result + n1;
    }
    return result;
  }

  friend Number operator-(const Number &n1, const Number &n2) {
    Number result;
    node *temp = n1.last;
    node *tempTwo = n2.last;
    int borrow = 0;
    string diffNode = "";
    while (temp != nullptr && tempTwo != nullptr) {
      string value = temp->data;
      string valueTwo = tempTwo->data;

      diffNode = "";
      while ((!value.empty()) && (!valueTwo.empty())) {
        int digitOne = int(value.back() - '0') - borrow;
        int digitTwo = int(valueTwo.back() - '0');
        int digitDiff;

        if (digitOne >= digitTwo) {
          digitDiff = digitOne - digitTwo;
          borrow = 0;
        } else {
          digitDiff = digitOne + 10 - digitTwo;
          borrow = 1;
        }
        diffNode.push_back(digitDiff + '0');
        value.pop_back();
        valueTwo.pop_back();
      }
      temp = temp->prev;
      tempTwo = tempTwo->prev;
      if (tempTwo == nullptr && !value.empty()) {
        while (!value.empty()) {
          diffNode.push_back(value.back());
          value.pop_back();
        }
      }
      reverse(diffNode.begin(), diffNode.end());
      result.InsertAtFront(diffNode);
    }
    if (tempTwo == nullptr) {
      while (temp != nullptr) {
        result.InsertAtFront(temp->data);
        temp = temp->prev;
      }
    }
    return result;
  }

  friend Number operator-(const Number &n1, int n2) {
    Number result;
    node *temp = n1.last;
    string n_temp = to_string(n2);
    int len = n_temp.length();
    int padding = n1.length - len;
    string n = "";
    for (int i = 0; i < padding; i++) {
      n.push_back('0');
    }
    n += n_temp;
    int borrow = 0;
    string diffNode = "";
    while (temp != nullptr) {
      string value = temp->data;
      diffNode = "";
      while ((!value.empty()) && (!n.empty())) {
        int digitOne = int(value.back() - '0') - borrow;
        int digitTwo = int(n.back() - '0');
        int digitDiff;

        if (digitOne >= digitTwo) {
          digitDiff = digitOne - digitTwo;
          borrow = 0;
        } else {
          digitDiff = digitOne + 10 - digitTwo;
          borrow = 1;
        }
        diffNode.push_back(digitDiff + '0');
        value.pop_back();
        n.pop_back();
      }
      temp = temp->prev;
      reverse(diffNode.begin(), diffNode.end());
      result.InsertAtFront(diffNode);
    }
    return result;
  }

  friend bool operator==(const Number &n1, const Number &n2) {
    if (n1.GetNumber() == n2.GetNumber()) {
      return true;
    }
    return false;
  }

  friend bool operator==(const Number &n1, int &n2) {
    if (n1.GetNumber() == to_string(n2)) {
      return true;
    }
    return false;
  }

  friend bool operator<(const Number &n1, const Number &n2) {
    if (n1.length < n2.length) {
      return true;
    }
    if (n1.length > n2.length) {
      return false;
    }
    node *temp = n1.first;
    node *tempTwo = n2.first;
    while (temp != nullptr) {
      string data = temp->data;
      string dataTwo = tempTwo->data;
      while (!data.empty()) {
        if (int(data.front()) < int(dataTwo.front())) {
          return true;
        }
        data = data.substr(1, data.length());
        dataTwo = dataTwo.substr(1, dataTwo.length());
      }
      temp = temp->next;
      tempTwo = tempTwo->next;
    }
    return false;
  }

  friend bool operator<(const Number &n1, int &n2) {
    string n = to_string(n2);
    if (n1.length < n.length()) {
      return true;
    }
    if (n1.length > n.length()) {
      return false;
    }
    node *temp = n1.first;
    while (temp != nullptr) {
      string data = temp->data;
      while (!data.empty()) {
        if (int(data.front()) < int(n.front())) {
          return true;
        }
        data = data.substr(1, data.length());
        n = n.substr(1, n.length());
      }
      temp = temp->next;
    }
    return false;
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
  if (num.length() <= 9) {
    (*n).InsertAtEnd(num.substr(0, num.length()));
    return;
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

  n3 = n1 * n2;
  n3.PrintList();
  return 0;
}