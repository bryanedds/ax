#include <sstream>

#include "../../impl/ax/string.hpp"

namespace ax
{
    std::vector<std::string> split_string(const std::string& str, char delimiter)
    {
        std::vector<std::string> strs{};
        std::stringstream ss(str);
        std::string token{};
        while (std::getline(ss, token, delimiter)) strs.push_back(token);
        return strs;
    }

    std::string join_strings(const std::vector<std::string>& strs, char delimiter)
    {
        std::string str_joined{};
        val& begin = std::begin(strs);
        val& end = std::end(strs);
        for (var iter = begin; iter != end; ++iter)
        {
            if (iter != begin) str_joined.push_back(delimiter);
            str_joined.append(*iter);
        }
        return str_joined;
    }

    std::string to_string(int32_t value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    std::string to_string(int64_t value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    std::string to_string(float value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    std::string to_string(double value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    std::string to_string(std::size_t value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
}
