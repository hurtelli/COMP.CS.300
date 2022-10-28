#include <iterator>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


/**
 * @brief duplicates even numbers in the vector, removes uneven numbers. Example: {1, 2, 3 4} -> {2, 2, 4, 4}
 *
 * @param vec vector to be handled
 */
void duplicateEvenRemoveUneven(std::vector<int>& vec) {
    for(auto it = vec.begin();it!=vec.end();++it){
        if(*it%2==0){
            int v =*it;
            vec.insert(it+1,v);
            ++it;
        }
        else{
            vec.erase(it);
            --it;
        }
    }
}

