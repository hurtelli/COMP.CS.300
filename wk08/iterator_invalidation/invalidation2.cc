#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;


/**
 * @brief Erases every second item from the vector. Example: {1, 2, 3, 4} -> {1, 3}
 *
 * @param vec vector where every second item is erased.
 */
void eraseEverySecond(std::vector<int>& vec) {
    for(auto it = next(vec.begin(),1);it != vec.end();){
        it=vec.erase(it);
        if(it!=vec.end()){it++;}
    }
}

