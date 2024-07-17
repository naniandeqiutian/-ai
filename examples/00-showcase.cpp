#include "openai.hpp"
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <exception>
#include <vector>

using json = nlohmann::json;

void handle_api_request(json& request) {
    try {
        auto completion = openai::chat().create(request);
        if (completion.contains("choices") && !completion["choices"].empty()) {
            std::string ai_response = completion["choices"][0]["message"]["content"].get<std::string>();
            std::cout << "AI: " << ai_response << std::endl;
        } else {
            std::cout << "Error: No valid response from OpenAI API" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    // 启动 OpenAI 客户端
    openai::start("sk-on-Jo5FAGExsug1vrAvXCQ", "", true, "https://agino.me/");

    bool continueConversation = true;
    std::vector<json> conversationHistory;

    while (continueConversation) {
        std::string input;
        std::cout << "你: ";
        std::getline(std::cin, input);

        if (input == "exit") {
            continueConversation = false;
            continue;
        }

        // 构建 JSON 对象，并添加对话历史
        json request = {
            {"model", "gpt-3.5-turbo"},
            {"max_tokens", 100}, // 减少 max_tokens 避免过长的回答
            {"temperature", 0},
            {"messages", conversationHistory}
        };
        request["messages"].push_back({{"role", "user"}, {"content", input}});

        handle_api_request(request);

        // 更新对话历史
        json userMessage = {{"role", "user"}, {"content", input}};
        conversationHistory.push_back(userMessage);
        // 假设AI的响应已经在 handle_api_request 中打印，这里不再重复添加到历史中
    }

    return 0;
}