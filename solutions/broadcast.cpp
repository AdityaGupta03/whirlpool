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

using namespace whirlpool;

int main() {
    // Create node with init handler already provided
    Node node;

    // Register broadcast handler
    node.register_handler("broadcast", [&node](const Message& msg) -> void {
        json response;
        response["type"] = "broadcast_ok";
        node.reply(msg, response);
    });

    // Register read handler
    node.register_handler("read", [&node](const Message& msg) -> void {
        json response;
        response["type"] = "read_ok";
        node.reply(msg, response);
    });

    // Register topology handler
        node.register_handler("topology", [&node](const Message& msg) -> void {
            json response;
            response["type"] = "topology_ok";
            node.reply(msg, response);
        });

    // Start processing loop
    node.run();
    return 0;
}
