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

typedef struct sqlite3;

namespace tmt{
	class SQLite : public Database{
	private:
		sqlite3 *db;
	public:
		SQLite(const std::string &init);
		~SQLite();
		
		int insert(const std::string &table, const fields_and_values &values);
		void save(const std::string &table, const fields_and_values &values);
		void update(const std::string &table, int id, const fields_and_values &values);
		void del(const std::string &table, int id);
		
		int query(const std::string &query, const tmt::fields_and_values &values);
	};
};
