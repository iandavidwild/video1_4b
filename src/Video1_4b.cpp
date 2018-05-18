/*
 * Video1_4b.cpp
 *
 *  Created on: 18 May 2018
 *      Author: Ian Wild
 */

#include <iostream>
#include <utility>
#include <memory>

using namespace std;

class Averages {
	private:
		int * ptrSum;
		int * ptrCount;

	public:
		Averages(int sum, int count) {
			// Default constructor
			ptrSum = new int(sum);
			ptrCount = new int(count);
		}

		~Averages() {
			delete ptrSum;
			delete ptrCount;
		}

		float getMean() {
			if(*ptrCount == 0) {
				throw runtime_error("Divide by zero");
			}
			float mean = *ptrSum / *ptrCount;
			return mean;
		}

		void setSum(int newSum) {
			*ptrSum = newSum;
		}

		int getSum() {
			return *ptrSum;
		}

		void setCount(int newCount) {
			*ptrCount = newCount;
		}

		int getCount() {
			return *ptrCount;
		}
};

class SmartAverages {
	private:
		unique_ptr<int> ptrSum;
		unique_ptr<int> ptrCount;

	public:
		SmartAverages(int sum, int count): ptrSum(new int(sum)), ptrCount(new int(count))  {
		}

		~SmartAverages() {
			// clean up
			ptrSum.release();
			ptrCount.release();
		}

		float getMean() {
			// avoid a divide by zero error
			if(*ptrCount == 0) {
				throw runtime_error("Divide by zero");
			}
			float mean = *ptrSum / *ptrCount;
			return mean;
		}

		void setSum(int newSum) {
			*ptrSum = newSum;
		}

		int getSum() {
			return *ptrSum;
		}

		void setCount(int newCount) {
			*ptrCount = newCount;
		}

		int getCount() {
			return *ptrCount;
		}
};

int main() {
	// Bad use of pointers

	Averages * av = new Averages(20, 5);

	try {
		float mean = av->getMean();
		cout << mean << endl;
	} catch (...) {
		cout << "Oops!" << endl;
	}
	delete av;

	// Try again but cause an exception
/*
	av = new Averages(20, 0);

	try {
		// check the count
		float mean = av->getMean();
		cout << mean << endl;
	} catch (...) {
		cout << "Oops!" << endl;
	}
	delete av;
*/
	// Re-implement using smart pointers
	unique_ptr<SmartAverages> smartAv = make_unique<SmartAverages>(20, 0);

	try {
		cout << smartAv->getSum() << endl;
		cout << smartAv->getCount() << endl;
		cout << smartAv->getMean() << endl;
	} catch (...) {
		cout << "Oops!" << endl;
	}

	smartAv.release();

	// Reference counted pointers
	shared_ptr<SmartAverages> av1 = make_shared<SmartAverages>(20, 5);
	shared_ptr<SmartAverages> av2 = av1;

	av1.reset(); // av1 is now null but the memory is still in use

	// av2 still points to something useful
	cout << av2->getSum() << endl;

	av2.reset(); // all memory is now released


	// Weak pointers (no reference count)
	av1 = make_shared<SmartAverages>(20, 5);
	weak_ptr<SmartAverages> avWeak = av1;

	if(avWeak.use_count() > 0) {
		auto usePtr = avWeak.lock(); //weak pointers need to be copied into shared pointers before use
		cout << usePtr->getSum() << endl;
		usePtr.reset();
	}

	// reset the original pointer
	av1.reset();

	// now avWeak doesn't point to anything
	if(avWeak.use_count() > 0) {
		usePtr = avWeak.lock();
		cout << usePtr->getSum() << endl;
	}

	return 0;
}
