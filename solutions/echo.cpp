/**
 * @file main.cpp
 * @author Aditya Gupta (Github: AdityaGupta03)
 * @brief Solution to the Fly.io Echo Challenge using the Maelstrom C++ library
 * @date 2024-02
 *
 * References:
 *  - Fly.io Distributed Systems Challenge (https://fly.io/dist-sys/1/)
 */

#include "whirlpool.hpp"

using namespace whirlpool;

int main() {
    // Create node with init handler already provided
    Node node;

    // Register echo handler
    node.register_handler("echo", [&node](const Message& msg) -> void {
        json response;
        response["type"] = "echo_ok";
        response["echo"] = msg.body["echo"];
        response["in_reply_to"] = msg.body["msg_id"];

        node.reply(msg, response);
    });

    // Start processing loop
    node.run();
    return 0;
}
