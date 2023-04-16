#pragma once

#include "utils.hpp"
#include "datasrc.hpp"

#include <array>
#include <vector>
#include <set>
#include <unordered_map>

template<uint8_t logD = 1, typename V = uint64_t, V NYTvalue = std::numeric_limits<V>::max()>
class HuffmanTreeFGK {
public:
    static constexpr uint8_t D = 1 << logD;

    HuffmanTreeFGK(size_t);
    size_t height() const;

    DataType getcode(size_t);
    DataType getnytcode();

    DataType encode(const V);
    V decode(DataSrc&);

    const size_t bits;

private:
    struct Node {
        bool leaf;
        size_t idx, order, parent;
        size_t freq, height;
        union {
            V value;
            std::array<size_t, D> cls;
        };
        Node(size_t, size_t, size_t, V);
    };

    size_t newNode(V, size_t, size_t = 1);

    void update(V);
    void fixH(size_t);
    void inc(size_t);
    void swap(size_t, size_t);

    std::unordered_map<V, size_t> table;
    // std::unordered_map<size_t, std::set<size_t>> freqIds;

    size_t root, NYT;
    std::vector<Node> nodes;
    std::vector<size_t> orders;

    void debug();
};
