/**
 * @file uuid.cpp
 * @author Aditya Gupta (Github: AdityaGupta03)
 * @brief Solution to the Fly.io Unique ID Generation Challenge using my Maelstrom C++ library
 * @date 2024-02
 *
 * References:
 *  - Fly.io Distributed Systems Challenge (https://fly.io/dist-sys/2/)
 */

#include "whirlpool.hpp"
#include <chrono>
#include <cstdint>

using namespace whirlpool;

static uint64_t sequence{0};

int main() {
    // Create node with init handler already provided
    Node node;

    // Register generate handler
    node.register_handler("generate", [&node](const Message& msg) -> void {
        json response;

        // Get current timestamp in milliseconds
        long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        // Get the node id of the form nX where X is a number
        // Cap the node id number to at most 8 bits
        std::string node_id = node.get_id().substr(1);
        uint8_t node_bits = std::stoul(node_id) & 0xFF;

        // Generate next sequence number capped at 8 bits
        uint8_t seq = sequence++ & 0xFF;

        // Generate uuid into 64 bit id
        // Format: [timestamp(48 bit)][node_id(8 bits)][sequence(8 bits)]
        uint64_t uuid = (timestamp << 16) | (node_bits << 8) | seq;

        response["type"] = "generate_ok";
        response["id"] = uuid;

        node.reply(msg, response);
    });

    // Start processing loop
    node.run();
    return 0;
}
