#ifndef TIMEFUNCTION_H
#define TIMEFUNCTION_H

#include <iostream>
#include <ctime>

/**
 * @brief Times the execution time of the callable f with arguments args, printing the results to cout.
 * 
 * @tparam Callable the type of the callable object
 * @tparam Args the arguments the callable accepts
 * @param f the function to be timed
 * @param args the arguments to f
 */
template <typename Callable, typename... Args>
void time_function (Callable f, Args... args) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    f(args...);
    clock_gettime(CLOCK_MONOTONIC, &end);
    long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    std::cout << "Elapsed time: " << elapsed_ns << " nanoseconds" << std::endl;
}

#endif