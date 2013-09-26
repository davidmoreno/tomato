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
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>

#include "tmt.hpp"

namespace tmt{
	void sqlite3_init(const std::string &dbname);
	void sqlite3_delete();
	void sqlite3_insert(const std::string &table, const fields_and_values &values);
	void sqlite3_update(const std::string &table, int id, const fields_and_values &values);
	void sqlite3_delete(const std::string &table, int id);
}

using namespace tmt;



void tmt::init(const std::string &dbname)
{
	sqlite3_init(dbname);
}

void tmt::free()
{
	sqlite3_delete();
}

void tmt::insert(const std::string &table, const fields_and_values &values){
	sqlite3_insert(table, values);
}
void tmt::save(const std::string &table, int id, const fields_and_values &values){
	sqlite3_update(table, id, values);
}

void tmt::del(const std::string &table, int id){
	sqlite3_delete(table, id);
}
