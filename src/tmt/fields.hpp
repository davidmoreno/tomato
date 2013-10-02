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

namespace tmt{
	class Field{
	public:
		const char *name;
		std::string sql_type;
		std::string extra_at_create;
		Field(const char *name, const char *sql_type) : name(name), sql_type(sql_type){}
	};
	class Text : public Field{
	public:
		Text(const char *name) : Field(name,"TEXT"){};
	};
	class Double : public Field{
		Double(const char *name) : Field(name,"DOUBLE"){};
	};
	class ForeignKey : public Field{
	public:
		const char *related_table;
		ForeignKey(const char *name, const char *related_table) : Field(name,"INTEGER"), related_table(related_table){
			extra_at_create=std::string("FOREIGN KEY(")+name+std::string(") REFERENCES ")+related_table+std::string("(id)");
		}
	};
};
