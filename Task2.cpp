#include <algorithm>
#include <iostream>
#include <string>
#include <random>
#include <cmath>

using namespace std;

random_device rd;
std::mt19937 eng(rd());
std::uniform_int_distribution<> distr(0, 100000);

class Number;

void storeNumber(Number *n, string num);

class node
{
public:
  string data;
  node *prev = nullptr;
  node *next = nullptr;
};

class Number
{
public:
  node *first = nullptr;
  node *last = nullptr;
  int numItems = 0;
  node *Loc_ = nullptr;
  node *PLoc_ = nullptr;
  int length = 0;

  bool isEmpty() const { return first == nullptr; }

  void PrintList()
  {
    if (!isEmpty())
    {
      node *temp;
      temp = first;
      while (temp != nullptr)
      {
        cout << temp->data;
        temp = temp->next;
      }
      cout << endl;
    }
    else
    {
      cout << "Number is empty" << endl;
    }
  }

  string GetNumber() const
  {
    string num = "";
    if (!isEmpty())
    {
      node *temp;
      temp = first;
      while (temp != nullptr)
      {
        num += temp->data;
        temp = temp->next;
      }
      return num;
    }
    return "";
  }

  void Search(string key)
  {
    if (isEmpty())
    {
      return;
    }
    Loc_ = first;
    PLoc_ = nullptr;
    while (Loc_ == nullptr && Loc_->data < key)
    {
      PLoc_ = Loc_;
      Loc_ = Loc_->next;
    }

    if (Loc_ == nullptr && Loc_->data != key)
    {
      Loc_ = nullptr;
    }
  }

  void InsertAtFront(string value)
  {
    node *newNode = new node();
    newNode->data = value;

    if (first == nullptr)
    {
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

  void InsertAtEnd(string value)
  {
    node *newNode = new node();
    newNode->data = value;

    if (first == nullptr)
    {
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

  void Delete(string value)
  {
    Search(value);
    if (Loc_ != nullptr)
    {
      if (PLoc_ == nullptr)
      {
        if (Loc_ == last)
        {
          first = nullptr;
          last = nullptr;
        }
        else
        {
          first = first->next;
          first->prev = nullptr;
        }
      }
      else
      {
        if (Loc_ == last)
        {
          last = PLoc_;
          PLoc_->next = nullptr;
        }
        else
        {
          Loc_->next->prev = PLoc_;
          PLoc_->next = Loc_->next;
        }
      }
      delete Loc_;
    }
    else
    {
      cout << "Value not found." << endl;
    }
  }

  void DestroyList()
  {
    node *temp = first;
    node *t2;
    while (temp != last)
    {
      t2 = temp;
      temp = temp->next;
      delete t2;
    }
    delete temp;
    first = nullptr;
    last = nullptr;
    length = 0;
    numItems = 0;
  }

  // Check for bugs both + Operators
  friend Number operator+(const Number &n1, const Number &n2)
  {
    Number result;
    int carry = 0;
    string numOne = n1.GetNumber();
    string numTwo = n2.GetNumber();
    string sumNode = "";
    while (!numOne.empty() && !numTwo.empty())
    {
      int digitOne = int(numOne.back() - '0');
      int digitTwo = int(numTwo.back() - '0');
      int digitSum = digitOne + digitTwo + carry;
      if (digitSum > 9)
      {
        carry = digitSum / 10;
        digitSum = digitSum % 10;
      }
      else
      {
        carry = 0;
      }
      sumNode.push_back(digitSum + '0');
      numOne.pop_back();
      numTwo.pop_back();
    }
    if (!numOne.empty())
    {
      while (!numOne.empty())
      {
        int res = carry + numOne.back() - '0';
        if (res > 9)
        {
          carry = res / 10;
          res = res % 10;
        }
        else
        {
          carry = 0;
        }
        sumNode.push_back(res + '0');
        numOne.pop_back();
      }
    }
    if (!numTwo.empty())
    {
      while (!numTwo.empty())
      {
        int res = carry + numTwo.back() - '0';
        if (res > 9)
        {
          carry = res / 10;
          res = res % 10;
        }
        else
        {
          carry = 0;
        }
        sumNode.push_back(res + '0');
        numTwo.pop_back();
      }
    }
    if (carry > 0)
    {
      sumNode.push_back(carry + '0');
    }
    reverse(sumNode.begin(), sumNode.end());
    storeNumber(&result, sumNode);
    return result;
  }

  friend Number operator+(const Number &n1, const int n2)
  {
    Number result;
    node *tempOne = n1.last;
    int carry = n2;

    while (tempOne != nullptr)
    {
      string partOne = (tempOne != nullptr) ? tempOne->data : "0";
      string sumNode;

      while (!partOne.empty())
      {
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

    if (carry > 0)
    {
      string sumNode = "";
      sumNode.push_back(carry + '0');

      node *newNode = new node();
      newNode->data = sumNode;

      result.InsertAtFront(sumNode);
    }

    return result;
  }

  friend Number operator*(const Number &n1, const Number &n2)
  {
    Number result;
    Number *temp = new Number[n2.length];
    node *tempTwo = n2.last;
    int z = 0;

    while (tempTwo != nullptr)
    {
      node *tempOne = n1.last;
      int carry = 0;
      string productNode = "";

      string partTwo = tempTwo->data;
      productNode = "";
      for (int i = partTwo.size() - 1; i >= 0; --i)
      {
        productNode = "";
        carry = 0;
        tempOne = n1.last;
        while (tempOne != nullptr)
        {
          string partOne = tempOne->data;
          for (int j = partOne.size() - 1; j >= 0; --j)
          {
            int digitOne = partOne[j] - '0';
            int digitTwo = partTwo[i] - '0';
            int digitProduct = digitOne * digitTwo + carry;
            carry = digitProduct / 10;
            productNode.push_back((digitProduct % 10) + '0');
          }
          tempOne = tempOne->prev;
        }
        if (carry > 0)
        {
          productNode.push_back(carry + '0');
        }

        reverse(productNode.begin(), productNode.end());
        if (z > 0)
        {
          int temp = z;
          while (temp > 0)
          {
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
    for (int i = 1; i < n2.length; i++)
    {
      result = result + temp[i];
    }
    delete[] temp;
    return result;
  }

  friend Number operator*(const Number &n1, int n2)
  {
    Number result = n1;
    for (int i = 0; i < n2 - 1; i++)
    {
      result = result + n1;
    }
    return result;
  }

  friend Number operator-(const Number &n1, const Number &n2)
  {
    Number result;
    int borrow = 0;
    string numOne = n1.GetNumber();
    string numTwo = n2.GetNumber();
    string diffNode = "";
    while (!numOne.empty() && !numTwo.empty())
    {
      int digitDiff;
      int digitOne = int(numOne.back() - '0') - borrow;
      int digitTwo = int(numTwo.back() - '0');

      if (digitOne >= digitTwo)
      {
        borrow = 0;
        digitDiff = digitOne - digitTwo;
      }
      else
      {
        borrow = 1;
        digitDiff = digitOne + 10 - digitTwo;
      }
      diffNode.push_back(digitDiff + '0');
      numOne.pop_back();
      numTwo.pop_back();
    }
    if (!numOne.empty())
    {
      while (!numOne.empty())
      {
        int res;
        if (int(numOne.back() - '0') >= borrow)
        {
          res = int(numOne.back() - '0') - borrow;
          borrow = 0;
        }
        else
        {
          res = int(numOne.back() - '0') + 10 - borrow;
          borrow = 1;
        }
        diffNode.push_back(res + '0');
        numOne.pop_back();
      }
    }
    if (!numTwo.empty())
    {
      while (!numTwo.empty())
      {
        int res;
        if (int(numTwo.back() - '0') >= borrow)
        {
          res = int(numTwo.back() - '0') - borrow;
          borrow = 0;
        }
        else
        {
          res = int(numTwo.back() - '0') + 10 - borrow;
          borrow = 1;
        }
        diffNode.push_back(res + '0');
        numTwo.pop_back();
      }
    }
    reverse(diffNode.begin(), diffNode.end());
    storeNumber(&result, diffNode);
    return result;
  }

  friend Number operator-(const Number &n1, int n2)
  {
    Number result;
    node *temp = n1.last;
    string n_temp = to_string(n2);
    int len = n_temp.length();
    int padding = n1.length - len;
    string n = "";
    for (int i = 0; i < padding; i++)
    {
      n.push_back('0');
    }
    n += n_temp;
    int borrow = 0;
    string diffNode = "";
    while (temp != nullptr)
    {
      string value = temp->data;
      diffNode = "";
      while ((!value.empty()) && (!n.empty()))
      {
        int digitOne = int(value.back() - '0') - borrow;
        int digitTwo = int(n.back() - '0');
        int digitDiff;

        if (digitOne >= digitTwo)
        {
          digitDiff = digitOne - digitTwo;
          borrow = 0;
        }
        else
        {
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

    string data = result.GetNumber();
    size_t TrimIndexData = data.find_first_not_of('0');
    data = data.substr(TrimIndexData);

    result.DestroyList();
    storeNumber(&result, data);
    return result;
  }

  friend bool operator==(const Number &n1, const Number &n2)
  {
    string numOne = n1.GetNumber();
    bool zeroCheckOne = true, zeroCheckTwo = true;
    string numTwo = n2.GetNumber();
    for (int i = 0; i < numOne.length(); i++)
    {
      if (numOne.at(i) != '0')
      {
        zeroCheckOne = false;
        break;
      }
    }
    for (int i = 0; i < numTwo.length(); i++)
    {
      if (numTwo.at(i) != '0')
      {
        zeroCheckTwo = false;
        break;
      }
    }
    if (zeroCheckOne && zeroCheckTwo)
    {
      return true;
    }
    if (zeroCheckTwo || zeroCheckOne)
    {
      return false;
    }

    size_t TrimIndexNumOne = numOne.find_first_not_of('0');
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0');
    numOne = numOne.substr(TrimIndexNumOne);

    numTwo = numTwo.substr(TrimIndexNumTwo);

    if (numOne == numTwo)
    {
      return true;
    }
    return false;
  }

  friend bool
  operator==(const Number &n1, int &n2)
  {
    string num = n1.GetNumber();
    bool zeroCheck = true;
    for (int i = 0; i < num.length(); i++)
    {
      if (num.at(i) != '0')
      {
        zeroCheck = false;
        break;
      }
    }

    if (zeroCheck && (n2 == 0))
    {
      return true;
    }

    if (zeroCheck || (n2 == 0))
    {
      return false;
    }

    size_t TrimIndexNum = num.find_first_not_of('0');
    num = num.substr(TrimIndexNum);
    if (num == to_string(n2))
    {
      return true;
    }
    return false;
  }

  friend bool operator<(const Number &n1, const Number &n2)
  {
    if (n1 == n2)
    {
      return false;
    }

    string numOne = n1.GetNumber();
    string numTwo = n2.GetNumber();
    bool zeroCheckOne = true, zeroCheckTwo = true;

    for (int i = 0; i < numOne.length(); i++)
    {
      if (numOne.at(i) != '0')
      {
        zeroCheckOne = false;
        break;
      }
    }
    for (int i = 0; i < numTwo.length(); i++)
    {
      if (numTwo.at(i) != '0')
      {
        zeroCheckTwo = false;
        break;
      }
    }
    if (zeroCheckTwo && zeroCheckTwo)
    {
      return false;
    }
    if (zeroCheckOne && (!zeroCheckTwo))
    {
      return true;
    }
    if (!zeroCheckOne && zeroCheckTwo)
    {
      return false;
    }

    size_t TrimIndexNumOne = numOne.find_first_not_of('0');
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0');
    numOne = numOne.substr(TrimIndexNumOne);
    numTwo = numTwo.substr(TrimIndexNumTwo);

    if (numOne.length() > numTwo.length())
    {
      return false;
    }
    if (numOne.length() < numTwo.length())
    {
      return true;
    }
    for (int i = 0; i < numOne.length(); i++)
    {
      if (int(numOne.at(i) - '0') == int(numTwo.at(i) - '0'))
      {
        continue;
      }
      if (int(numOne.at(i) - '0') < int(numTwo.at(i) - '0'))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    return false;
  }

  friend bool
  operator<(const Number &n1, int &n2)
  {
    if (n1 == n2)
    {
      return false;
    }

    string numOne = n1.GetNumber();
    string numTwo = to_string(n2);
    bool zeroCheckOne = true, zeroCheckTwo = true;

    for (int i = 0; i < numOne.length(); i++)
    {
      if (numOne.at(i) != '0')
      {
        zeroCheckOne = false;
        break;
      }
    }
    for (int i = 0; i < numTwo.length(); i++)
    {
      if (numTwo.at(i) != '0')
      {
        zeroCheckTwo = false;
        break;
      }
    }
    if (zeroCheckTwo && zeroCheckTwo)
    {
      return false;
    }
    if (zeroCheckOne && (!zeroCheckTwo))
    {
      return true;
    }
    if (!zeroCheckOne && zeroCheckTwo)
    {
      return false;
    }

    size_t TrimIndexNumOne = numOne.find_first_not_of('0');
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0');
    numOne = numOne.substr(TrimIndexNumOne);
    numTwo = numTwo.substr(TrimIndexNumTwo);

    if (numOne.length() > numTwo.length())
    {
      return false;
    }
    if (numOne.length() < numTwo.length())
    {
      return true;
    }
    for (int i = 0; i < numOne.length(); i++)
    {
      if (int(numOne.at(i) - '0') == int(numTwo.at(i) - '0'))
      {
        continue;
      }
      if (int(numOne.at(i) - '0') < int(numTwo.at(i) - '0'))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    return false;
  }

  friend Number operator%(const Number &n1, const Number &n2)
  {
    int z = 0;
    if (n1 == z)
    {
      Number z2;
      z2.InsertAtEnd("0");
      return z2;
    }

    if (n1 < n2)
    {
      return n1;
    }

    string numOne = n1.GetNumber();
    string numTwo = n2.GetNumber();
    size_t TrimIndexNumOne = numOne.find_first_not_of('0');
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0');
    numOne = numOne.substr(TrimIndexNumOne);
    numTwo = numTwo.substr(TrimIndexNumTwo);
    int lenOne = numOne.length();
    int lenTwo = numTwo.length();
    if (numOne == numTwo)
    {
      Number zero;
      zero.InsertAtEnd("0");
      return zero;
    }
    if (lenOne < lenTwo)
    {
      return n1;
    }

    Number nTwo;
    Number zero;
    zero.InsertAtEnd("0");
    storeNumber(&nTwo, numTwo);
    string quotientNode = "";
    string data;
    string remainder = "";
    bool less = false;
    bool firstTime = true;
    while (!numOne.empty())
    {
      if (remainder == "" && firstTime)
      {
        data = numOne.substr(0, lenTwo);
      }
      else
      {
        firstTime = false;
        data = remainder;
      }
      Number nOne;
      storeNumber(&nOne, data);
      int i = 0;
      if (nOne < nTwo)
      {
        i = 1;
        data = data + numOne.substr(0, 1);
        nOne.DestroyList();
        storeNumber(&nOne, data);
        if (nOne < nTwo)
        {
          i = 1;
          while (nOne < nTwo)
          {
            quotientNode.push_back('0');
            if (numOne.length() >= i)
            {
              data = data + numOne.substr(0 + i, 1);
              i++;
              nOne.DestroyList();
              storeNumber(&nOne, data);
            }
            else
            {
              numOne = "";
              remainder = data;
              goto nomore;
            }
          }
        }
      }

      int multiple = 0;
      while (!(nOne < nTwo))
      {
        nOne = nOne - nTwo;
        multiple++;
      }
      quotientNode = quotientNode + to_string(multiple);
      if (firstTime)
      {
        numOne = numOne.substr(lenTwo, numOne.length());
      }
      else
      {
        numOne = numOne.substr(i, numOne.length());
      }

      nOne = clean(nOne);
      remainder = (nOne == zero) ? "" : nOne.GetNumber();
    }
  nomore:
    Number result;
    storeNumber(&result, remainder);
    return result;
  }

  friend Number operator%(Number &n1, int &n2)
  {
    int res = 0;
    string num = n1.GetNumber();
    for (int i = 0; i < num.length(); i++)
    {
      res = (res * 10 + num[i] - '0') % n2;
    }

    Number result;
    result.InsertAtEnd(to_string(res));
    return result;
  }

  friend Number pow(Number &base, Number &exp)
  {
    int z = 0;
    int o = 1;
    if (exp == z)
    {
      Number one;
      one.InsertAtEnd("1");
      return one;
    }

    if (exp == o)
    {
      return base;
    }

    Number result;
    result.InsertAtEnd("1");

    while (!(exp == z || exp < z))
    {

      if ((exp.GetNumber().back() - '0') % 2 == 1)
      {
        result = result * base;
      }
      base = base * base;
      exp = exp / 2;
    }

    return result;
  }

  friend Number pow(const Number &n1, const int &n2)
  {
    Number result;
    result.InsertAtEnd("1");
    for (int i = 0; i < n2; i++)
    {
      result = result * n1;
    }
    result = clean(result);
    return result;
  }

  friend Number operator/(const Number &n1, const Number &n2)
  {
    int z = 0;
    if (n1 == z)
    {
      Number z2;
      z2.InsertAtEnd("0");
      return z2;
    }

    if (n1 < n2)
    {
      Number z2;
      z2.InsertAtEnd("0");
      return z2;
    }

    string numOne = n1.GetNumber();
    string numTwo = n2.GetNumber();
    size_t TrimIndexNumOne = numOne.find_first_not_of('0');
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0');
    numOne = numOne.substr(TrimIndexNumOne);
    numTwo = numTwo.substr(TrimIndexNumTwo);
    int lenOne = numOne.length();
    int lenTwo = numTwo.length();
    if (numOne == numTwo)
    {
      Number one;
      one.InsertAtEnd("1");
      return one;
    }

    if (lenOne < lenTwo)
    {
      Number zero;
      zero.InsertAtEnd("0");
      return zero;
    }

    Number nTwo;
    Number zero;
    zero.InsertAtEnd("0");
    storeNumber(&nTwo, numTwo);
    string quotientNode = "";
    string data;
    string remainder = "";
    bool less = false;
    bool firstTime = true;
    while (!numOne.empty())
    {
      if (remainder == "" && firstTime)
      {
        data = numOne.substr(0, lenTwo);
      }
      else
      {
        firstTime = false;
        data = remainder;
      }
      Number nOne;
      bool digitRequestFirstTime = false;
      storeNumber(&nOne, data);
      int i = 0;
      if (nOne < nTwo)
      {
        i = 1;
        if (!firstTime)
        {
          data = data + numOne.substr(0, 1);
          nOne.DestroyList();
          storeNumber(&nOne, data);
        }
        else
        {
          data = data + numOne.substr(lenTwo, 1);
          nOne.DestroyList();
          storeNumber(&nOne, data);
          digitRequestFirstTime = true;
        }

        if (nOne < nTwo)
        {
          i = 1;
          while (nOne < nTwo)
          {
            quotientNode.push_back('0');
            if (numOne.length() >= i)
            {
              data = data + numOne.substr(0 + i, 1);
              i++;
              nOne.DestroyList();
              storeNumber(&nOne, data);
            }
            else
            {
              numOne = "";
              remainder = data;
              goto nomore2;
            }
          }
        }
      }

      int multiple = 0;
      while (!(nOne < nTwo))
      {
        nOne = nOne - nTwo;
        multiple++;
      }
      quotientNode = quotientNode + to_string(multiple);
      if (firstTime)
      {
        if (!digitRequestFirstTime)
        {
          numOne = numOne.substr(lenTwo, numOne.length());
        }
        else
        {
          if (numOne.length() == lenTwo + i)
          {
            numOne = "";
          }
          else
          {
            numOne = numOne.substr(lenTwo + i, numOne.length());
          }
        }
      }
      else
      {
        numOne = numOne.substr(i, numOne.length());
      }

      nOne = clean(nOne);
      remainder = (nOne == zero) ? "" : nOne.GetNumber();
    }
  nomore2:

    Number result;
    storeNumber(&result, quotientNode);
    return result;
  }

  friend Number operator/(const Number &n1, const int &n2)
  {
    Number n2T;
    string n2I = to_string(n2);
    storeNumber(&n2T, n2I);

    Number result = n1 / n2T;

    return result;
  }

  static Number clean(Number &n1)
  {
    string numOne = n1.GetNumber();
    bool zeroCheckOne = true;

    for (int i = 0; i < numOne.length(); i++)
    {
      if (numOne.at(i) != '0')
      {
        zeroCheckOne = false;
        break;
      }
    }

    if (zeroCheckOne)
    {
      n1.DestroyList();
      n1.InsertAtEnd("0");
      return n1;
    }
    n1.DestroyList();
    size_t TrimIndexNumOne = numOne.find_first_not_of('0');
    numOne = numOne.substr(TrimIndexNumOne);
    storeNumber(&n1, numOne);
    return n1;
  }
};

Number modpow(Number a, Number b, Number mod)
{ // Compute a^b % mod
  Number result;
  result.InsertAtEnd("1");
  int zero = 0;
  int one = 1;
  int two = 2;
  a = a % mod;

  while (!(b < zero || b == zero))
  {
    if ((int(b.GetNumber().back() - '0') % 2) == 1)
    {
      Number temp2 = result * a;
      result = (temp2) % mod;
    }

    b = b / 2;
    Number temp = a * a;
    a = (temp % mod);
  }
  return result;
}

bool millerRabin(Number d, Number n)
{
  int one = 1;
  Number two;
  two.InsertAtEnd("2");
  Number rand;
  rand.InsertAtEnd(to_string(distr(eng)));
  Number temp2 = n - 4;
  Number temp = (rand % temp2);
  Number a = two + temp;
  Number x = modpow(a, d, n);

  if ((x == one) || (x == (n - 1)))
  {
    return true;
  }
  while (!(d == (n - 1)))
  {
    x = (x * x) % n;
    d = d * 2;
    if (x == one)
    {
      return false;
    }
    if (x == (n - 1))
    {
      return true;
    }
  }
  return false;
}

bool isPrime(Number n, int k)
{
  int four = 4;
  int one = 1;
  int three = 3;
  if (((n < one) || (n == one)) || (n == four))
  {
    return false;
  }
  if (n < three || n == three)
  {
    return true;
  }

  Number d = n - 1;
  while ((int(d.GetNumber().back() - '0') % 2) == 0)
  {
    d = (d / 2);
  }

  for (int i = 0; i < k; i++)
  {
    if (!millerRabin(d, n))
    {
      return false;
    }
  }

  return true;
}

bool correctString(string temp)
{
  if (temp.length() == 0)
  {
    return false;
  }
  for (int i = 0; i < temp.length(); i++)
  {
    if (!((temp.at(i) >= '0') && (temp.at(i) <= '9')))
    {
      return false;
    }
  }
  return true;
}

string getNumber()
{
  string temp = "";
  cout << "Enter your number: ";
  bool firstTime = true;
  while (!correctString(temp))
  {
    if (!firstTime)
    {
      cout << endl
           << "You have not entered a correct number. Try Again!" << endl;
      cout << "Enter your number: ";
    }
    firstTime = false;
    getline(cin, temp);
  }
  return temp;
}

void storeNumber(Number *n, string num)
{
  if (num.length() <= 9)
  {
    (*n).InsertAtEnd(num.substr(0, num.length()));
    return;
  }
  else
  {
    for (int i = 0; i < num.length() - (num.length() % 9); i += 9)
    {
      (*n).InsertAtEnd(num.substr(i, 9));
    }

    (*n).InsertAtEnd(num.substr(num.length() - (num.length() % 9),
                                num.length() - (num.length() % 9)));
  }
}

int main()
{
  string num = getNumber();
  // string num2 = getNumber();

  Number n1;
  Number n2;
  Number n3;

  storeNumber(&n1, num);
  // storeNumber(&n2, num2);

  n1.PrintList();

  cout << "Is prime: " << isPrime(n1, 32);
  // n2.PrintList();

  // n3 = n1 / n2;
  // n3.PrintList();

  return 0;
}