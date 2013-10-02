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
#include <vector>

namespace tmt{
	using fields_and_values = std::vector<std::pair<std::string, std::string>>;
	using fields_and_refs = std::vector<std::pair<std::string, std::string&>>;
	using fields_types = std::vector<std::pair<std::string, std::string>>;
	
	class ResultSet;
	class Field;
	
	class Database{
	private:
		static Database *_singleton;
	public:
		Database();
		virtual ~Database();
		
		virtual int insert(const std::string &table, const fields_and_values &values);
		virtual void save(const std::string &table, int id, const fields_and_values &values);
		virtual void del(const std::string &table, int id);
		virtual ResultSet *resultset(const std::string &query)=0;
		virtual int query(const std::string &query, const tmt::fields_and_values &values)=0;
		virtual void create_table(const std::string &table_name, const std::vector<Field> &fieldstypes);
		
		static Database *singleton(){ return _singleton; }
	};
};
