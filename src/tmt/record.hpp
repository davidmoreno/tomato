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

#include <exception>
#include <string>
#include <vector>
#include <tuple>
#include "database.hpp"

namespace tmt{
	class Record{
	public:
		int id=0;

		// Must reimplement
		virtual fields_and_refs field_map_ref()=0;
		virtual const char *table_name()=0;

		
		class not_implemented : public std::exception{};
		virtual void set(
				int n,
				const std::string &a
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b, const std::string &c
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b, const std::string &c,
				const std::string &d
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b, const std::string &c,
				const std::string &d, const std::string &e
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b, const std::string &c,
				const std::string &d, const std::string &e, const std::string &f
			){ throw not_implemented(); };
			
		virtual fields_and_values field_map_val(){
			std::vector<std::tuple<std::string, std::string &>> ref{field_map_ref()};
			fields_and_values ret;
			for(auto f: ref){
				ret.push_back({f.get<0>(), f.get<1>()});
			}
			return ret;
		};
		virtual void save(){
			if (id==0){
				tmt::Database::singleton()->insert(table_name(), field_map_val());
			}
			else{
				tmt::Database::singleton()->save("users", id, field_map_val());
			}
		};
		virtual void del(){
			tmt::Database::singleton()->del("users", id);
		}
		
	};
}
