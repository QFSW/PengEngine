#pragma once

// Macros to help work around the fact that Clang doesn't have support
// for execution policies

#ifdef PLATFORM_MAC

#define EXEC_SEQ
#define EXEC_PAR
#define EXEC_PAR_UNSEQ

#else

#define EXEC_SEQ       std::execution::seq, 
#define EXEC_PAR       std::execution::par, 
#define EXEC_PAR_UNSEQ std::execution::par_unseq, 

#endif