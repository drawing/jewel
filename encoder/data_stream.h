#ifndef DATA_STREAM
#define DATA_STREAM

#include <string>

#include <arpa/inet.h>
/*
extern "C" {
	uint32_t htonl(uint32_t hostlong);
	uint16_t htons(uint16_t hostshort);
	uint32_t ntohl(uint32_t netlong);
	uint16_t ntohs(uint16_t netshort);
}
*/

namespace jewel {

namespace manip {
	template <typename T>
	struct ProtobufManip {
		ProtobufManip(T & msg, size_t len) : _msg(msg), _len(len) {}
		T & _msg;
		size_t _len;
	};

	template <typename T>
	inline ProtobufManip<T> protobuf(T & msg, size_t len = 0) {
		return ProtobufManip<T>(msg, len);
	}

	struct StringManip {
		StringManip(std::string & s, uint16_t len) : _s(s), _len(len) {}
		std::string & _s;
		uint16_t _len;
	};

	inline StringManip in_string(std::string & s, size_t len) {
		return StringManip(s, len);
	}
}


/*
class StringWithLenManip {
public:
	StringWithLenManip(std::string & s, uint16_t len) : _s(s), _len(len) {}
public:
	std::string & _s;
	uint16_t _len;
};
*/

class OutDataStream {
public:
	OutDataStream() {
	}
public:
	friend inline OutDataStream & operator << (OutDataStream & out, uint8_t value) {
		out._buf.push_back(value);
		return out;
	}

	friend inline OutDataStream & operator << (OutDataStream & out, uint16_t value) {
		uint16_t tmp = htons(value);
		out._buf.append((const char*)&tmp, sizeof(tmp));
		return out;
	}

	friend inline OutDataStream & operator << (OutDataStream & out, uint32_t value) {
		uint32_t tmp = htonl(value);
		out._buf.append((const char*)&tmp, sizeof(tmp));
		return out;
	}

	friend inline OutDataStream & operator << (OutDataStream & out, const std::string & value) {
		out._buf.append(value);
		return out;
	}

	template <typename T>
	friend inline OutDataStream & operator << (OutDataStream & out, manip::ProtobufManip<T> value) {
		std::string bussi_buf;
		value._msg.SerializeToString(&bussi_buf);
		return out << bussi_buf;
	}

	std::string data() {
		return _buf;
	}

	void clear() {
		_buf.clear();
	}

private:
	std::string _buf;
};

class InDataStream {
public:
	InDataStream() : _cur(0), _succ(true) {}
	InDataStream(const char * p, int len) : _buf(p, len), _cur(0), _succ(true) {}
	InDataStream(const std::string & buf) : _buf(buf), _cur(0), _succ(true) {}
public:
	friend inline InDataStream & operator >> (InDataStream & in, uint8_t & value) {
		if (in._succ && in._buf.size() >= in._cur + sizeof(value)) {
			value = in._buf[in._cur++];
		}
		else {
			in._succ = false;
		}
		return in;
	}

	friend inline InDataStream & operator >> (InDataStream & in, uint16_t & value) {
		if (in._succ && in._buf.size() >= in._cur + sizeof(value)) {
			value = ntohs(*(uint16_t*)(in._buf.data() + in._cur));
			in._cur += sizeof(value);
		}
		else {
			in._succ = false;
		}
		return in;
	}

	friend inline InDataStream & operator >> (InDataStream & in, uint32_t & value) {
		if (in._succ && in._buf.size() >= in._cur + sizeof(value)) {
			value = ntohl(*(uint32_t*)(in._buf.data() + in._cur));
			in._cur += sizeof(value);
		}
		else {
			in._succ = false;
		}
		return in;
	}


	/*
	friend inline InDataStream & operator >> (InDataStream & in, StringWithLenManip && value) {
		if (in._succ && in._buf.size() >= in._cur + (size_t)value._len) {
			value._s = in._buf.substr(in._cur, value._len);
			in._cur += value._len;
		}
		else {
			in._succ = false;
		}
		return in;
	}
	*/

	friend inline InDataStream & operator >> (InDataStream & in, manip::StringManip value) {
		if (in._succ && in._buf.size() >= in._cur + (size_t)value._len) {
			value._s = in._buf.substr(in._cur, value._len);
			in._cur += value._len;
		}
		else {
			in._succ = false;
		}
		return in;
	}

	template <typename T>
	friend inline InDataStream & operator >> (InDataStream & in, manip::ProtobufManip<T> value) {
		if (in._succ && in._buf.size() >= in._cur + (size_t)value._len) {
			std::string tmp = in._buf.substr(in._cur, value._len);
			bool ret = value._msg.ParseFromArray(tmp.data(), static_cast<int>(tmp.size()));
			if (!ret) {
				in._succ = false;
				return in;
			}
		}
		else {
			in._succ = false;
		}
		return in;
	}

	explicit operator bool() const {
		return _succ;
	}

private:
	std::string _buf;
	int _cur;
	bool _succ;
};

}

#endif // DATA_STREAM

