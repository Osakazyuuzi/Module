////////////////////////////////////////////////////////////////////////////////
// ファイル名: EntityIDGenerator.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EntityIDGenerator.h"

std::atomic<std::size_t> EntityIDGenerator::m_entityIdCounter = {};