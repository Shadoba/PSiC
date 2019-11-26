#pragma once

#include <vector>
#include <string>
#include <map>

enum httpRequestMethod
{
    CONNECT = 0u,
    OTHER,
    INVALID = 255u
};

enum protocol
{
    HTTP = 0u,
    INVALID = 255u
};

class Utility
{
public:
    static std::vector<std::string> splitString(std::string input, char delimiter);
    static std::vector<std::string> splitString(std::string input, std::string delimiter);
    static httpRequestMethod getMethodByString(std::string input);
    static protocol getProtocolByString(std::string input);
    
    template<
            class Key,
            class T,
            class Compare = std::less<Key>,
            class Allocator = std::allocator<std::pair<const Key, T> >
        >
    class ConstableMap : public std::map<Key, T, Compare, Allocator>
    {
    public:
        /**
         * @brief Construct a new Constable Map object
         * 
         * @param init initializer list
         * @param comp comparator delagete
         * @param alloc allocator delgate
         */
        ConstableMap( std::initializer_list<std::pair<const Key, T>> init,
            const Compare& comp = Compare(),
            const Allocator& alloc = Allocator() ) : std::map<Key, T, Compare, Allocator>(init, comp, alloc) 
            {}

        /**
         * @brief overloaded subscript operator, can return const value
         * 
         * @param key key
         * @return const T& return value
         */
        const T& operator[]( const Key& key ) const
        {
            auto retVal = this->find(key);
            //SERVER_ASSERT_MSG(this->end() == retVal, "Element non existent")
            return (this->end() != retVal) ? (retVal->second) : ((T) 255u);
        }
    };
    static const ConstableMap<std::string, httpRequestMethod> httpRequestMethodMap;         //<? Contains supported request methods
    static const ConstableMap<std::string, protocol> protocolMap;                           //<? Contains supported protocols

    Utility() = delete;
};
