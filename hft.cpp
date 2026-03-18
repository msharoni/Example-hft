#include <atomic>

// Shared slot between feed (producer) and strategy (consumer)
// Kept on its own cache line to avoid false sharing
struct alignas(64) MarketUpdate {
    std::atomic<int> seq{0};   // incremented when a new update is ready
    int price{0};              // latest trade price
};

// Strategy state (lives hot in cache, updated in place)
struct alignas(64) StrategyState {
    int max_price = 0;
    int trades_seen = 0;
};

class Strategy {
private:
    StrategyState state; // allocated once, reused

public:
    // Process the next expected market update
    void handle_update(MarketUpdate& feed, int next_seq) {

        // Wait for the producer to publish the next update
        while (feed.seq.load(std::memory_order_acquire) != next_seq) {
            // tight spin: lowest possible latency
        }

        int price = feed.price;

        // Maintain running maximum
        if (price > state.max_price) {
            state.max_price = price;
        }

        // Track how many trades we've processed
        state.trades_seen++;
    }
};
