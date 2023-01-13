#pragma once
#include <thread>
#include <iostream>
#include "tableManager.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <chrono>
#include <mutex>
#include <map>
#include <regex>
using json = nlohmann::json;

void audit_init_table_config(json config);
void audit_has_one_conditions(json rule);
void audit_column_type(json rule);
