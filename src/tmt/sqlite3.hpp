/** 
 * (C) 2013 David Moreno & Coralbits SL. <info@coralbits.com>
 * 
 * https://github.com/davidmoreno/tomato
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include "database.hpp"

typedef struct sqlite3 sqlite3;

namespace tmt{
	class SQLite3 : public Database{
	public:
		sqlite3 *db;
		
		SQLite3(const std::string &init);
		~SQLite3();

		ResultSet *resultset(const std::string &query);
		int query(const std::string &query, const tmt::fields_and_values &values);
	};
};
