#include <algorithm> //Reverse string algorithm
#include <iostream>
#include <string>
#include <random> //Random number generation for Miller-Rabin

using namespace std;

random_device rd;                              // Generate
std::mt19937 eng(rd());                        // Random Numbers
std::uniform_int_distribution<> distr(0, 128); // in range 0 - 128

class Number; // Declaration of class for functions declared below

void storeNumber(Number *n, string num); // Declaration of function

class node // Stores 0-9 digits of the 1024 bit number
{
public:
  string data;          // Contains 0-9 digits of the 1024 bit number
  node *prev = nullptr; // Address of next bundle of digits
  node *next = nullptr; // Address of previous bundle of digits
};

class Number // Stores the 1024 bit number
{
public:
  node *first = nullptr; // Address of first node
  node *last = nullptr;  // Address of last node
  node *Loc_ = nullptr;  // Address of node being searched
  node *PLoc_ = nullptr; // Address of node previous to node being searched
  int length = 0;        // Number of digits stored in class

  // List methods

  bool isEmpty() const { return first == nullptr; }

  void PrintList()
  {
    if (!isEmpty())
    {
      node *current;
      current = first;           // Start from first node
      while (current != nullptr) // Continue until end of list reached
      {
        cout << current->data;   // Print the data
        current = current->next; // Move to next node
      }
      cout << endl;
    }
    else
    {
      cout << "Number is empty" << endl;
    }
  }

  string GetNumber() const // Same as PrintList except it returns it as a string
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

  void InsertAtFront(string value)
  {
    node *newNode = new node(); // Make a new node
    newNode->data = value;      // Assign it value

    if (first == nullptr) // If first node is null
    {
      first = newNode;          // new node will be the first node
      last = newNode;           // new node will also be the last node
      length += value.length(); // Increase length of class
      return;
    }

    first->prev = newNode;    // First node's previous now points to new node
    newNode->next = first;    // new node's next points to the first node
    first = newNode;          // new node is now the first node
    length += value.length(); // Increase length of class
  }

  void InsertAtEnd(string value)
  {
    node *newNode = new node(); // Make a new node
    newNode->data = value;      // Assign it value

    if (first == nullptr) // If first node is null
    {
      first = newNode;          // new node will be the first node
      last = newNode;           // new node will also be the last node
      length += value.length(); // Increase length of class
      return;
    }

    last->next = newNode;     // Last node's next now points to new node
    newNode->prev = last;     // new node's prev points to the last node
    last = newNode;           // new node is now the last node
    length += value.length(); // Increase length of class
  }

  void DestroyList() // Free up memory by iterating over list and deleting nodes and reset pointers
  {
    node *temp = first;
    node *tempTwo;
    while (temp != last)
    {
      tempTwo = temp;
      temp = temp->next;
      delete tempTwo;
    }
    delete temp;
    first = nullptr;
    last = nullptr;
    length = 0;
  }

  static Number clean(Number &n1) // Remove leading zeros from numbers, happens after operations on number
  {
    string numOne = n1.GetNumber(); // Get string representation of number
    bool zeroCheckOne = true;       // Is number zero

    for (int i = 0; i < numOne.length(); i++) // Checks if number is zero
    {
      if (numOne.at(i) != '0')
      {
        zeroCheckOne = false;
        break;
      }
    }

    if (zeroCheckOne) // Return single 0
    {
      n1.DestroyList();
      n1.InsertAtEnd("0");
      return n1;
    }

    n1.DestroyList(); // Free up old list

    size_t TrimIndexNumOne = numOne.find_first_not_of('0'); // Method to remove leading
    numOne = numOne.substr(TrimIndexNumOne);                // zeros
    storeNumber(&n1, numOne);                               // Store clean digit back in list
    return n1;
  }

  // Overloaded operator methods

  friend Number operator+(const Number &n1, const Number &n2) // Add two Numbers
  {
    Number result;
    int carry = 0;
    string numOne = n1.GetNumber(); // Get number in string format
    string numTwo = n2.GetNumber(); // Get number in string format

    string sumNode = "";

    while (!numOne.empty() && !numTwo.empty()) // Repeat until one of the number becomes empty
    {
      int digitOne = int(numOne.back() - '0'); // Extract last digit from first number
      int digitTwo = int(numTwo.back() - '0'); // Extract last digit from second number
      int digitSum = digitOne + digitTwo + carry;
      if (digitSum > 9)
      {
        carry = digitSum / 10;    // Extract the carry
        digitSum = digitSum % 10; // Extract the sum
      }
      else
      {
        carry = 0; // Otherwise no carry
      }
      sumNode.push_back(digitSum + '0'); // Push this digit into the sumNode
      numOne.pop_back();                 // Remove last digit from number one
      numTwo.pop_back();                 // Remove last digit from number two
    }

    if (!numOne.empty()) // If number one had more digits then number two
    {
      while (!numOne.empty()) // Loop until number one has no more digits
      {
        int res = carry + numOne.back() - '0'; // If there was a carry it will get added
        if (res > 9)
        {
          carry = res / 10; // Extract carry
          res = res % 10;   // Extract sum
        }
        else
        {
          carry = 0; // No more carry
        }
        sumNode.push_back(res + '0'); // Push this digit into the sumNode
        numOne.pop_back();            // Remove last digit from number one
      }
    }

    if (!numTwo.empty()) // If number two had more digits then number one
    {
      while (!numTwo.empty()) // Loop until number one has no more digits
      {
        int res = carry + numTwo.back() - '0'; // If there was a carry it will get added
        if (res > 9)
        {
          carry = res / 10; // Extract carry
          res = res % 10;   // Extract sum
        }
        else
        {
          carry = 0; // No more carry
        }
        sumNode.push_back(res + '0'); // Push this digit into the sumNode
        numTwo.pop_back();            // Remove last digit from number one
      }
    }

    if (carry > 0) // Even after both numbers digits are catered and carry remains
    {
      sumNode.push_back(carry + '0'); // Push the carry into sum node
    }

    reverse(sumNode.begin(), sumNode.end()); // Note that we added from right to left and sumNode answer is reverse of actual answer
    storeNumber(&result, sumNode);           // Store the number in result
    return result;
  }

  friend Number operator+(const Number &n1, const int n2) // Add Number with int
  {
    Number result;
    node *tempOne = n1.last; // Start from last node
    int carry = n2;          // Assume that the second number is carry

    while (tempOne != nullptr)
    {
      string partOne = tempOne->data; // Extract data from current node
      string sumNode;

      while (!partOne.empty()) // Loop until string becomes empty
      {
        int digitOne = partOne.back() - '0'; // Get last digit from data and convert it into int
        int digitSum = digitOne + carry;     // Add last digit with carry

        carry = digitSum / 10;                    // Extract carry from resulting sum
        sumNode.push_back((digitSum % 10) + '0'); // Extract sum

        partOne.pop_back(); // Remove last digit from data
      }

      reverse(sumNode.begin(), sumNode.end()); // Note that we added from right to left and sumNode answer is reverse of actual answer

      tempOne = tempOne->prev; // Move to previous node

      result.InsertAtFront(sumNode); // Push sumNode at front of result
    }

    if (carry > 0) // At end if there was any carry left
    {
      string sumNode = "";
      sumNode.push_back(carry + '0'); // Push carry into sumNode

      node *newNode = new node(); // Create new node
      newNode->data = sumNode;

      result.InsertAtFront(sumNode); // Push new node into result at start
    }

    return result; // Return result
  }

  friend Number operator*(const Number &n1, const Number &n2) // Multiply two Numbers
  {
    Number result;
    Number *temp = new Number[n2.length]; // Stores results of digit multiplication for each digit in second number
    node *tempTwo = n2.last;              // Start from last node of second number
    int z = 0;                            // Counter to store digit number of last digit

    while (tempTwo != nullptr) // Until all nodes of second number have been traversed
    {
      node *tempOne = n1.last; // Start from last node of first number
      int carry = 0;           // Initialize carry to zero
      string productNode;

      string partTwo = tempTwo->data;               // Get value of node into string
      productNode = "";                             // Initialize productNode to empty
      for (int i = partTwo.size() - 1; i >= 0; --i) // Iterate over value of node from last to first digit
      {
        productNode = "";  // Initialize productNode to empty
        carry = 0;         // Initialize carry to empty
        tempOne = n1.last; // Start from last node of first number
        while (tempOne != nullptr)
        {
          string partOne = tempOne->data;               // Get value of node into string
          for (int j = partOne.size() - 1; j >= 0; --j) // Iterate over value of node from last to first digit
          {
            int digitOne = partOne[j] - '0';                  // Extract jth digit from part one
            int digitTwo = partTwo[i] - '0';                  // Extract ith digit from part two
            int digitProduct = digitOne * digitTwo + carry;   // Multiply the digits and add carry
            carry = digitProduct / 10;                        // Extract carry from multiplicatoin
            productNode.push_back((digitProduct % 10) + '0'); // Extract product and push it into productNode
          }
          tempOne = tempOne->prev; // Move onto next node of first number
        }

        if (carry > 0) // At the end of multiplication if there was a carry push it into productNode
        {
          productNode.push_back(carry + '0');
        }

        reverse(productNode.begin(), productNode.end()); // Multiplication is from right to left so we need to reverse result order

        if (z > 0) // This follows the concept of X in multiplication each layer you add X in right side of result which basically acts as zero
        {
          int temp = z;                 // e.g
          while (temp > 0)              //   25
          {                             // x 12
            productNode.push_back('0'); //   50
            temp--;                     //  25x  =  Add both layers = 300 = 25x12
          }
        }
        temp[z].InsertAtEnd(productNode); // Store this result as answer of one layer
        z++;                              // Move onto next layer number of X(0) will increase per layer
      }

      tempTwo = tempTwo->prev; // Move onto next node of number two
    }

    result = temp[0];                   // Initialize result as first layer
    for (int i = 1; i < n2.length; i++) // Add each layer to result
    {
      result = result + temp[i];
    }
    delete[] temp; // Free up memory
    return result;
  }

  friend Number operator*(const Number &n1, int n2) // Multiply a Number with int
  {
    Number result = n1;
    for (int i = 0; i < n2 - 1; i++) // Add the result n2 times to multiply
    {
      result = result + n1;
    }
    return result;
  }

  friend Number operator-(const Number &n1, const Number &n2) // Subtract two Numbers
  {
    Number result;
    int borrow = 0;
    string numOne = n1.GetNumber(); // Get string representation of first number
    string numTwo = n2.GetNumber(); // Get string representation of second number

    string diffNode = "";

    while (!numOne.empty() && !numTwo.empty()) // Until one of the string does not get empty
    {
      int digitDiff;
      int digitOne = int(numOne.back() - '0') - borrow; // Digit one will be last digit of first number minus borrow (char to int)
      int digitTwo = int(numTwo.back() - '0');          // Digit two will be last digit of second number (char to int)

      if (digitOne >= digitTwo)
      {
        borrow = 0;
        digitDiff = digitOne - digitTwo;
      }
      else
      {
        borrow = 1;                           // There is a borrow if digit is smaller than second digit
        digitDiff = digitOne + 10 - digitTwo; // Extract digitDiff after accounting for borrow
      }
      diffNode.push_back(digitDiff + '0'); // Push the difference in diffNode
      numOne.pop_back();                   // Remove last digit from first number
      numTwo.pop_back();                   // Remove last digit from second number
    }

    if (!numOne.empty()) // If number two got empty but number one still has digits
    {
      while (!numOne.empty())
      {
        int res;
        if (int(numOne.back() - '0') >= borrow) // Account for any borrow left
        {
          res = int(numOne.back() - '0') - borrow;
          borrow = 0;
        }
        else
        {
          res = int(numOne.back() - '0') + 10 - borrow;
          borrow = 1;
        }
        diffNode.push_back(res + '0'); // Push the digit into diffNode
        numOne.pop_back();             // Remove last digit from first number
      }
    }

    if (!numTwo.empty()) // If number one got empty but number two still has digits
    {
      while (!numTwo.empty())
      {
        int res;
        if (int(numTwo.back() - '0') >= borrow) // Account for any borrow left
        {
          res = int(numTwo.back() - '0') - borrow;
          borrow = 0;
        }
        else
        {
          res = int(numTwo.back() - '0') + 10 - borrow;
          borrow = 1;
        }
        diffNode.push_back(res + '0'); // Push the digit into diffNode
        numTwo.pop_back();             // Remove last digit from second number
      }
    }

    reverse(diffNode.begin(), diffNode.end()); // Subtraction was from right to left so answer is in reversed form
    storeNumber(&result, diffNode);            // Store diffNode in result

    return result;
  }

  friend Number operator-(const Number &n1, int n2) // Subtract
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
    bool firstTime = true;
    while (!numOne.empty())
    {
      if (remainder == "" && firstTime)
      {
        if (numOne.length() >= lenTwo)
        {
          data = numOne.substr(0, lenTwo);
          if (numOne.length() == lenTwo)
          {
            numOne = "";
          }
          else
          {
            numOne = numOne.substr(lenTwo, numTwo.length());
          }
        }
        firstTime = false;
      }
      else
      {
        firstTime = false;
        data = remainder;
      }
      Number nOne;
      storeNumber(&nOne, data);
      if (nOne < nTwo)
      {
        if (!numOne.empty())
        {
          data = data + numOne.substr(0, 1);
          nOne.DestroyList();
          storeNumber(&nOne, data);
          if (numOne.length() == 1)
          {
            numOne = "";
          }
          else
          {
            numOne = numOne.substr(1, numOne.length());
          }
        }
        else
        {
          remainder = data;
          goto nomore;
        }

        if (nOne < nTwo)
        {
          while (nOne < nTwo && !numOne.empty())
          {
            quotientNode.push_back('0');
            if (numOne.length() >= 1)
            {
              data = data + numOne.substr(0, 1);
              nOne.DestroyList();
              storeNumber(&nOne, data);
              numOne = numOne.substr(1, numOne.length());
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
    if (n2 == z)
    {
      throw runtime_error("Math error: Attempted to divide by Zero\n");
      Number x;
      return x;
    }

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

    Number nTwo;
    Number zero;
    zero.InsertAtEnd("0");
    storeNumber(&nTwo, numTwo);
    string quotientNode = "";
    string data;
    string remainder = "";
    bool firstTime = true;
    while (!numOne.empty())
    {
      if (remainder == "" && firstTime)
      {
        if (numOne.length() >= lenTwo)
        {
          data = numOne.substr(0, lenTwo);
          if (numOne.length() == lenTwo)
          {
            numOne = "";
          }
          else
          {
            numOne = numOne.substr(lenTwo, numOne.length());
          }
        }
        firstTime = false;
      }
      else
      {
        firstTime = false;
        data = remainder;
      }
      Number nOne;
      storeNumber(&nOne, data);
      if (nOne < nTwo)
      {
        if (!numOne.empty())
        {
          data = data + numOne.substr(0, 1);
          nOne.DestroyList();
          storeNumber(&nOne, data);
          if (numOne.length() == 1)
          {
            numOne = "";
          }
          else
          {
            numOne = numOne.substr(1, numOne.length());
          }
        }
        else
        {
          remainder = data;
          goto nomore2;
        }

        if (nOne < nTwo)
        {
          while (nOne < nTwo && !numOne.empty())
          {
            quotientNode.push_back('0');
            if (numOne.length() >= 1)
            {
              data = data + numOne.substr(0, 1);
              nOne.DestroyList();
              storeNumber(&nOne, data);
              numOne = numOne.substr(1, numOne.length());
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
};

Number modpow(Number numOne, Number numTwo, Number mod) // Compute a^b % mod
{
  Number result;           // Initialize
  result.InsertAtEnd("1"); // result as 1

  // Common digits
  int zero = 0;
  int one = 1;
  int two = 2;

  numOne = numOne % mod; // Initialize numOne

  while (!(numTwo < zero || numTwo == zero))
  {
    if ((int(numTwo.GetNumber().back() - '0') % 2) == 1) // If number is odd
    {
      Number tempTwo = result * numOne; // Multiply result by numOne
      result = (tempTwo) % mod;         // Take mod of tempTwo with mod
    }

    numTwo = numTwo / 2;
    Number temp = numOne * numOne; // temp = numOne ^ 2
    numOne = (temp % mod);         // Take modulus of temp with mod
  }
  return result;
}

bool millerRabin(Number d, Number n)
{
  int one = 1;          // Initialize
  Number two;           // Common
  two.InsertAtEnd("2"); // Digits

  Number rand;                             // Store random number
  rand.InsertAtEnd(to_string(distr(eng))); // Generate random number and store it in rand

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
  string *num = new string;
  *num = n.GetNumber();
  int result = 0;
  for (int i = 0; i < (*num).length(); i++)
  {
    result = result + int((*num).at(i) - '0');
  }
  delete num;
  if (result % 3 == 0)
  {
    return false;
  }

  int test = int(n.GetNumber().back() - '0');

  if (test % 2 == 0 || test == 0 || test == 5)
  {
    return false;
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

bool correctString(string temp) // Checks if user input was a number only
{
  if (temp.length() == 0)
  {
    return false;
  }
  for (int i = 0; i < temp.length(); i++) // Iterates over the string and if any character is not a digit it returns false
  {
    if (!((temp.at(i) >= '0') && (temp.at(i) <= '9')))
    {
      return false;
    }
  }
  return true;
}

string getNumber() // Takes input from user
{
  string temp = "";
  cout << "Enter your number: ";
  bool firstTime = true;       // To not display incorrect number message
  while (!correctString(temp)) // Loop until user enters a number only
  {
    if (!firstTime)
    {
      cout << endl
           << "You have not entered a correct number. Try Again!" << endl;
      cout << "Enter your number: ";
    }
    firstTime = false;  // After first iteration it becomes false
    getline(cin, temp); // Get input from user
  }
  return temp;
}

void storeNumber(Number *n, string num) // Store user input in a Number class object
{
  if (num.length() <= 9) // If user number was less than or equal to 9 digits
  {
    (*n).InsertAtEnd(num.substr(0, num.length())); // Insert it as a (the only) node
    return;
  }
  else
  {
    for (int i = 0; i < num.length() - (num.length() % 9); i += 9) // Split number into chunks of 9 digits
    {
      (*n).InsertAtEnd(num.substr(i, 9)); // Insert each chunk as a node
    }

    (*n).InsertAtEnd(num.substr(num.length() - (num.length() % 9),   // Insert remaining digits at end (less than 9)
                                num.length() - (num.length() % 9))); //  as a separate node
  }
}

int main()
{
  string num = getNumber(); // Get number from user and store it in string

  Number number;

  storeNumber(&number, num);

  // Check if number is prime and store result in message
  string message = isPrime(number, 32) ? "The number is a prime number." : "The number is not a prime number.";

  cout << endl
       << message; // Display message to user
  return 0;
}