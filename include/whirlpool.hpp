/**
 * @file whirlpool.hpp
 * @brief A C++ library for working with Maelstrom distributed systems testing framework
 * @author Aditya Gupta
 * @copyright 2024 MIT License
 *
 * Core functionality for implementing distributed system nodes compatible with
 * the Maelstrom testing framework. See README.md for detailed documentation.
 * Provides the node and message classes, as well as utility functions for working
 * with JSON.
 */

#ifndef WHIRLPOOL_WHIRLPOOL_HPP_
#define WHIRLPOOL_WHIRLPOOL_HPP_

#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_set>

namespace whirlpool {

using json = nlohmann::json;

template<typename BodyType = json>
struct Message {
  std::string src;
  std::string dest;
  BodyType body;
};

template<typename T>
Message<T> jsonToMsg(const json& raw_json) {
    return Message<T>{
        .src=raw_json["src"].get<std::string>(),
        .dest=raw_json["dest"].get<std::string>(),
        .body=raw_json["body"].get<T>()
    };
}

template<typename T>
json msgToJson(const Message<T>& msg) {
    json raw_json = {
        {"src", msg.src},
        {"dest", msg.dest},
        {"body", msg.body}
    };

    return raw_json;
}

struct InitBody {
    std::string type{"init"};
    uint64_t msg_id;
    std::string node_id;
    std::unordered_set<std::string> node_ids;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitBody, type, msg_id, node_id, node_ids);
};

struct InitOkBody {
    std::string type{"init_ok"};
    uint64_t msg_id;
    uint64_t in_reply_to;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(InitOkBody, type, msg_id, in_reply_to);
};

struct EchoBody {
    std::string type{"echo"};
    uint64_t msg_id;
    std::string echo;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EchoBody, type, msg_id, echo);
};

struct EchoOkBody {
    std::string type{"echo_ok"};
    uint64_t msg_id;
    uint64_t in_reply_to;
    std::string echo;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EchoOkBody, type, msg_id, in_reply_to, echo);
};

struct GenerateBody {
    std::string type{"generate_ok"};
    uint64_t msg_id;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GenerateBody, type, msg_id);
};

struct GenerateOkBody {
    std::string type{"generate_ok"};
    uint64_t msg_id;
    uint64_t in_reply_to;
    std::string id;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GenerateOkBody, type, msg_id, in_reply_to, id);
};

class Node {
public:
    Node() : next_msg_id_(1), running_(false) {
        // Register init function
        registerHandler<InitBody>("init", [this](const Message<InitBody>& msg) {
            id_ = msg.body.node_id;

            InitOkBody res_body = {
                "init_ok",
                reserveMsgID(),
                msg.body.msg_id,
            };

            Message<InitOkBody> res = {
                .src = id_,
                .dest = msg.src,
                .body = res_body
            };

            writeToStdout(res);
        });
    }

    template<typename T>
    void registerHandler(const std::string& msg_type, std::function<void(const Message<T>&)> handler) {
        handlers_[msg_type] = [handler](const json& raw_json) {
            Message<T> msg = jsonToMsg<T>(raw_json);
            handler(msg);
        };
    }

    template<typename T>
    void writeToStdout(const Message<T>& response) {
        std::cout << msgToJson(response).dump() << std::endl;
    }

    void run() {
        running_ = true;

        std::string raw_msg;
        while (running_ && std::getline(std::cin, raw_msg)) {
            try {
                json raw_json = json::parse(raw_msg);
                handleMessage(raw_json);
            } catch (const std::exception& e) {
                std::cerr << "Failed to parse message: " << e.what() << std::endl;
            }
        }
    }

    std::string getID() {
        return id_;
    }

    uint64_t reserveMsgID() {
        return next_msg_id_++;
    }

private:
    std::string id_;
    bool running_;
    uint64_t next_msg_id_;
    std::unordered_map<std::string, std::function<void(const json&)> > handlers_;

    void handleMessage(json& raw_json) {
        if (!raw_json.contains("body") || !raw_json["body"].contains("type")) {
            std::cerr << "Invalid message format: missing 'type'" << std::endl;
            std::cerr << "Message: " << raw_json.dump() << std::endl;
            return;
        }

        auto it = handlers_.find(raw_json["body"]["type"]);
        if (it != handlers_.end()) {
            it->second(raw_json);
        } else {
            std::cerr << "Unhandled message type: " << raw_json["type"] << std::endl;
            std::cerr << "Message: " << raw_json.dump() << std::endl;
        }
    }
};

} // namespace whirlpool

#endif // WHIRLPOOL_WHIRLPOOL_HPP_
