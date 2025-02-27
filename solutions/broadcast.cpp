/**
 * @file broadcast.cpp
 * @author Aditya Gupta (Github: AdityaGupta03)
 * @brief Solution to the Fly.io Broadcast Challenge using my Maelstrom C++ library
 * @date 2024-02
 *
 * References:
 *  - Fly.io Distributed Systems Challenge (https://fly.io/dist-sys/3/)
 */

#include "whirlpool.hpp"

#include <assert.h>

using namespace whirlpool;

int main() {
    // Create node with init handler already provided
    Node node;

    // Register broadcast handler
    node.registerHandler<json>("broadcast", [&node](const Message<json>& msg) -> void {
        assert(false && "Broadcast handler not implemented");
    });

    // Register read handler
    node.registerHandler<json>("read", [&node](const Message<json>& msg) -> void {
        assert(false && "Read handler not implemented");
    });

    // Register topology handler
    node.registerHandler<json>("topology", [&node](const Message<json>& msg) -> void {
        assert(false && "Topology handler not implemented");
    });

    // Start processing loop
    node.run();
    return 0;
}
