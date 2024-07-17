#include "openai.hpp"

#include <iostream>

int main() {
    openai::start("sk-on-Jo5FAGExsug1vrAvXCQ", "", true, "https://agino.me/");

    auto chat_completion = openai::chat().create(R"(
    {
        "model": "gpt-3.5-turbo",
        "messages": [
            {"role": "user", "content": "Say this is a test"}
        ],
        "max_tokens": 7,
        "temperature": 0
    }
    )"_json);
    
    std::cout << "Response is:\n" << chat_completion.dump(2) << '\n'; 
}