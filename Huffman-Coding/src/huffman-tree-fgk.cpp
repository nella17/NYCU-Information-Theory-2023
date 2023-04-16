#include "huffman-tree-fgk.hpp"

#include <iostream>
#include <iomanip>

template<uint8_t logD, typename V, V NYTvalue>
HuffmanTreeFGK<logD, V, NYTvalue>::HuffmanTreeFGK::
Node::Node(size_t i, size_t p, size_t f, V v):
    leaf(true), idx(i), order(i), parent(p), freq(f), height(1), value(v) {}

template<uint8_t logD, typename V, V NYTvalue>
size_t
HuffmanTreeFGK<logD, V, NYTvalue>::HuffmanTreeFGK::
newNode(V v, size_t p, size_t f) {
    size_t idx = nodes.size();
    nodes.emplace_back(idx, p, f, v);
    // freqIds[f].emplace(idx);
    orders.emplace_back(idx);
    if (v != NYTvalue)
        table.emplace(v, idx);
    return idx;
}

template<uint8_t logD, typename V, V NYTvalue>
HuffmanTreeFGK<logD, V, NYTvalue>::
HuffmanTreeFGK(size_t _b):
    bits(_b), nodes{}
{
    root = NYT = newNode(NYTvalue, 0, 0);
}

template<uint8_t logD, typename V, V NYTvalue>
size_t
HuffmanTreeFGK<logD, V, NYTvalue>::
height() const {
    return nodes[root].height;
}

template<uint8_t logD, typename V, V NYTvalue>
DataType
HuffmanTreeFGK<logD, V, NYTvalue>::
getcode(size_t cur) {
    DataType dt{};
    dt.reserve(height());
    while (cur != root) {
        auto par = nodes[cur].parent;
        // TODO: iter D cls
        dt.push_back(nodes[par].cls[1] == cur);
        cur = par;
    }
    std::reverse(dt.begin(), dt.end());
    return dt;
}

template<uint8_t logD, typename V, V NYTvalue>
DataType
HuffmanTreeFGK<logD, V, NYTvalue>::
encode(const V v) {
    DataType code;
    auto it = table.find(v);
    if (it != table.end()) {
        code = getcode(it->second);
    } else {
        code = getcode(NYT);
        for (size_t i = 0; i < bits; i++)
            code.push_back( (v >> (bits-1-i)) & 1);
    }
    update(v);
    // std::cerr << "done " << v << std::endl;
    // debug();
    return code;
}

template<uint8_t logD, typename V, V NYTvalue>
V
HuffmanTreeFGK<logD, V, NYTvalue>::
decode(DataSrc&) {
    // TODO
}

template<uint8_t logD, typename V, V NYTvalue>
void
HuffmanTreeFGK<logD, V, NYTvalue>::
update(V v) {
    auto it = table.find(v);
    size_t cur;
    if (it != table.end()) {
        cur = it->second;
    } else {
        auto par = NYT;
        auto nxt = newNode(v, par);
        NYT = newNode(NYTvalue, par, 0);
        nodes[par].leaf = false;
        nodes[par].cls = { nxt, NYT };
        fixH(par);
        if (par != root) inc(par);
        cur = nodes[par].parent;
    }
    while (cur != root) {
        // std::cerr << "update " << cur << " " << root << std::endl;
        size_t nxt = cur;
        for (size_t i = nodes[cur].order; i; i--)
            if (nodes[ orders[i-1] ].freq == nodes[cur].freq)
                nxt = orders[i-1];
        // auto nxt = orders[ *freqIds[ nodes[cur].freq ].begin() ];
        // debug();
        swap(cur, nxt);
        inc(cur);
        cur = nodes[cur].parent;
    }
    inc(root);
}

template<uint8_t logD, typename V, V NYTvalue>
void
HuffmanTreeFGK<logD, V, NYTvalue>::
fixH(size_t cur) {
    if (nodes[cur].leaf)
        nodes[cur].height = 1;
    else
        nodes[cur].height = 1 + std::max(
            nodes[ nodes[cur].cls[0] ].height,
            nodes[ nodes[cur].cls[1] ].height
        );
}

template<uint8_t logD, typename V, V NYTvalue>
void
HuffmanTreeFGK<logD, V, NYTvalue>::
inc(size_t cur) {
    // std::cerr << "inc " << cur << std::endl;
    auto& f = nodes[cur].freq;
    auto o = nodes[cur].order;
    // assert( freqIds[f].erase(o) );
    // if (freqIds[f].empty()) freqIds.erase(f);
    f++;
    // assert( freqIds[f].emplace(o).second );
}

template<uint8_t logD, typename V, V NYTvalue>
void
HuffmanTreeFGK<logD, V, NYTvalue>::
swap(size_t a, size_t b) {
    auto pa = nodes[a].parent, pb = nodes[b].parent;
    if (a == b or a == root or b == root or pa == b or a == pb)
        return;

    // std::cerr << "swap " << a << " " << b << std::endl;

    // TODO: iter D cls
    std::swap(
        nodes[pa].cls[ nodes[pa].cls[1] == a ],
        nodes[pb].cls[ nodes[pb].cls[1] == b ]
    );

    auto oa = nodes[a].order, ob = nodes[b].order;
    std::swap(orders[oa], orders[ob]);
    std::swap(nodes[a].parent, nodes[b].parent);
    std::swap(nodes[a].order, nodes[b].order);

    fixH(pa);
    fixH(pb);

    // std::swap(a, b);
}

template<uint8_t logD, typename V, V NYTvalue>
void
HuffmanTreeFGK<logD, V, NYTvalue>::
debug() {
    // return;
    /*
    std::cerr << " nodes: \n";
    for (auto n: nodes) {
        std::cerr << "  " << n.idx << ' ' << n.order << " | " << n.freq << ' ' << n.parent << " / ";
        if (n.leaf) std::cerr << std::hex << n.value << '(' << char(n.value) << ')';
        else std::cerr << n.cls[0] << ' ' << n.cls[1];
        std::cerr << '\n';
    }
    std::cerr << " freq: \n";
    for(const auto& [f, s]: freqIds) {
        std::cerr << "  " << f << " -> ";
        for (auto x: s) std::cerr << x << ' ';
        std::cerr << '\n';
    }
    std::cerr << " table: \n";
    for (auto [v, i]: table) {
        std::cerr << "  " << getcode(v) << " -> "
            << std::hex << v << '(' << char(v) << ')'
            << " " << i << '\n';
    }
    std::cerr << std::endl;
    //*/
}

template class HuffmanTreeFGK<1, uint64_t, (uint64_t)-1>;
