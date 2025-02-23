/**
 * @file maelstrom.hpp
 * @author Aditya Gupta (Github: AdityaGupta03)
 * @brief A C++ library/impl for working with Maelstrom
 * @date 2024-02
 *
 * @details This software provides a high-level interface for implementing
 * distributed system nodes that can be tested using the Maelstrom framework. It
 * handles message serialization, node communication, and error handling,
 * allowing developers to focus on implementing distributed algorithms.
 *
 * Note: This is an independent implementation of a client library for use with
 * Maelstrom (https://github.com/jepsen-io/maelstrom). Maelstrom itself is
 * licensed under the Eclipse Public License and provides its own C++
 * interface implementation. This library is an alternative implementation and
 * is not affiliated with or endorsed by the Maelstrom project.
 *
 * Dependencies:
 *  - nlohmann/json (https://github.com/nlohmann/json)
 *
 * References/Related:
 *  - Maelstrom (https://github.com/jepsen-io/maelstrom)
 *  - Maelstrom C++ Impl (https://github.com/jepsen-io/maelstrom/tree/main/demo/c%2B%2B)
 *  - Fly.io Distributed Systems Challenge (https://fly.io/dist-sys)
 *
 * @copyright Copyright (c) 2024 Aditya Gupta
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CPP_WHIRLPOOL_HPP
#define CPP_WHIRLPOOL_HPP

#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <string>
#include <unordered_map>

namespace whirlpool {

using json = nlohmann::json;

struct Message {
  std::string src;
  std::string dest;
  nlohmann::json body;

  std::string type() const {
    return body.contains("type") ? body["type"].get<std::string>() : "";
  }
};

inline Message json_to_msg(const json &raw_json) {
    return Message{
        .src=raw_json["src"].get<std::string>(),
        .dest=raw_json["dest"].get<std::string>(),
        .body=raw_json["body"]
    };
}

using HandlerFunc = std::function<void(const Message &)>;

class Node {
public:
    Node()
        : next_msg_id_(1), running_(false) {
            // Register init function
            register_handler("init", [this](const Message &msg) {
                json req_body = msg.body;
                id_ = req_body["node_id"].get<std::string>();

                json response;
                response["type"] = "init_ok";

                reply(msg, response);
            });
    }

    void reply(const Message& msg, json& res) {
        if (!res.contains("msg_id")) {
            res["msg_id"] = next_msg_id_++;
        }

        if (!res.contains("in_reply_to")) {
            res["in_reply_to"] = msg.body["msg_id"];
        }

        json response = {
            {"src", id_},
            {"dest", msg.src},
            {"body", res}
        };

        std::cout << response.dump() << std::endl;
    }

    void run() {
        running_ = true;

        std::string raw_msg;
        while (running_ && std::getline(std::cin, raw_msg)) {
            json raw_json = json::parse(raw_msg);
            handle_message(raw_json);
        }
    }

    void register_handler(const std::string& msg_type, HandlerFunc handler) {
        handlers_[msg_type] = handler;
    }

    std::string get_id() {
        return id_;
    }

private:
    std::string id_;
    bool running_;
    uint64_t next_msg_id_;
    std::unordered_map<std::string, HandlerFunc> handlers_;

    void handle_message(json& raw_json) {
        Message msg = json_to_msg(raw_json);
        auto it = handlers_.find(msg.type());
        if (it != handlers_.end()) {
            it->second(msg);
        } else {
            std::cerr << "Unhandled message type: " << msg.type() << std::endl;
            std::cerr << "Message: " << raw_json.dump() << std::endl;
        }
    }
};

} // namespace whirlpool

#endif // CPP_WHIRLPOOL_HPP
