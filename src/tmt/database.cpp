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

#include <sstream>

#include "database.hpp"
#include "model.hpp"
#include "tmt.hpp"

using namespace tmt;

Database *Database::_singleton=nullptr;

Database::Database()
{
	if (_singleton)
		throw(tmt::initialization_exception("Singleton already exists. Trying to initialize datbase twice."));
	_singleton=this;
}

Database::~Database(){
	_singleton=nullptr;
}


int Database::insert(const std::string& table, const fields_and_values& values){
	std::stringstream qi, qv;
	qi<<"INSERT INTO "<<table<<" (id,";
	int n=values.size();
	for (auto pair: values){
		n--;
		qi<<pair.first;
		qv<<'?';
		if (n>0){
			qi<<", ";
			qv<<", ";
		}
	}
	qi<<") VALUES (NULL, "<<qv.str()<<")";
	
	return query(qi.str(), values);
}


void Database::save(const std::string &table, int id, const tmt::fields_and_values &values){
	std::stringstream qi, qv;
	qi<<"UPDATE "<<table<<" SET ";
	int n=values.size();
	for (auto pair: values){
		n--;
		qi<<pair.first<<"="<<"?";
		qv<<'?';
		if (n>0){
			qi<<", ";
			qv<<", ";
		}
	}
	qi<<" WHERE id = "<<id;
	query(qi.str(), values);
}

void Database::del(const std::string &table, int id){
	std::stringstream qi;
	qi<<"DELETE FROM "<<table<<" WHERE id = "<<id;

	query(qi.str(), {});
}

void Database::create_table(const std::string& table_name, const std::vector<Field>& fieldstypes)
{
	std::stringstream qi;
	
	qi<<"CREATE TABLE "<<table_name<<"(id INTEGER PRIMARY KEY AUTOINCREMENT";
	for(auto &ft:fieldstypes){
		qi<<", "<<ft.name<<" "<<ft.sql_type;
	}
	for(auto &ft:fieldstypes){
		if (!ft.extra_at_create.empty())
			qi<<", "<<ft.extra_at_create;
	}
	qi<<");";
	
	query(qi.str(), {});
}
