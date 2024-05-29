#pragma once

#define CONCAT1(sep, p1) p1
#define CONCAT2(sep, p1, p2) p1##sep##p2
#define CONCAT3(sep, p1, p2, p3) p1##sep##p2##sep##p3
#define CONCAT4(sep, p1, p2, p3, p4) p1##sep##p2##sep##p3##sep##p4
#define CONCAT5(sep, p1, p2, p3, p4, p5) p1##sep##p2##sep##p3##sep##p4##sep##p5

#define COUNT_N(_1, _2, _3, _4, _5, N, ...)    N
#define COUNT(...)   COUNT_N(__VA_ARGS__, 5, 4, 3, 2, 1)

#define IDENTITY(N) N
#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))

#define DISPATCH(FUNC, N) CONCAT ## N
#define CONCAT(sep, ...) IDENTITY(APPLY(DISPATCH, CONCAT, COUNT(__VA_ARGS__)))(sep, __VA_ARGS__)

#define STRINGIFY(x) #x

#define VA_LIST(...) __VA_ARGS__

