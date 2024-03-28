#ifndef _STL_MAP_FUNC_H_
#define _STL_MAP_FUNC_H_

#include <map>
/**
* Generates a 2d map, std::map<key, std::map<key, val> >
* Used as it also generates all the default settings. This means it can be used with map2dDoesExist()
*/
#define STD_MAP2d(key, val)                        \
    std::map<                                      \
        key,                                       \
        std::map<key, val>,                        \
        std::less<key>,                            \
        std::allocator<                            \
            std::pair<                             \
                const key,                         \
                std::map<                          \
                    key,                           \
                    val,                           \
                    std::less<key>,                \
                    std::allocator<                \
                        std::pair<                 \
                            const key,             \
                            val                    \
                        >                          \
                    >                              \
                >                                  \
            >                                      \
        >                                          \
    >

/**
* Checks if a item on a 2d map exists
* @param K the map key class
* @param V the map value class
* @param k1 the first key
* @param k1 the section key
* @param m the map
*/

template<class K, class V>
inline bool map2dDoesExist(K k1, K k2, std::map<K, std::map<K, V> > &m) {
    if (m.find(k1) != m.end()) {
        if (m[k1].find(k2) != m[k1].end()) {
            return true;
        }
    }

    return false;
}

#endif

