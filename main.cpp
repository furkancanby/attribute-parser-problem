/*
 * File: main.cpp
 * Author: Furkancan Bilal Yüce
 * Date: 11.02.2024
 * Description: This project offers a solution to 'Attribute Parser' problem in HackerRank.
 */

#include "logger.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <stack>
#include <vector>

Logger logger;

// Structure to represent a tag in the HRML document
struct Tag
{
    Tag* parent;
    std::map<std::string, Tag> childrens;
    std::map<std::string, std::string> attributes;

    std::optional<std::string> getAttribute(const std::vector<std::string>& tags,
                                            const std::string& attributeName) const
    {
        if (tags.empty())
        {
            auto it = attributes.find(attributeName);
            if (it != attributes.end())
            {
                return it->second;
            }
            return std::nullopt;  // Attribute not found
        }
        auto next_tag = tags.front();
        auto it = childrens.find(next_tag);
        if (it == childrens.end())
        {
            logger.warning("Tag could not found in childrens, tag: " + next_tag);
            return std::nullopt;
        }
        std::vector<std::string> childTags(tags.begin() + 1, tags.end());
        return childrens.at(next_tag).getAttribute(childTags, attributeName);
    }
};

// Class to handle HRML parsing and queries
class HRMLEngine
{
  public:
    // A map to store all the Tags
    std::map<std::string, Tag> tag_map;

    // A stack to store state of nested tags
    std::stack<std::string> tag_stack;

    // Function to parse HRML input and construct tag hierarchy
    void parseHRML(int line_count)
    {
        Tag* parentTag{nullptr};
        Tag* currentTag{nullptr};

        // Iterate over inputs from std::cin
        for (int i = 0; i < line_count; ++i)
        {
            std::string line;
            std::getline(std::cin, line);

            // Use regex to extract tag names from each line of HRML input.
            std::smatch match;
            if (std::regex_search(line, match, tag_pattern))
            {
                std::string tag_name = match.str(1);
                logger.info("Tag created: " + tag_name);
                if (tag_stack.empty())
                {
                    tag_map[tag_name] = Tag{};
                    tag_map[tag_name].parent = nullptr;
                    currentTag = &tag_map[tag_name];
                    // If there is no tags in stack, then parent tag should be
                    // current tag
                    parentTag = currentTag;
                }
                else
                {
                    auto parent_name = tag_stack.top();
                    parentTag->childrens[tag_name] = Tag{};
                    parentTag->childrens[tag_name].parent = parentTag;
                    currentTag = &parentTag->childrens[tag_name];
                }

                tag_stack.push(tag_name);

                // Use regex to extract attributes and values from each line of
                // HRML input.
                if (std::regex_search(line, match, attribute_pattern))
                {
                    std::smatch match;
                    std::string::const_iterator searchStart(line.cbegin());
                    while (std::regex_search(searchStart, line.cend(), match, attribute_pattern))
                    {
                        auto attribute_name = match[1];
                        auto attribute_value = match[2];
                        currentTag->attributes[attribute_name] = attribute_value;
                        searchStart = match.suffix().first;
                        logger.info("Attribute called " + attribute_name.str() +
                                    " is created with value of " + attribute_value.str());
                    }
                }
                else
                {
                    logger.warning("No attribute found for the tag called " + tag_name);
                }
                parentTag = currentTag;
            }
            else
            {
                if (tag_stack.empty())
                {
                    throw std::runtime_error("Closing tag found without opening tag");
                }
                std::smatch match;
                if (std::regex_search(line, match, tag_end_pattern))
                {
                    std::string tag_name = match.str(1);
                    logger.info("Tag closed: " + tag_name);
                    tag_stack.pop();
                    // If tag is closed then parentTag should be currentTag's
                    // parent(one level up)
                    parentTag = currentTag->parent;
                }
                else
                {
                    logger.warning("No tag found for the line called " + line);
                }
            }
        }
    }

    // Function to process queries and print attribute values
    void processQueries(int line_count)
    {
        for (int i = 0; i < line_count; ++i)
        {
            std::string line;
            std::getline(std::cin, line);

            std::vector<std::string> tokens = tokenize(line, '~');

            if (tokens.size() != 2)
            {
                throw std::runtime_error("Invalid input format: ");
            }
            std::vector<std::string> tags = tokenize(tokens[0], '.');
            std::string attribute = tokens[1];

            if (auto att = getAttribute(tags, attribute))
            {
                std::cout << *att << "\n";
            }
            else
            {
                std::cout << "Not Found!\n";
            }
        }
    }

  private:
    // Function to parse a string into tokens based on a delimiter
    std::vector<std::string> tokenize(const std::string& str, char delimiter)
    {
        using namespace std;
        vector<string> tokens;
        istringstream iss(str);
        string token;
        while (getline(iss, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::optional<std::string> getAttribute(const std::vector<std::string>& tags,
                                            const std::string& attributeName)
    {
        if (tags.empty())
        {
            return std::nullopt;  // No tags provided
        }
        auto& parentTag = tag_map[tags.front()];
        if (tags.size() == 1)
        {
            return parentTag.getAttribute({}, attributeName);
        }
        std::vector<std::string> childTags(tags.begin() + 1, tags.end());
        return parentTag.getAttribute(childTags, attributeName);
    }

  private:
    const std::regex tag_pattern{"<(\\w+)"};        // Check for "<tag..."
    const std::regex tag_end_pattern{"</(\\w+)>"};  // Check for "</tag..."
    const std::regex attribute_pattern{
        "(\\w+)\\s*=\\s*['\"]([^'\"]*)['\"]"};  // Check for attributes
};

void parseArguments(int argc, char* argv[])
{
    if (argc != 3)
    {
        Logger::currentLogLevel = LogLevel::OFF;
        return;
    }
    std::string opt = argv[1];

    if (opt == "-l")
    {
        std::string level = argv[2];

        if (level == "i")
        {
            Logger::currentLogLevel = LogLevel::INFO;
        }
        else if (level == "w")
        {
            Logger::currentLogLevel = LogLevel::WARNING;
        }
        else
        {
            Logger::currentLogLevel = LogLevel::OFF;
        }
    }
}

int main(int argc, char* argv[])
{
    using namespace std;

    parseArguments(argc, argv);

    int N{}, Q{};
    cin >> N >> Q;
    cin.ignore();

    HRMLEngine hrml;

    try
    {
        hrml.parseHRML(N);
        hrml.processQueries(Q);
    }
    catch (const exception& e)
    {
        cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
