#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

struct Message {
  std::string src;
  std::string dest;
  json body;
};

uint64_t curr_msg_id = 1;

void handle_nop(const Message &input_msg) {
  return;
}

void handle_init(const Message &input_msg) {
  Message response_msg;

  // Swap source and destination for return message
  response_msg.src = input_msg.dest;
  response_msg.dest = input_msg.src;

  // Prepare the body
  response_msg.body["type"] = "init_ok";
  response_msg.body["in_reply_to"] = input_msg.body["msg_id"];
  response_msg.body["msg_id"] = curr_msg_id;
  curr_msg_id++;

  // Output response message to stdout
  json output {
    {"src", response_msg.src},
    {"dest", response_msg.dest},
    {"body", response_msg.body},
  };
  cout << output << endl;
}

void handle_echo(const Message &input_msg) {
  Message response_msg;

  // Swap source and destination for return message
  response_msg.src = input_msg.dest;
  response_msg.dest = input_msg.src;

  // Prepare the body of the response
  response_msg.body["type"] = "echo_ok";
  response_msg.body["in_reply_to"] = input_msg.body["msg_id"];
  response_msg.body["msg_id"] = curr_msg_id;
  curr_msg_id++;

  // Output response message
  json output {
    {"src", response_msg.src},
    {"dest", response_msg.dest},
    {"body", response_msg.body}
  }
  cout << output << endl;
}

static const unordered_map<string, function<void (const Message &)> > handlers = {
  {"init",  handle_init},
  {"echo",  handle_echo}
};

int main() {
  string line;
  while (getline(std::cin, line)) {
    try {
      // Parse the incoming JSON message
      json received_json = json::parse(line);

      // Deserialize into Message struct
      Message input_msg;
      input_msg.src = received_json["src"];
      input_msg.dest = received_json["dest"];
      input_msg.body = received_json["body"];

      // Get message type and call appropriate handler
      string msg_type = input_msg.body["type"];
      auto it = handlers.find(msg_type);
      if (it != handlers.end()) {
        it->second(input_msg);
      } else {
        cerr << "Unknown message type: " << input_msg.body["type"] << endl;
      }
    } catch (const std::exception &e) {
      cerr << "Error parsing or processing message: " << e.what() << endl;
    }
  }

  return 0;
}
