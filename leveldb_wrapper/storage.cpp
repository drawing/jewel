
#include "storage.h"

#include "leveldb/write_batch.h"
#include "leveldb/cache.h"

namespace jewel
{
	
int Storage::StorageKeyComparator::Compare(const leveldb::Slice& a, const leveldb::Slice& b) const
{
	if (a.size() == 0 || b.size() == 0) {
		return a.size() - b.size();
	}
	
	if (a[0] != b[0]) {
		return a[0] - b[0];
	}
	
	uint8_t type = a[0];
	
	auto it = cmp_.find(type);
    if (it == cmp_.end()) {
		assert("no compare");
		return -1;
	}
    
	return it->second->Compare(std::string(a.data() + 1, a.size() - 1),
			std::string(b.data() + 1, b.size() - 1));
}

bool DeserializeFromString(const std::string & skey, std::string * key)
{
	*key = skey;
	return true;
}
bool SerializeToString(const std::string & key, std::string * skey)
{
	*skey = key;
	return true;
}
bool SerializeToString(uint64_t ikey, std::string * out)
{
	out->assign((char*)&ikey, sizeof(ikey));
	return true;
}
bool DeserializeFromString(const std::string & key, uint64_t * out)
{
	if (key.size() != sizeof(uint64_t)) {
		return false;
	}
	*out = *(uint64_t*)key.data();
	return true;
}
int CompareObject(const std::string & key1, const std::string & key2)
{
	return strcmp(key1.c_str(), key2.c_str());
}
int CompareObject(uint64_t key1, uint64_t key2)
{
	return key1 - key2;
}

bool Storage::Init(const std::string & path)
{
	leveldb::Options options;

    options.create_if_missing = true;
    options.comparator = cmp_;
    options.block_cache = leveldb::NewLRUCache(8 * 1048576);
    leveldb::Status s = leveldb::DB::Open(options, path, &db_);
    if (!s.ok()) {
        return false;
    }
	
	return true;
}

bool Storage::UnInit()
{
	if (db_ != nullptr) {
		delete db_;
		db_ = nullptr;
	}
	return true;
}

Storage::~Storage()
{
	UnInit();
	if (cmp_ != nullptr) {
		delete cmp_;
		cmp_ = nullptr;
	}
}

Storage::Storage()
{
	cmp_ = new StorageKeyComparator();
}

}
