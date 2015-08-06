#ifndef PROTOBUF_STREAM
#define PROTOBUF_STREAM

#include "data_stream.h"

namespace jewel {

inline OutDataStream & operator << (OutDataStream & out, MessageLite * msg) {
	return out;
}


}

#endif // PROTOBUF_STREAM

