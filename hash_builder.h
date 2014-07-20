#ifndef HASH_BUILDER_H
#define HASH_BUILDER_H

#include "hash_table.h"
/*
 A mem hash table builder
*/
template <typename T>
HashTable<T> * CreateMemHash(int row, int col)
{
	HashTable<T> * res = new HashTable<T>();
	int iSize = res->GetHashSize(row, col);

	char * base = new char[iSize];
	memset(base, 0, iSize);

	res->Init(base, iSize, row, col);
	return res;
}

#endif // HASH_BUILDER_H
