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
#include "fields.hpp"

namespace tmt{
	class Model{
	public:
		class Meta{
		public:
			const char *model_name;
			const char *table_name;
			
			std::vector<Field> fields;
			
			Meta(const char *model_name, const char *table_name, const std::vector<Field> &fields) : model_name(model_name), table_name(table_name), fields(fields){}
		};
		int id=0;

		const char *table_name(){ return meta()->table_name; }
		// Must reimplement
		virtual fields_and_refs field_refs()=0;
		virtual Meta *meta()=0;
		
		virtual fields_and_values field_vals(){
			auto ref=field_refs();
			fields_and_values ret;
			for(auto f: ref){
				ret.push_back({f.first, f.second});
			}
			return ret;
		};
		virtual void save(){
			auto fv=field_vals();
			if (id==0){
				id=tmt::Database::singleton()->insert(table_name(), fv);
			}
			else{
				tmt::Database::singleton()->save("users", id, fv);
			}
		};
		virtual void del(){
			tmt::Database::singleton()->del("users", id);
		}
		
		template<typename T>
		static void create_table(){
			tmt::Database::singleton()->create_table(T::_meta->table_name, T::_meta->fields);
		}
		
	};
	
}
