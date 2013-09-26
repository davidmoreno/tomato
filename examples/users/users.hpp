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

#include <tmt/record.hpp>
#include <tmt/queryset.hpp>

class User : public tmt::Record{
public:
	int id=0;
	std::string username;
	std::string password;

	bool operator==(const User &other) const{
		return id==other.id && username==other.username && password==other.password;
	}
	
	static tmt::QuerySet<User> objects;
	
	virtual void set(const std::string &id, const std::string &username, const std::string &password){
		this->id=atoi(id.c_str());
		this->username=username;
		this->password=password;
	};
	
	virtual void save(){
		if (id==0){
			tmt::insert("users", {{"name",username}, {{"password"},password}});
		}
		else{
			tmt::save("users", id, {{"name",username}, {{"password"},password}});
		}
	};
	virtual void del(){
		tmt::del("users", id);
	}
};

inline static std::ostream &operator<<(std::ostream &o, const User &u){
	o<<"id: <"<<u.id<<">, username <"<<u.username<<">, password <"<<u.password<<">";
	return o;
}
