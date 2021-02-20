#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <set>
#include <type_traits>
#include <vector>



template <typename T=int>
std::vector<std::vector<T>> handh_function(size_t n);


int main(int argc, char const* argv[]) {
    std::srand(std::time(nullptr));

    const size_t n = (2 == argc && 0 <= std::atoi(argv[1])) ? std::atoi(argv[1]) % 100 + 1 : 10;
    std::cout << "n == " << n << std::endl;
    

    auto res = handh_function(n);

    return 0;
}




template<typename T>
class ThreadSafetySet {
public:
    bool contains(const T& value) const {
        std::lock_guard<std::mutex> l(_mtx);
        return _set.contains(value);
    }

    void insert(const T& value) {
        std::lock_guard<std::mutex> l(_mtx);
        _set.insert(value);
    }

private:
    mutable std::mutex _mtx;
    std::set<T> _set;
};


template <typename T>
std::vector<std::vector<T>> handh_function(size_t n) {

    std::vector<std::vector<T>> result;
    result.reserve(n);

    auto randomizer = (std::is_integral<T>::value) ? []{ return std::rand() - std::rand(); }
                                                   : []{ return std::rand() - std::rand() + static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX); };


    ThreadSafetySet<size_t> vector_sizes;

    #pragma omp parallel 
    {
        #pragma omp for
        for(size_t i = 0; i < n; ++i) {

            size_t size;
            do {
                size = std::rand() % 100;   // NOTE: "% 100" for a quick demo
            } while(vector_sizes.contains(size));

            vector_sizes.insert( size );

            std::vector<T> vec;
            vec.reserve(size);
            std::generate_n(std::back_inserter(vec), size, randomizer);

            result.push_back(std::move(vec));
        }

        #pragma omp for ordered
        for(size_t i = 0; i < result.size(); ++i) {
            (0 == i % 2) ? std::sort(result.at(i).begin(), result.at(i).end())
                         : std::sort(result.at(i).begin(), result.at(i).end(), std::greater<T>());
        }

    }

    return result;
}
