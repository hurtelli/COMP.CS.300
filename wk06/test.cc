#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <list>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "test.hh"

using namespace std;

void usage()
{
    cout << "Usage: test_iteration <test_function> [<num_of_items>]" << endl;
    cout << "  test_function: 1|2|3|4 (number of the function to test as in iteration1.cc, iteration2.cc, ...)" << endl;
    cout << "   num_of_items: number of items in the test set (optional, default=10)" << endl << endl;
    cout << "Examples:" << endl;
		cout << "  test_iteration 2        (test printEverySecond() with a list of 10 items)" << endl;
		cout << "  test_iteration 4 30     (test printReverse() with a list of 30 items)" << endl;
		cout << "  test_iteration 1 100    (test printAllItems() with a list of 100 items)" << endl;
}

int getRandom(int upper)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, upper);
  return dis(gen);
}

std::list<int> getRandomList(std::size_t size)
{
  std::set<int> s;
  while (s.size() < size)
  {
    int value = getRandom(size * 2);
    s.insert(value);
  }

  std::list<int> lst(s.begin(), s.end());
  return lst;
}

void printData(const std::vector<int>& data)
{
	// Using vector and regular indexing and not iterators on purpose here
	// to avoid revealing how to loop with iterators
	for (std::size_t i = 0; i < data.size(); ++i) {
		cout << data[i] << " ";
	}
	cout << endl << endl;
}

int test(int test_id, int size)
{
  std::list<int> lst = getRandomList(size);
	std::vector<int> vect(lst.begin(), lst.end());


  switch (test_id)
  {
  case 1:
		cout << "Testing function printAllItems() with the following data:" << endl << endl;
		printData(vect);
		cout << "Your function printed the following values:" << endl << endl;
    printAllItems(lst);
    break;
  case 2:
		cout << "Testing function printEverySecond() with the following data:" << endl << endl;
		printData(vect);
		cout << "Your function printed the following values:" << endl << endl;
    printEverySecond(lst);
    break;
  case 3:
		cout << "Testing function printHalf() with the following data:" << endl << endl;
		printData(vect);
		cout << "Your function printed the following values:" << endl << endl;
    printHalf(lst);
    break;
  case 4:
		cout << "Testing function printReverse() with the following data:" << endl << endl;
		printData(vect);
		cout << "Your function printed the following values:" << endl << endl;
    printReverse(lst);
    break;
  default:
    cout << "ERROR: Unknown function_to_test: " << test_id << endl;
    cout << "       Valid values are: 1,2,3,4" << endl;
    usage();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
  // size must be given as the first command line param
  if (argc < 2) {
    usage();
    return EXIT_FAILURE;
  }

  char *p;
  long l_test = strtol(argv[1], &p, 10);
  int test_id = (int)l_test;

  int size = 10;

	if (argc > 2) {
		long l_size = strtol(argv[2], &p, 10);
		size = (int)l_size;
	}

	if (size < 10) {
    cout << "ERROR: Invalid number of items: " << size << endl;
    cout << "       Number of items should be an integer larger than or equal to 10." << endl;
    usage();
    return EXIT_FAILURE;
	}

  return test(test_id, size);
}
