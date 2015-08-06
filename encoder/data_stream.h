#ifndef DATA_STREAM
#define DATA_STREAM

namespace jewel {

class InDataStream {
public:
	InDataStream() {
	}
public:
	friend inline InDataStream & opterator << (InDataStream & in, uint8_t value) {
		in._buf.push_back(value);
		return in;
	}

	friend inline InDataStream & opterator << (InDataStream & in, uint16_t value) {
		uint16_t tmp = htons(value);
		in._buf.append((const char*)&tmp, sizeof(tmp));
		return in;
	}

	friend inline InDataStream & opterator << (InDataStream & in, uint32_t value) {
		uint32_t tmp = htonl(value);
		in._buf.append((const char*)&tmp, sizeof(tmp));
		return in;
	}

	friend inline InDataStream & opterator << (InDataStream & in, const string & value) {
		in._buf.append(value);
		return in;
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

}

#endif // DATA_STREAM

