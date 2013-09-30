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
#include <memory>
#include <iterator>
#include <vector>

namespace tmt{
	void init(const std::string &dbname);
	void free();
	
	class exception : public std::exception{
		std::string str;
	public:
		exception(const std::string &s) : str(s){}
		const char *what() const throw(){ return str.c_str(); }
	};

#define ADD_EXCEPTION(NAME, STR) \
	class NAME : public exception{ \
	public: \
		NAME(const std::string& s) : exception(STR+s) {}; \
	};
	
	ADD_EXCEPTION(invalid_query, "invalid query: ");
	ADD_EXCEPTION(initialization_exception, "initialization exception: ");
	ADD_EXCEPTION(does_not_exist, "does_not_exist: ");
	ADD_EXCEPTION(not_unique, "more than one records with that requirements: ");
	ADD_EXCEPTION(invalid_count_of_columns, "Invalid count of columns reading from database. Check schema. ");
};
