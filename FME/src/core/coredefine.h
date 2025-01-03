#pragma once

// debug 
#define PROFILER
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

// assertion
#define FME_ASSERT(condition, message) if (!condition) {FME_LOG_CRITICAL(message); std::abort();}

// utils

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define FME_CAT_NX(A, B) A##B

 /*
  * Concatenate preprocessor tokens A and B after macro-expanding them.
  */
#define FME_CAT(A, B) FME_CAT_NX(A, B)
