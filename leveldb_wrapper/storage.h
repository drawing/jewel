
#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <map>

#include "leveldb/comparator.h"
#include "leveldb/db.h"

namespace leveldb {
class DB;
}

namespace jewel
{

enum DBStatus {
	DBStatusSucc					= 0,
	DBStatusNotFound				= 1,
	DBStatusNotInit					= 2,
	DBStatusUnkownFail				= 3,
};

bool DeserializeFromString(const std::string & skey, std::string * out);
bool SerializeToString(const std::string & key, std::string * out);
int CompareObject(const std::string & key1, const std::string & key2);

bool SerializeToString(uint64_t ikey, std::string * out);
bool DeserializeFromString(const std::string & key, uint64_t * out);
int CompareObject(uint64_t key1, uint64_t key2);

class DBCompareBase
{
public:
	virtual ~DBCompareBase() {};
	virtual int Compare(const std::string & s1, const std::string & s2) = 0;
};


template<typename T>
class DBCompare : public DBCompareBase
{
public:
	int Compare(const std::string & s1, const std::string & s2)
	{
		T t1, t2;
		bool bRet = DeserializeFromString(s1, &t1);
		if (!bRet) {
			return -1;
		}
		
		bRet = DeserializeFromString(s2, &t2);
		if (!bRet) {
			return -1;
		}
		
		return CompareObject(t1, t2);
	}
};


class Storage
{
public:
	bool Init(const std::string & path);
	bool UnInit();
	
	Storage();
	~Storage();
	
private:
	class StorageKeyComparator : public leveldb::Comparator {
	public:
	    int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const;
		const char* Name() const { return "StorageKeyCmp"; }
	    void FindShortestSeparator(std::string*, const leveldb::Slice&) const {}
	    void FindShortSuccessor(std::string*) const {}
		
	public:
		std::map<uint8_t, DBCompareBase*> cmp_;
	};
	
public:
	template<typename KEY, typename VALUE>
	DBStatus Get(uint8_t type, const KEY & key, VALUE * val) {
		if (db_ == nullptr) {
			return DBStatusNotInit;
		}
		
		std::string skey;
		std::string temp_skey;
		std::string sval;
		bool ret = SerializeToString(key, &temp_skey);
		if (!ret) {
			return DBStatusUnkownFail;
		}
		skey.push_back(type);
		skey.append(temp_skey);
		
		leveldb::Status s = db_->Get(leveldb::ReadOptions(), skey, &sval);
		if (s.IsNotFound()) {
			return DBStatusNotFound;
		}
		
		if (!s.ok()) {
			return DBStatusUnkownFail;
		}
		
		ret = DeserializeFromString(sval, val);
		if (!ret) {
			return DBStatusUnkownFail;
		}
		
		return DBStatusSucc;
	}
	
	template<typename KEY, typename VALUE>
	DBStatus Put(uint8_t type, const KEY & key, const VALUE & val) {
		if (db_ == nullptr) {
			return DBStatusNotInit;
		}
		
		std::string skey;
		std::string temp_skey;
		std::string sval;
		bool ret = SerializeToString(key, &temp_skey);
		if (!ret) {
			return DBStatusUnkownFail;
		}
		skey.push_back(type);
		skey.append(temp_skey);
		
		ret = SerializeToString(val, &sval);
		if (!ret) {
			return DBStatusUnkownFail;
		}
		
		leveldb::Status s = db_->Put(leveldb::WriteOptions(), skey, sval);
		if (!s.ok()) {
			return DBStatusUnkownFail;
		}
		
		return DBStatusSucc;
	}
	
	template<typename KEY>
	void Register(uint8_t type, const KEY & key) {
		cmp_->cmp_[type] = new DBCompare<KEY>();
	}
private:
	leveldb::DB* db_;
	StorageKeyComparator * cmp_;
};

}

#endif // STORAGE
