#pragma once
#include <algorithm>
#include <iostream>

class unidirected_edge
{
private:
    int m_from;
    int m_to;
public:
    unidirected_edge(int a, int b)
    {
        if (a <= b) {
            m_from = a;
            m_to = b;
        } else {
            m_from = b;
            m_to = a;
        }
    }
    int from() const
    {
        return m_from;
    }
    int to() const
    {
        return m_to;
    }
    bool operator<(const unidirected_edge& other) const
    {
        if (m_from < other.m_from) {
            return true;
        }
        if (m_from > other.m_from) {
            return false;
        }
        return m_to < other.m_to;
    }
    bool operator==(const unidirected_edge& other) const
    {
        return m_from == other.m_from && m_to == other.m_to;
    }
};
inline std::ostream& operator<<(std::ostream& os, const unidirected_edge& edge)
{
    os << "(" << edge.from() << ", " << edge.to() << ")";
    return os;
}