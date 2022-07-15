#pragma once

// draw

#define PREDEF_DEFAULT_POINT "dfa"

// serializer

#define PREDEF_FAKE_MATH_CTX "sfa"
#define PREDEF_RAISEMATH_CTX "sfb"

#define BEGIGN_PREDEF_ARG(cur_serializer,key)\
serializer tmps(((cur_serializer).mode() | serializer_mode::no_def),(cur_serializer).item_size());\
std::stringstream _outbuf;\
_outbuf << "\\def\\" << key

#define BEGIGN_PREDEF(cur_serializer,key)\
BEGIGN_PREDEF_ARG(cur_serializer,key);_outbuf<<'{'

#define END_PREDEF(cur_serializer,key)\
tmps.end_all_context();\
tmps.output_to(_outbuf);\
_outbuf << "}";\
(cur_serializer).add_predef(key, _outbuf.str())
