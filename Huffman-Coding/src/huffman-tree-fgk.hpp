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

    DataType encode(const V);
    V decode(DataSrc&);

    const size_t bits;

private:
    struct Node {
        bool leaf;
        size_t freq, height;
        union {
            V value;
            std::array<size_t, D> cls;
        };
        Node();
        Node(V);
    };

    size_t newNode();
    size_t newNode(V);

    void update(size_t);

    // /TODO
    // std::unordered_map<V, DataType> table;
    std::unordered_map<size_t, std::set<size_t>> weightIds;

    size_t root, NYT;
    std::vector<Node> nodes;
};
