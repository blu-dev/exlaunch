#pragma once

#include <optional>

namespace exl::util {

/**
 * @brief Simple class to aid in lookups with keys of a numeric type. Does not require dynamically-allocated memory
 * 
 * @tparam K The key type (must be numeric)
 * @tparam T The value type
 * @tparam N The maximum number of entries to store (the larger the closer to O(1) lookups will be)
 * @tparam U The number of least-significant bits to ignore
 */
template<typename K, typename V, size_t N, size_t U = 0>
class BasicMap {
private:
    struct entry {
        K key;
        V value;
        bool valid;
    };

    static size_t GetIndex(K key) {
        return (key >> U) % N;
    }

    entry m_Contents[N];

public:
    BasicMap() {
        for (size_t i = 0; i < N; i++)
            m_Contents[i].valid = false;
    }

    std::optional<V> Get(K key) {
        auto cur_index = GetIndex(key);
        auto end_index = (cur_index + N - 1) % N;
        do {
            if (!m_Contents[cur_index].valid)
                return {};
            if (m_Contents[cur_index].key == key)
                return m_Contents[cur_index].value;
            cur_index = (cur_index + 1) % N;
        } while (cur_index != end_index);
        return {};
    }

    bool Insert(K key, V value) {
        auto cur_index = GetIndex(key);
        auto end_index = (cur_index + N - 1) % N;
        do {
            if (!m_Contents[cur_index].valid) {
                m_Contents[cur_index].valid = true;
                m_Contents[cur_index].key = key;
                m_Contents[cur_index].value = value;
                return true;
            }
            cur_index = (cur_index + 1) % N;
        } while (cur_index != end_index);
        return false;
    }
};

}