#ifndef HASH_BUILDER_H
#define HASH_BUILDER_H

#include "hash_table.h"

#include <sys/shm.h>
#include <sys/ipc.h>
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

template <typename T>
HashTable<T> * CreateShmHash(key_t key, int row, int col)
{
	HashTable<T> * res = new HashTable<T>();

	int iSize = res->GetHashSize(row, col);

	bool bCreated = false;
	int iShmID = 0;
	int iFlag = 0600;
	char * sShm = NULL;

	if((iShmID = shmget(key, iSize, iFlag)) < 0) {
		bCreated = true;
		iFlag |= IPC_CREAT;
		if ((iShmID = shmget(key, iSize, iFlag)) < 0) {
			return NULL;
		}
	}

	if((sShm = (char *) shmat(iShmID, NULL, 0)) == (char *) -1) {
		return NULL;
	}

	if (bCreated) {
		memset(sShm, 0, iSize);
	}
	
	res->Init(sShm, iSize, row, col);
	return res;
}

#endif // HASH_BUILDER_H
