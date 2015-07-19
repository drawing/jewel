#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>
#include <vector>
#include <ctime>
#include <cstring>

namespace jewel
{

/*
 A hash node must implement below interface
*/
class BaseNode
{
public:
	virtual bool Serialize(char * buffer, int len) const = 0;
	virtual bool Deserialize(char * buffer, int len) = 0;
	virtual uint32_t HashCode() const = 0;
	virtual bool Equal(const BaseNode & node) const = 0;
	virtual int FixedSize() = 0;
};

/*
 hash table implement,
 Type T is the hash node
*/
template <typename T>
class HashTable
{
public:
	bool Init(char * mem, int mem_size, int row, int col);
	bool Get(T & node);
	bool Put(T & node);

	int GetHashSize(int row, int col);
private:
	void BuildPrime();
private:
	std::vector<int> _prime;
	int _col;
	int _row;
	int _elem_size;
	char * _base;
};

#pragma pack(1)
struct HashElem
{
	time_t update_time;
	char data[0];
};
#pragma pack(0)

inline bool IsPrime(int num)
{
	if (num <= 2) {
		return true;
	}
	for (int i = 2; i <= num / 2; i++) {
		if ((num % i) == 0) {
			return false;
		}
	}
	return true;
}

template <typename T>
int HashTable<T>::GetHashSize(int row, int col)
{
	return (row * col) * (T().FixedSize() + sizeof(time_t));
}

template <typename T>
bool HashTable<T>::Init(char * mem, int mem_size, int row, int col)
{
	if (mem_size < this->GetHashSize(row, col)) {
		return false;
	}
	_base = mem;
	_row = row;
	_col = col;
	_elem_size = T().FixedSize();

	BuildPrime();
	return 0;
}

template <typename T>
void HashTable<T>::BuildPrime()
{
	_prime.clear();
	int last_key = _col;
	for (int i = 0; i < _row; i++) {
		while (!IsPrime(last_key)) {
			last_key --;
		}
		_prime.push_back(last_key);
		last_key --;
	}
}

template <typename T>
bool HashTable<T>::Get(T & node)
{
	uint32_t hash_code = node.HashCode();

	T cur_node;
	char * off = _base;
	for (int i = 0; i < _row; i++, off += _prime[i] * (_elem_size + sizeof(HashElem))) {
		char * cur = off + (hash_code % _prime[i]) * (_elem_size + sizeof(HashElem));
		HashElem * p = (HashElem*)cur;
		if (p->update_time == 0) {
			continue;
		}
		if (cur_node.Deserialize(p->data, _elem_size)) {
			if (cur_node.Equal(node)) {
				node = cur_node;
				return true;
			}
		}
	}
	return false;
}


template <typename T>
bool HashTable<T>::Put(T & node)
{
	uint32_t hash_code = node.HashCode();

	char * off = _base;
	HashElem * empty = NULL;
	HashElem * replace = NULL;
	T cur_node;
	for (int i = 0; i < _row; i++, off += _prime[i] * (_elem_size + sizeof(HashElem))) {
		char * cur = off + (hash_code % _prime[i]) * (_elem_size + sizeof(HashElem));
		HashElem * p = (HashElem*)cur;

		if (p->update_time == 0) {
			if (empty == NULL) {
				empty = p;
			}
			continue;
		}
		if (replace == NULL || replace->update_time > p->update_time) {
			replace = p;
			continue;
		}
		if (cur_node.Deserialize(p->data, _elem_size)) {
			if (cur_node.Equal(node)) {
				node.Serialize(p->data, _elem_size);
				p->update_time = time(NULL);
				return true;
			}
		}
	}

	if (empty != NULL) {
		node.Serialize(empty->data, _elem_size);
		empty->update_time = time(NULL);
		return true;
	}
	if (replace != NULL) {
		node.Serialize(replace->data, _elem_size);
		replace->update_time = time(NULL);
		return true;
	}
	return false;
}

}

#endif // HASHTABLE_H

