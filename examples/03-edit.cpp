#include "openai.hpp"

#include <iostream>

int main() {
    openai::start("sk-on-Jo5FAGExsug1vrAvXCQ", "", true, "https://agino.me/");

    auto edit = openai::edit().create(R"(
    {
        "model": "text-davinci-edit-001",
        "input": "What day of the wek is it?",
        "instruction": "Fix the spelling mistakes"
    }
    )"_json); 
    std::cout << "Response is:\n" << edit.dump(2) << '\n'; 
}
