/**
 * @file echo.cpp
 * @author Aditya Gupta (Github: AdityaGupta03)
 * @brief Solution to the Fly.io Echo Challenge using my Maelstrom C++ library
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
    node.registerHandler<EchoBody>("echo", [&node](const Message<EchoBody>& msg) -> void {
        EchoOkBody body;
        body.msg_id = node.reserveMsgID();
        body.in_reply_to = msg.body.msg_id;
        body.echo = msg.body.echo;

        Message<EchoOkBody> res;
        res.src = node.getID();
        res.dest = msg.src;
        res.body = body;

        node.writeToStdout(res);
    });

    // Start processing loop
    node.run();
    return 0;
}
