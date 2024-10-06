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

  friend Number operator-(const Number &n1, int n2) // Subtract int from Number
  {
    Number result;
    node *temp = n1.last; // Start form last node of first number

    string n_temp = to_string(n2); // Convert second number into a string
    int len = n_temp.length();     // Find length of second number
    int padding = n1.length - len; // Calculate padding of zeros
    string n = "";
    for (int i = 0; i < padding; i++)
    {
      n.push_back('0'); // Create a padded string
    }
    n += n_temp;    // Add original value to padded string
    int borrow = 0; // Initialize borrow to zero
    string diffNode = "";
    while (temp != nullptr) // As long as list end is not reached
    {
      string value = temp->data; // Extract data from node
      diffNode = "";
      while ((!value.empty()) && (!n.empty())) // As long as one of the strings is not empty
      {
        int digitOne = int(value.back() - '0') - borrow; // Cast last digit of first number to int and subtract borrow
        int digitTwo = int(n.back() - '0');              // Cast last digit of second number to int
        int digitDiff;

        if (digitOne >= digitTwo) // Check for need for borrow
        {
          digitDiff = digitOne - digitTwo;
          borrow = 0;
        }
        else
        {
          digitDiff = digitOne + 10 - digitTwo; // Recalculate difference if borrow was attempted
          borrow = 1;
        }
        diffNode.push_back(digitDiff + '0'); // Push the difference into diffNode
        value.pop_back();                    // Remove last digit of value
        n.pop_back();                        // Remove last digit of number two
      }

      temp = temp->prev;                         // Move on to previous node
      reverse(diffNode.begin(), diffNode.end()); // Subtraction is from right to left result is reversed string
      result.InsertAtFront(diffNode);            // Make new node of result
    }

    result = clean(result); // Remove leading zeros after subtraction

    return result;
  }

  friend bool operator==(const Number &n1, const Number &n2) // Check if two Numbers are equal
  {
    string numOne = n1.GetNumber(); // Get String representation of first number
    string numTwo = n2.GetNumber(); // Get String representation of second number

    bool zeroCheckOne = true, zeroCheckTwo = true; // Zero check variables for both numbers

    for (int i = 0; i < numOne.length(); i++) // Iterate and check if non zero digit exists
    {
      if (numOne.at(i) != '0')
      {
        zeroCheckOne = false;
        break;
      }
    }
    for (int i = 0; i < numTwo.length(); i++) // Iterate and check if non zero digit exists
    {
      if (numTwo.at(i) != '0')
      {
        zeroCheckTwo = false;
        break;
      }
    }

    if (zeroCheckOne && zeroCheckTwo) // If both are zero then equal
    {
      return true;
    }

    if (zeroCheckTwo || zeroCheckOne) // If one is zero then not equal
    {
      return false;
    }

    size_t TrimIndexNumOne = numOne.find_first_not_of('0'); // Remove
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0'); // Pading
    numOne = numOne.substr(TrimIndexNumOne);                // Zeros
    numTwo = numTwo.substr(TrimIndexNumTwo);                // From both Numbers

    if (numOne == numTwo)
    {
      return true;
    }

    return false;
  }

  friend bool
  operator==(const Number &n1, int &n2) // Check if Number is equal to int
  {
    string num = n1.GetNumber(); // Get String Representation of Number
    bool zeroCheck = true;       // Zero check variables for number

    for (int i = 0; i < num.length(); i++) // Iterate and check if non zero digit exists
    {
      if (num.at(i) != '0')
      {
        zeroCheck = false;
        break;
      }
    }

    if (zeroCheck && (n2 == 0)) // If both are zero then equal
    {
      return true;
    }

    if (zeroCheck || (n2 == 0)) // If one is zero then not equal
    {
      return false;
    }

    size_t TrimIndexNum = num.find_first_not_of('0'); // Remove
    num = num.substr(TrimIndexNum);                   // Padding zeroes from number

    if (num == to_string(n2)) // Check if numbers are equal
    {
      return true;
    }

    return false;
  }

  friend bool operator<(const Number &n1, const Number &n2) // Check if Number is less than Number two
  {
    if (n1 == n2) // If they are equal then not less than
    {
      return false;
    }

    string numOne = n1.GetNumber(); // Get string representation of first number
    string numTwo = n2.GetNumber(); // Get string representation of second number

    bool zeroCheckOne = true, zeroCheckTwo = true; // Zero check for both numbers

    for (int i = 0; i < numOne.length(); i++) // Iterate and check if non zero digit exists
    {
      if (numOne.at(i) != '0')
      {
        zeroCheckOne = false;
        break;
      }
    }
    for (int i = 0; i < numTwo.length(); i++) // Iterate and check if non zero digit exists
    {
      if (numTwo.at(i) != '0')
      {
        zeroCheckTwo = false;
        break;
      }
    }

    if (zeroCheckTwo && zeroCheckTwo) // If both numbers are zero then not equal
    {
      return false;
    }
    if (zeroCheckOne && (!zeroCheckTwo)) // If first number is zero and second is not then true
    {
      return true;
    }
    if (!zeroCheckOne && zeroCheckTwo) // If first number is non-zero and second is zero then false
    {
      return false;
    }

    size_t TrimIndexNumOne = numOne.find_first_not_of('0'); // Removing
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0'); // Leading
    numOne = numOne.substr(TrimIndexNumOne);                // Zeroes
    numTwo = numTwo.substr(TrimIndexNumTwo);                // From both numbers

    if (numOne.length() > numTwo.length()) // If number one has more digits than number two than greater
    {
      return false;
    }
    if (numOne.length() < numTwo.length()) // If number two has less digits than number two than lesser
    {
      return true;
    }

    for (int i = 0; i < numOne.length(); i++) // If number two has same digits as number one
    {
      if (int(numOne.at(i) - '0') == int(numTwo.at(i) - '0')) // If digits are same continue to next digit
      {
        continue;
      }
      if (int(numOne.at(i) - '0') < int(numTwo.at(i) - '0')) // If first number digit is lesser than number is lesser
      {
        return true;
      }
      else // If first number digit is greater than number is greater
      {
        return false;
      }
    }

    return false;
  }

  friend bool
  operator<(const Number &n1, int &n2) // Check if Number is less than int
  {
    if (n1 == n2) // If both are equal then not lesser
    {
      return false;
    }

    string numOne = n1.GetNumber(); // Get String representation of first number
    string numTwo = to_string(n2);  // Get String representation of second number

    bool zeroCheckOne = true, zeroCheckTwo = true; // Zero check for both numbers

    for (int i = 0; i < numOne.length(); i++) // Iterate and check if non zero digit exists
    {
      if (numOne.at(i) != '0')
      {
        zeroCheckOne = false;
        break;
      }
    }

    for (int i = 0; i < numTwo.length(); i++) // Iterate and check if non zero digit exists
    {
      if (numTwo.at(i) != '0')
      {
        zeroCheckTwo = false;
        break;
      }
    }

    if (zeroCheckTwo && zeroCheckTwo) // Both numbers are zero and equal so not lesser
    {
      return false;
    }

    if (zeroCheckOne && (!zeroCheckTwo)) // First number is zero and second is non-zero therefore lesser
    {
      return true;
    }

    if (!zeroCheckOne && zeroCheckTwo) // First number is non-zero and second is zero therefore greater
    {
      return false;
    }

    size_t TrimIndexNumOne = numOne.find_first_not_of('0'); // Removing
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0'); // Leading
    numOne = numOne.substr(TrimIndexNumOne);                // Zeroes
    numTwo = numTwo.substr(TrimIndexNumTwo);                // From Both Numbers

    if (numOne.length() > numTwo.length()) // If first number has more digits it is greater
    {
      return false;
    }

    if (numOne.length() < numTwo.length()) // If first number has less digits it is less
    {
      return true;
    }

    for (int i = 0; i < numOne.length(); i++) // If both number have same digits
    {
      if (int(numOne.at(i) - '0') == int(numTwo.at(i) - '0')) // If both digits are equal continue to next digit
      {
        continue;
      }

      if (int(numOne.at(i) - '0') < int(numTwo.at(i) - '0')) // If first digit is lesser than number is lesser
      {
        return true;
      }

      else // If first digit is greater than number is greater
      {
        return false;
      }
    }

    return false;
  }

  friend Number operator%(const Number &n1, const Number &n2) // Number modulus Number
  {

    int z = 0;   // Check
    if (n1 == z) // If number one is zero
    {
      Number z2;
      z2.InsertAtEnd("0");
      return z2; // 0 mod X = 0
    }

    if (n1 < n2) // If second number greater than first number
    {
      return n1; // n1 mod n2 = n1
    }

    string numOne = n1.GetNumber(); // Get string representation of first number
    string numTwo = n2.GetNumber(); // Get string representation of second number

    size_t TrimIndexNumOne = numOne.find_first_not_of('0'); // Remove
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0'); // Leading
    numOne = numOne.substr(TrimIndexNumOne);                // Zeroes
    numTwo = numTwo.substr(TrimIndexNumTwo);                // From both Numbers

    int lenOne = numOne.length();
    int lenTwo = numTwo.length();

    if (numOne == numTwo) // If both numbers equal then mod is zero
    {
      Number zero;
      zero.InsertAtEnd("0");
      return zero;
    }

    Number nTwo;

    Number zero;           // Zero
    zero.InsertAtEnd("0"); // in Number Object

    storeNumber(&nTwo, numTwo); // Store numTwo in nTwo Object

    string quotientNode = "";
    string data;
    string remainder = "";

    bool firstTime = true; // Check if we first iteration of division

    while (!numOne.empty()) // Divide until first number becomes empty
    {
      if (firstTime)
      {
        if (numOne.length() >= lenTwo) // Check if first number has more or equal digits than second number
        {
          data = numOne.substr(0, lenTwo); // Extract first lenTwo digits from number One
          if (numOne.length() == lenTwo)   // If length of first number was equal to lenTwo
          {
            numOne = ""; // First Number is now empty
          }
          else
          {
            numOne = numOne.substr(lenTwo, numTwo.length()); // Remove first lenTwo numbers from first number
          }
        }
        firstTime = false; // No longer first iteration
      }
      else
      {
        data = remainder; // Data is remainder of last division
      }

      Number nOne;              // Store
      storeNumber(&nOne, data); // data in Number object

      if (nOne < nTwo) // If first number is less than second number
      {
        if (!numOne.empty()) // If first number is not empty
        {
          data = data + numOne.substr(0, 1); // Extract next digit from first number and add it to data
          nOne.DestroyList();                // Recreate
          storeNumber(&nOne, data);          // the data object
          if (numOne.length() == 1)          // If first number length was 1
          {
            numOne = ""; // First number is empty now
          }
          else
          {
            numOne = numOne.substr(1, numOne.length()); // Otherwise remove first digit from first number
          }
        }
        else
        {
          remainder = data; // If first number was empty no digit can make this number greater than second number
          goto nomore;      // Remainder is the result of mod
        }

        if (nOne < nTwo) // If first number is still less than second number
        {
          while (nOne < nTwo && !numOne.empty()) // Append digits from first number into data until first number is empty or data becomes greater than second number
          {
            quotientNode.push_back('0'); // Append zero to extract digit
            if (numOne.length() >= 1)
            {
              data = data + numOne.substr(0, 1);          // Extract digit from first number
              nOne.DestroyList();                         // Recreate
              storeNumber(&nOne, data);                   // the data Number Object
              numOne = numOne.substr(1, numOne.length()); // Remove first digit from first number
            }
            else
            {
              numOne = "";      // Number is empty now
              remainder = data; // Data is now remainder
              goto nomore;      // Remainder is the result of mod
            }
          }
        }
      }

      int multiple = 0; // Count how many times it takes second number to make first number less than it

      while (!(nOne < nTwo)) // Subtract second number from first number until it becomes smaller
      {
        nOne = nOne - nTwo;
        multiple++;
      }

      quotientNode = quotientNode + to_string(multiple); // Multiple is pushed back into quotient

      nOne = clean(nOne);                                 // Remove leading zeroes from first number
      remainder = (nOne == zero) ? "" : nOne.GetNumber(); // If first number is empty remainder was zero otherwise remainder is the first number
    }

  nomore: // Incase of infinite loop

    Number result;
    storeNumber(&result, remainder); // Store remainder in result

    return result;
  }

  friend Number operator%(Number &n1, int &n2) // Number modulus int
  {
    int res = 0; // Initialize result as zero

    string num = n1.GetNumber(); // Get string representation of first number

    for (int i = 0; i < num.length(); i++) // Find mod
    {
      res = (res * 10 + num[i] - '0') % n2;
    }

    Number result;                      // Store
    result.InsertAtEnd(to_string(res)); // res in Number object

    return result;
  }

  friend Number operator/(const Number &n1, const Number &n2) // Divide two Numbers
  {
    int z = 0;   // Initialize zero
    if (n2 == z) // Check for division by zero
    {
      throw runtime_error("Math error: Attempted to divide by Zero\n");
      Number x;
      return x; // Not reached due to exception
    }

    if (n1 == z) // If the numerator is zero
    {
      Number z2;
      z2.InsertAtEnd("0"); // Result is zero
      return z2;
    }

    if (n1 < n2) // If numerator is less than denominator
    {
      Number z2;
      z2.InsertAtEnd("0"); // Result is zero
      return z2;
    }

    // Get string representations of both numbers
    string numOne = n1.GetNumber();
    string numTwo = n2.GetNumber();

    // Remove leading zeros
    size_t TrimIndexNumOne = numOne.find_first_not_of('0');
    size_t TrimIndexNumTwo = numTwo.find_first_not_of('0');
    numOne = numOne.substr(TrimIndexNumOne);
    numTwo = numTwo.substr(TrimIndexNumTwo);

    int lenOne = numOne.length();
    int lenTwo = numTwo.length();

    // If both numbers are equal, result is 1
    if (numOne == numTwo)
    {
      Number one;
      one.InsertAtEnd("1");
      return one;
    }

    Number nTwo; // Store the denominator
    Number zero;
    zero.InsertAtEnd("0");      // Zero in Number object
    storeNumber(&nTwo, numTwo); // Store numTwo in nTwo object

    string quotientNode = ""; // To store the result of the division
    string data;              // Holds the current segment being divided
    string remainder = "";    // To keep track of the current remainder
    bool firstTime = true;    // Flag to track the first iteration of division

    while (!numOne.empty()) // Continue until all digits in numOne are processed
    {
      // Handle the first iteration
      if (remainder == "" && firstTime)
      {
        if (numOne.length() >= lenTwo) // If enough digits in numOne
        {
          data = numOne.substr(0, lenTwo); // Take first lenTwo digits
          if (numOne.length() == lenTwo)
          {
            numOne = ""; // numOne is now empty
          }
          else
          {
            numOne = numOne.substr(lenTwo, numOne.length()); // Remove taken digits
          }
        }
        firstTime = false; // Now not the first iteration
      }
      else
      {
        firstTime = false; // Ensure not the first iteration
        data = remainder;  // Use previous remainder as current data
      }

      Number nOne;              // To store the current segment
      storeNumber(&nOne, data); // Store data in nOne

      if (nOne < nTwo) // If current segment is less than n2
      {
        if (!numOne.empty()) // If there are still digits in numOne
        {
          data = data + numOne.substr(0, 1); // Append next digit to data
          nOne.DestroyList();                // Clean up nOne
          storeNumber(&nOne, data);          // Update nOne with new data
          if (numOne.length() == 1)
          {
            numOne = ""; // numOne is now empty
          }
          else
          {
            numOne = numOne.substr(1, numOne.length()); // Remove first digit
          }
        }
        else
        {
          remainder = data; // No more digits, this is the remainder
          goto nomore2;     // Exit loop
        }

        // Keep appending digits from numOne until nOne is no longer less than nTwo
        if (nOne < nTwo)
        {
          while (nOne < nTwo && !numOne.empty())
          {
            quotientNode.push_back('0'); // Append zero to quotient
            if (numOne.length() >= 1)
            {
              data = data + numOne.substr(0, 1);          // Add next digit from numOne
              nOne.DestroyList();                         // Clean up nOne
              storeNumber(&nOne, data);                   // Update nOne
              numOne = numOne.substr(1, numOne.length()); // Remove first digit
            }
            else
            {
              numOne = "";      // numOne is now empty
              remainder = data; // Remainder is current data
              goto nomore2;     // Exit loop
            }
          }
        }
      }

      int multiple = 0;      // Count how many times n2 can be subtracted from n1
      while (!(nOne < nTwo)) // Keep subtracting nTwo from nOne
      {
        nOne = nOne - nTwo; // Subtract
        multiple++;         // Increment count
      }

      quotientNode = quotientNode + to_string(multiple); // Add count to quotient

      nOne = clean(nOne);                                 // Remove leading zeros from nOne
      remainder = (nOne == zero) ? "" : nOne.GetNumber(); // Determine remainder
    }

  nomore2: // End of division processing

    Number result;
    storeNumber(&result, quotientNode); // Store the quotient in result

    return result; // Return the result of the division
  }

  friend Number operator/(const Number &n1, const int &n2) // Divide number by int
  {
    Number n2T;
    string n2I = to_string(n2); // Convert int to a string
    storeNumber(&n2T, n2I);     // Store number in a Number class object

    Number result = n1 / n2T; // Proceed with Number-Number division

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

bool millerRabin(Number d, Number n) // Miller-Rabin primality test
{
  int one = 1;          // Initialize variable for comparison
  Number two;           // Common base for calculations
  two.InsertAtEnd("2"); // Set base to 2

  // Generate a random number between 2 and n-2
  Number rand;
  rand.InsertAtEnd(to_string(distr(eng))); // Generate random number

  Number temp2 = n - 4;         // Set upper limit for random number
  Number temp = (rand % temp2); // Ensure the random number is within the limit
  Number a = two + temp;        // a = random number + 2
  Number x = modpow(a, d, n);   // Calculate a^d mod n

  // Check if x is either 1 or n-1
  if ((x == one) || (x == (n - 1)))
  {
    return true; // Passes the test
  }

  // Iteratively check for non-trivial roots of unity
  while (!(d == (n - 1)))
  {
    x = (x * x) % n; // Square x and reduce mod n
    d = d * 2;       // Double d
    if (x == one)
    {
      return false; // Composite number
    }
    if (x == (n - 1))
    {
      return true; // Passes the test
    }
  }
  return false; // Composite number
}

bool isPrime(Number n, int k) // Function to check if n is prime
{
  int four = 4;
  int one = 1;
  int three = 3;

  // Handle small values and known non-primes
  if (((n < one) || (n == one)) || (n == four))
  {
    return false; // 0, 1, and 4 are not prime
  }
  if (n < three || n == three)
  {
    return true; // 2 and 3 are prime
  }

  // Check for divisibility by 3
  string *num = new string;
  *num = n.GetNumber();
  int result = 0;

  for (int i = 0; i < (*num).length(); i++)
  {
    result += int((*num).at(i) - '0'); // Sum of digits
  }

  delete num; // Free allocated memory

  if (result % 3 == 0)
  {
    return false; // Divisible by 3
  }

  // Check for even numbers and multiples of 5
  int test = int(n.GetNumber().back() - '0'); // Last digit
  if (test % 2 == 0 || test == 0 || test == 5)
  {
    return false; // Not prime
  }

  // Write n-1 as d * 2^r
  Number d = n - 1;
  while ((int(d.GetNumber().back() - '0') % 2) == 0)
  {
    d = (d / 2); // Divide d by 2
  }

  // Perform Miller-Rabin test k times
  for (int i = 0; i < k; i++)
  {
    if (!millerRabin(d, n))
    {
      return false; // Composite number
    }
  }

  return true; // n is likely prime
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