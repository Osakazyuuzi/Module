////////////////////////////////////////////////////////////////////////////////
// ファイル名: ComponentIDGenerator.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ComponentIDGenerator.h"

std::mutex ComponentIDGenerator::m_mutex;
std::unordered_map<std::type_index, std::size_t> ComponentIDGenerator::m_componentIDMap;