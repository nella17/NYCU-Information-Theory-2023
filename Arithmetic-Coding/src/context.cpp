#include "context.hpp"

#include "util.h"

Context::Context(Context* p, int o, uint32_t ch):
    parent(p),
    order(o), charset(ch),
    size(0), used(0), count(charset),
    next{}
{}

void Context::inc(uint32_t symbol) {
    used[symbol] = 1;
    count[symbol]++;
    size++;
}

bool Context::has(uint32_t symbol) {
    return used[symbol];
}

Context::Range Context::range(uint32_t symbol, Bits skip) {
    uint32_t accum = 0, total = 0;
    for (uint32_t s = 0; s < charset; s++) {
        if (!skip[s] and used[s]) {
            total += count[s];
            if (s < symbol)
                accum += count[s];
        }
    }
    bool zz = total > 0;
    auto cnt = symbol == charset-1 ? zz : count[symbol];
    return { accum, accum + cnt, total + zz };
}

Context* Context::find(uint32_t symbol) {
    if (order <= 0) return nullptr;
    if (next.empty())
        return nullptr;
    return next[symbol];
}

Context* Context::get(uint32_t symbol) {
    if (order <= 0) return nullptr;
    if (next.empty())
        next.assign(charset, nullptr);
    auto ctx = next[symbol];
    if (ctx != nullptr) return ctx;
    return next[symbol] = new Context(this, order-1, charset);
}

std::ostream& operator<<(std::ostream& os, const Context& ctx) {
    for (uint32_t s = 0; s < ctx.charset; s++)
        if (ctx.used[s])
            os _ s _ char(s) _ ctx.count[s] _ '\n';
    return os;
}

Arithmetic::Accum Context::getacc(Bits skip) {
    Arithmetic::Accum accum{};
    accum.reserve(2 + (used & ~skip).count());
    accum.emplace_back(0);
    for (uint32_t s = 0; s < charset; s++)
        if (!skip[s] and used[s])
            accum.emplace_back(accum.back() + count[s]);
    accum.emplace_back(accum.back() + 1);
    return accum;
}

uint32_t Context::i2s(uint32_t idx, Bits skip) {
    for (uint32_t s = 0, c = 0; s < charset; s++)
        if (!skip[s] and used[s])
            if (c++ == idx)
                return s;
    return charset-1;
}
