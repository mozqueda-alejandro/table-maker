// Alejandro Mozqueda
// Requires C++20

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
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

class FlowerArrangement {
public:
    int solve(int range, std::vector<int> &locations, std::vector<int> &values) {
        if (!isValidVenue(range, locations, values)) {
            return -1;
        }

        memo = {};
        this->locations = locations;
        this->values = values;

        return solveRecurse(locations.size() - 1);
    }

    // Saves key/values each to a one line, comma demarcated string
    std::string getData() {
        std::string memoString = "";
        std::vector<int> tableIds(getTableCount());
        std::iota(tableIds.begin(), tableIds.end(), 1); // Table Ids from 1 to n

        memoString += generateParsableString(Output::Table, tableIds);
        memoString += generateParsableString(Output::Location, locations);
        memoString += generateParsableString(Output::Value, values);
        memoString += generateParsableString(Output::Choice, choices);
        memoString += generateParsableString(Output::Memo, memo);
        memoString += toString(Output::Max) + "," + std::to_string(memo[locations.size() - 1]) + '\n';

        return memoString;
    }

    int getTableCount() {
        return locations.size();
    }

private:
    std::unordered_map<int, int> memo, choices;
    std::vector<int> locations, values;

    /**
     * @brief Algorithm variation of LeetCode 198. House Robber
     *
     * Recursively solves the problem by placing flowers on the first table
     * or skipping the first table and placing on the second, using memoization
     * @param x - index of the table
     * @return the maximum value that can be achieved
     */
    int solveRecurse(int x) {
        if (x < 0) {
            return 0;
        }
        if (memo.contains(x)) {
            return memo[x];
        }

        int placeFirst = solveRecurse(getLeftIndexByOffset(x)) + values[x];
        int placeSecond = solveRecurse(x - 1);
        int result = std::max(placeFirst, placeSecond);
        memo[x] = result;
        choices[x] = placeFirst > placeSecond ? 1 : 0;

        return result;
    }

    // Iterates backwards from the currentIndex to find the table index
    // that is > 5 units away by subtraction of x-positions
    int getLeftIndexByOffset(int currentIndex, int offset = 5) {
        for (int i = currentIndex - 1; i >= 0; i--) {
            if (locations[currentIndex] - locations[i] > offset) {
                return i;
            }
        }
        return -1;
    }

    static bool isValidVenue(int range, std::vector<int> &locations, std::vector<int> &values) {
        if (locations.size() != values.size()) {
            return false;
        }
        auto [min, max] = std::minmax_element(locations.begin(), locations.end());
        if (*min < 0 || *max > range) {
            return false;
        }
        for (int i = 1; i < locations.size(); i++) {
            if (locations[i] < locations[i - 1]) {
                return false;
            }
        }
        for (int value: values) {
            if (value < 0) {
                return false;
            }
        }
        return true;
    }

    template<typename TAccessor>
    std::string generateParsableString(Output type, TAccessor& accessor) {
        std::string memoString = std::format("{},", toString(type));
        try {
            for (int i = 0; i < getTableCount(); i++) {
                memoString += std::to_string(accessor[i]) + ',';
            }
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return "";
        }
        memoString.back() = '\n';

        return memoString;
    }
};

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &vector) {
    std::string text = "";
    for (const std::string &element: vector) {
        text += element + ", ";
    }
    text = text.substr(0, text.size() - 2);
    os << text;
    return os;
}

void writeToOutput() {
    FlowerArrangement fa;
    int range = 30;
    std::vector nums = {8, 10, 15, 22, 26};
    std::vector values = {15, 5, 25, 15, 5};

    int maxValue = fa.solve(range, nums, values);
    std::cout << "Max value: " << maxValue << std::endl;

    static const std::string filePB = "../bonus_pb_out.txt";
    std::ofstream outPB(filePB, std::ios::out | std::ios::trunc);
    if (!outPB.is_open()) {
        std::cerr << "Cannot open file in \'" << filePB << "\'" << std::endl;
        exit(1);
    }

    outPB << fa.getData() << std::endl;
    outPB.close();
}

int main() {
    writeToOutput();
    return 0;
}
