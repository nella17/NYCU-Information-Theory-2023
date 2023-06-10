#include "context.hpp"

Context::Context(Context* p, int o, uint16_t ch):
    parent(p),
    order(o), charset(ch),
    size(0), used(0), count(charset),
    next{}
{}

void Context::inc(uint16_t symbol) {
    used[symbol] = 1;
    count[symbol]++;
    size++;
}

bool Context::has(uint16_t symbol) {
    return used[symbol];
}

Context::Range Context::range(uint16_t symbol, Bits skip) {
    uint32_t accum = 0, total = 0;
    for (uint16_t i = 0; i < symbol; i++)
        if (!skip[i])
            accum += count[i];
    for (uint16_t i = 0; i < charset; i++)
        if (!skip[i])
            total += count[i];
    bool zz = total > 0;
    auto cnt = symbol == charset-1 ? zz : count[symbol];
    return { accum, accum + cnt, total + zz };
}

Context* Context::get(uint16_t symbol) {
    if (order <= 0) return nullptr;
    if (next.empty())
        next.assign(charset, nullptr);
    auto ctx = next[symbol];
    if (ctx != nullptr) return ctx;
    return next[symbol] = new Context(this, order-1, charset);
}

#define _ <<' '<<
std::ostream& operator<<(std::ostream& os, const Context& ctx) {
    for (uint16_t s = 0; s < ctx.charset; s++)
        if (ctx.used[s])
            os _ s _ char(s) _ ctx.count[s] _ '\n';
    return os;
}
