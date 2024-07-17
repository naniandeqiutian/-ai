#include "openai.hpp"
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    // 启动 OpenAI 客户端
    openai::start("sk-on-Jo5FAGExsug1vrAvXCQ", "", true, "https://agino.me/");

    bool continueConversation = true;

    while (continueConversation) {
        std::string input;
        std::cout << "你: ";
        std::getline(std::cin, input); // 使用 getline 读取整行

        if (input == "exit") { // 如果用户输入 'exit'，则退出循环
            continueConversation = false;
            continue;
        }

        // 创建 JSON 对象
        json request = {
            {"model", "gpt-3.5-turbo"},
            {"messages", json::array({{
                {"role", "user"},
                {"content", "What do you think about " + input}
            }})},
            {"max_tokens", 4096},
            {"temperature", 0}
        };

        // 调用 OpenAI API 进行聊天
        auto completion = openai::chat().create(request);

        // 检查 API 响应并输出聊天结果
        if (completion.contains("choices") && !completion["choices"].empty()) {
            std::string ai_response = completion["choices"][0]["message"]["content"].get<std::string>();
            std::cout << "AI: " << ai_response << std::endl;
        } else {
            std::cout << "Error: No valid response from OpenAI API" << std::endl;
        }
    }

    return 0;
}