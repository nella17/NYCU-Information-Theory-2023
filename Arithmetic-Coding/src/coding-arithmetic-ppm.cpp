#include "coding-arithmetic-ppm.hpp"

#include "arithmetic-code.hpp"
#include "options.hpp"
#include <iostream>
#include <iomanip>
#define _ <<' '<<

constexpr size_t STEP = 1000;

namespace coding {

ArithmeticPPM::ArithmeticPPM():
    Base(),
    bits((uint16_t)opts.bits),
    charset((uint16_t)(1 << bits)),
    order(opts.order), root(nullptr)
{
    assert(charset+1 <= Context::MAX_CHARSET);
    if (order >= 0)
        root = new Context(nullptr, order, charset+1);
}

ArithmeticPPM::~ArithmeticPPM() {}

Context* ArithmeticPPM::find(const History& history) {
    if (order < 0) return nullptr;
    auto ctx = root;
    for (auto s: history) {
        if (!ctx->has(s)) {
            return ctx;
        } else {
            ctx = ctx->get(s);
        }
    }
    return ctx;
}

void ArithmeticPPM::update(const History& history, uint16_t symbol) {
    if (order < 0) return;
    root->inc(symbol);
    auto ctx = root;
    for (auto s: history) {
        ctx = ctx->get(s);
        ctx->inc(symbol);
    }
}

size_t ArithmeticPPM::encode(DataSrc& src, DataDst& dst) {
    auto origsize = src.total();
    auto size = origsize * 8 / bits;

    Data data;
    Arithmetic code;
    History history{};
    timer_start_progress("compress file");
        src.reset();
        for (size_t i = 0; i <= size; i++) {
            uint16_t symbol = i == size ? charset : (uint16_t)src.readint(bits);

            // std::cerr << "round" _ i _ symbol _ std::endl;
            // std::cerr _ "history" _ history.size() _ ':';
            // for (auto x: history) std::cerr _ char(x);
            // std::cerr _ std::endl;

            auto ctx = find(history);
            Context::Bits skip(0);


            uint32_t cL, cR, total;

            while (ctx != nullptr) {
                if (ctx->has(symbol)) break;
                std::tie(cL, cR, total) = ctx->range(charset, skip);
                if (total) {
                    // std::cerr << "ctx\n----\n" << *ctx << "----\n";
                    auto bs = code.send(cL, cR, total);
                    data.write(bs);
                }
                skip |= ctx->used;
                ctx = ctx->parent;
            }

            if (ctx == nullptr) {
                // std::cerr << "ctx: -1\n";
                cL = symbol;
                cR = symbol+1;
                total = charset;
            } else {
                // std::cerr << "ctx\n----\n" << *ctx << "----\n";
                std::tie(cL, cR, total) = ctx->range(symbol, skip);
            }

            auto bs = code.send(cL, cR, total);
            data.write(bs);

            update(history, symbol);

            history.emplace_front(symbol);
            if ((int)history.size() > order)
                history.pop_back();

            if (i % STEP == 0)
                timer_progress((double)i / (double)size);
        }
    timer_stop_progress();

    timer_start("write file");
        dst.write(data);
        dst.write(true);
    timer_stop();

    auto compsize = (data.size() + 7) / 8;
    auto total_compsize = compsize;
    if (opts.verbose) {
        std::cerr
            << "Original size:      " << origsize << " bytes\n"
            << "Compressed size:    " << compsize << " bytes (" << data.size() << " bits)\n"
            << "Compression ratio:  "
                << std::setw(5) << std::setprecision(2) << std::fixed
                << 100 * (double)((int64_t)origsize - (int64_t)compsize) / (double)origsize << "%\n"
            << std::flush;
    }

    return total_compsize;
}

size_t ArithmeticPPM::decode(DataSrc& src, DataDst& dst) {
    // TODO
}

}

