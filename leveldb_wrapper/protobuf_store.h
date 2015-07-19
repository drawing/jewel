
#ifndef PROTOBUF_STORE_H
#define PROTOBUF_STORE_H

#include <google/protobuf/message_lite.h>

namespace jewel
{

inline bool SerializeToString(const ::google::protobuf::MessageLite & msg, std::string * out) {
	bool bRet = msg.SerializeToString(out);
	if (!bRet) {
		return false;
	}
	return true;
}

inline bool DeserializeFromString(const std::string & key, ::google::protobuf::MessageLite * out) {
	if (!out->ParseFromArray(key.data(), key.size())) {
		return false;
	}
	return true;
}

}

#endif // PROTOBUF_STORE_H
