// Alejandro Mozqueda
// Requires C++20

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>


enum class Output {
    Table,
    Location,
    Value,
    Choice,
    Memo,
    Max
};

std::unordered_map<Output, std::string> outputMappings = {
    {Output::Table, "table"},
    {Output::Location, "location"},
    {Output::Value, "value"},
    {Output::Choice, "choice"},
    {Output::Memo, "memo"},
    {Output::Max, "max"}
};

std::string toString(Output type) {
    return outputMappings[type];
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &vector) {
    std::string text = "";
    for (const std::string &element: vector) {
        text += element + ", ";
    }
    text = text.substr(0, text.size() - 2);
    os << text;
    return os;
}

void parse() {
    static const std::string filePC = "../bonus_pc_out.txt";
    static const std::string filePD = "../bonus_pd_out.txt";

    std::ifstream inPC(filePC);
    if (!inPC.is_open()) {
        std::cerr << "Cannot open file in \'" << filePC << "\'" << std::endl;
        exit(1);
    }

    std::unordered_map<std::string, std::vector<std::string>> data;
    while (!inPC.eof()) {
        std::string line;
        std::getline(inPC, line);

        // Tokenize line
        std::vector<std::string> tokens;
        std::string token;
        for (char c: line) {
            if (c == ',') {
                tokens.push_back(token);
                token.clear();
            } else {
                token += c;
            }
        }
        if (token.empty()) {
            continue;
        }
        tokens.push_back(token);

        // Set first token as the key
        std::string key = tokens[0];
        tokens.erase(tokens.begin());
        data[key] = tokens;
    }

    std::ofstream outPD(filePD, std::ios::out | std::ios::trunc);
    if (!outPD.is_open()) {
        std::cerr << "Cannot open file in \'" << filePD << "\'" << std::endl;
        exit(1);
    }

    // get indices of all numbers in the choices array that are 1
    std::vector<std::string> choices = data[toString(Output::Choice)];
    std::vector<std::string> choiceIndices;

    for (int i = 0; i < choices.size(); i++) {
        if (choices[i] == "1") {
            choiceIndices.push_back(std::to_string(i));
        }
    }

    outPD << "Table locations selected:           ";
    for (const auto index: choiceIndices) {
        outPD << data[toString(Output::Location)][std::stoi(index)] << ", ";
    }
    outPD << std::endl;

    outPD << "Values corresponding to the tables: ";
    for (const auto index: choiceIndices) {
        outPD << data[toString(Output::Value)][std::stoi(index)] << ", ";
    }
    outPD << std::endl;

    outPD.close();
}

int main() {
    parse();
    return 0;
}
