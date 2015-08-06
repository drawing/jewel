#ifndef DATA_STREAM
#define DATA_STREAM

#include <string>

extern "C" {
	uint32_t htonl(uint32_t hostlong);
	uint16_t htons(uint16_t hostshort);
	uint32_t ntohl(uint32_t netlong);
	uint16_t ntohs(uint16_t netshort);
}

namespace jewel {

class StringManip {
public:
	StringManip(std::string & s, uint16_t & len) : _s(s), _len(len) {}
public:
	std::string & _s;
	uint16_t & _len;
};

class StringWithLenManip {
public:
	StringWithLenManip(std::string & s, uint16_t len) : _s(s), _len(len) {}
public:
	std::string & _s;
	uint16_t _len;
};

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

	friend inline OutDataStream & operator << (OutDataStream & out, const StringManip & value) {
		return out << (uint16_t)value._len << value._s;
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

	friend inline InDataStream & operator >> (InDataStream & in, StringManip && value) {
		in >> value._len;
		in >> StringWithLenManip(value._s, value._len);
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

