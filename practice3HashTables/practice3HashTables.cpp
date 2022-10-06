#include <iostream>
#include <vector>
#include <list>

using namespace std;


bool isPrime(const int n)
{
	for (int i = 2; i < n / 2; ++i)
	{
		if (n % i == 0)
		{
			return false;
		}
	}

	return true;
}
int nextPrime(const int num)
{
	for (int i = num + 1; i < 2 * num; ++i)
	{
		if (isPrime(i))
		{
			return i;
		}
	}
	return 0;
}

class SPHashTable 
{
private:
	vector<list<int>> m_theLists;
	int m_currentSize;

	void rehash() 
	{
		vector<list<int>> oldLists = this->m_theLists;
		this->m_theLists.clear();
		this->m_theLists.resize(nextPrime(2 * oldLists.size()));
		this->m_currentSize = 0;
		for (auto list : oldLists)
		{
			for (auto el : list)
			{
				this->insert(el);
			}
		}
	}

public:
	explicit SPHashTable(int size = 101) : m_currentSize(size)
	{
		this->m_theLists.resize(this->m_currentSize);
	}
	int load() const
	{
		return this->m_currentSize;
	}
	int size() const 
	{
		return this->m_theLists.size();
	}
	size_t hash(const int key) const
	{
		return key % this->m_theLists.size();
	}
	list<int> operator[](const int index) const
	{
		return this->m_theLists[index];
	}
	explicit SPHashTable(const vector<int> arr): m_currentSize(0)
	{
		this->m_theLists.resize(nextPrime(2 * arr.size()));
		for (int i = 0; i < arr.size(); ++i)
		{
			this->insert(arr[i]);
		}
	}
	void makeEmpty() 
	{
		for (auto& theList : this->m_theLists)
		{
			theList.clear();
		}
	}
	bool contains(const int data) const 
	{
		auto& theList = this->m_theLists[hash(data)];
		return find(begin(theList), end(theList), data) != end(theList);
	}
	bool remove(const int data) 
	{
		auto& theList = this->m_theLists[hash(data)];
		auto itr = find(begin(theList), end(theList), data);
		
		if (itr == end(theList))
		{
			return false;
		}

		--this->m_currentSize;
		theList.erase(itr);
	}
	bool insert(const int data)
	{
		auto& theList = this->m_theLists[hash(data)];
		
		if (this->contains(data))
		{
			return false;
		}

		theList.push_back(data);

		if (++this->m_currentSize > this->m_theLists.size())
		{
			this->rehash();
		}
	}
};

template <typename T>
class HashTable
{
private:
	enum State {
		ACTIVE,
		EMPTY,
		DELETED
	};
	struct HashObj
	{
		T element;
		State st;

		HashObj(const T& element = T{}, State st = EMPTY) : element{ element }, st{ st } {}
	};
	vector<HashObj> m_arr;
	int m_currentSize;
	void rehash()
	{
		vector<HashObj> tempArr = this->m_arr;
		this->m_arr.clear();
		this->m_currentSize = 0;
		this->m_arr.resize(nextPrime(2 * tempArr.size()));
		for (auto& element : tempArr)
		{
			if (element.st == ACTIVE)
			{
				this->insert(element.element);
			}
		}
	}
	size_t second_hash(const T& element) const
	{
		return 7 - element % 7;
	}
	size_t linear_probing(size_t hashIndex)
	{
		int k = 0;
		while (!this->isEmpty(hashIndex))
		{
			if (++hashIndex >= this->m_arr.size())
			{
				hashIndex -= this->m_arr.size();
			}
			++k;
		}
		cout << "Iterations for linear probing: " << k << endl;
		return hashIndex;
	}
	size_t quadratic_probing(size_t hashIndex)
	{
		size_t newHashIndex = hashIndex;
		int k = 0;
		for (int i = 0; !this->isEmpty(newHashIndex); ++i)
		{
			newHashIndex = hashIndex + pow(2, i);
			while (newHashIndex >= this->m_arr.size())
			{
				newHashIndex -= this->m_arr.size();
			}
			k = i;
		}
		cout << "Iterations for quadratic probing: " << k << endl;
		return newHashIndex;
	}
	size_t double_hashing(size_t hashIndex)
	{
		size_t newHashIndex = hashIndex;
		int k = 0;
		for (int i = 1; !this->isEmpty(newHashIndex); ++i)
		{
			newHashIndex = hashIndex + i * second_hash(hashIndex);
			while (newHashIndex >= this->m_arr.size())
			{
				newHashIndex -= this->m_arr.size();
			}
			k = i;
		}
		cout << "Iterations for double hashing: " << k << endl;
		return newHashIndex;
	}
public:
	int load() const
	{
		return this->m_currentSize;
	}
	int size() const
	{
		return this->m_arr.size();
	}
	T& operator[](const int index)
	{
		return this->m_arr[index].element;
	}
	size_t hash(const T& element) const
	{
		return element % this->m_arr.size();
	}
	HashTable(int size = 101) : m_currentSize{ 0 }
	{
		this->m_arr.resize(size);
	}
	HashTable(vector<T>& arr) : m_currentSize{ 0 }
	{
		this->m_arr.resize(nextPrime(2 * arr.size()));
		for (auto& element : arr)
		{
			this->insert(element);
		}
	}
	bool isEmpty(const int key)
	{
		return this->m_arr[key].st == EMPTY || this->m_arr[key].st == DELETED;
	}
	bool insert(const T& element)
	{
		size_t hashNum = this->hash(element);
		this->m_arr[this->double_hashing(hashNum)] = HashObj{ element, ACTIVE };
		if (++this->m_currentSize > this->m_arr.size() / 2)
		{
			this->rehash();
		}
		return true;
	}
};

void printHashTable(const SPHashTable& ht)
{
	for (int i = 0; i < ht.size(); ++i)
	{
		cout << i << "\t";
		for (auto el : ht[i])
		{
			cout << el << " ";
		}
		if (ht[i].empty())
		{
			cout << "x";
		}
		cout << endl;
	}
}
template<typename T>
void printHashTable(HashTable<T>& ht)
{
	for (int i = 0; i < ht.size(); ++i)
	{
		cout << i << "\t";
		if (ht.isEmpty(i))
		{
			cout << "x";
		}
		else
		{
			cout << ht[i];
		}
		cout << endl;
	}
}

int main()
{
	HashTable<int> ht{};
	for (int i = 0; i < 100; ++i)
	{
		ht.insert(rand() % 100);
	}
	printHashTable(ht);
}