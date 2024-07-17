#include "openai.hpp"
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <exception>
#include <vector>
#include <fstream>
#include <ctime>

using json = nlohmann::json;

// 日志记录函数
void log(const std::string& message) {
    std::time_t now = std::time(nullptr);
    std::tm timeinfo;

    // 使用 localtime 或 gmtime 根据你的需求
    // localtime 用于本地时间，gmtime 用于 UTC 时间
    #ifdef _WIN32
        localtime_s(&timeinfo, &now); // Windows-specific localtime_s for thread safety
    #else
        timeinfo = *std::localtime(&now); // Linux and other POSIX systems
    #endif

    char timeStr[30];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::ofstream logFile("app.log", std::ios_base::app); // 追加模式打开日志文件
    logFile << "[" << timeStr << "] " << message << "\n";
    logFile.close();
}

// 用户输入验证函数
bool validateInput(const std::string& input) {
    // 简单地检查输入是否非空
    return !input.empty();
}

// 处理API请求的函数，返回AI的响应
std::string handle_api_request(json& request) {
    try {
        auto completion = openai::chat().create(request);
        if (completion.contains("choices") && !completion["choices"].empty()) {
            std::string ai_response = completion["choices"][0]["message"]["content"].get<std::string>();
            return ai_response;
        } else {
            std::cout << "Error: No valid response from OpenAI API" << std::endl;
            return "";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error handling API request: " << e.what() << std::endl;
        return "";
    }
}

int main() {
    // 启动 OpenAI 客户端
    openai::start("sk-on-Jo5FAGExsug1vrAvXCQ", "", true, "https://agino.me/");

    bool continueConversation = true;
    std::vector<json> conversationHistory = {
        // 添加一个系统消息，指示模型使用中文
        {{"role", "system"}, {"content", u8"你是一个能够说流利中文的助手。请使用中文回复。"}}
    };

    while (continueConversation) {
        std::string input;
        std::cout << "你: ";
        std::getline(std::cin, input);

        if (input == "exit") {
            continueConversation = false;
            continue;
        }

        if (!validateInput(input)) {
            std::cout << "无效输入，请重新输入。\n";
            continue;
        }

        // 构建 JSON 对象，并添加对话历史
        json request = {
            {"model", "gpt-3.5-turbo"},
            {"max_tokens", 200},
            {"temperature", 0.5},
            {"messages", conversationHistory}
        };
        request["messages"].push_back({{"role", "user"}, {"content", input}});

        std::string ai_response = handle_api_request(request);

        if (!ai_response.empty()) {
            std::cout << "AI: " << ai_response << std::endl;
            // 更新对话历史
            conversationHistory.push_back({{"role", "assistant"}, {"content", ai_response}});
            log("User input: " + input);
            log("AI response: " + ai_response);
        }
    }

    return 0;
}