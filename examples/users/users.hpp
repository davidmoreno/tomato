/** 
 * (C) 2013 Coralbits SL. <info@coralbits.com>
 * All rights reserved.
 * 
 * Under BSD 3 clause license.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *     1. Redistributions of source code must retain the above copyright notice, 
 *        this list of conditions and the following disclaimer.
 *     
 *     2. Redistributions in binary form must reproduce the above copyright 
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 * 
 *     3. Neither the name of Django nor the names of its contributors may be used
 *        to endorse or promote products derived from this software without
 *        specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <ormcpp/record.hpp>
#include <ormcpp/queryset.hpp>

class User : public ORM::Record{
public:
	int id=0;
	std::string username;
	std::string password;

	bool operator==(const User &other) const{
		return id==other.id && username==other.username && password==other.password;
	}
	
	static ORM::QuerySet<User> objects;
	
	virtual void set(const std::string &id, const std::string &username, const std::string &password){
		this->id=atoi(id.c_str());
		this->username=username;
		this->password=password;
	};
	
	virtual void save(){
		if (id==0){
			ORM::insert("users", {{"name",username}, {{"password"},password}});
		}
		else{
			ORM::save("users", id, {{"name",username}, {{"password"},password}});
		}
	};
	virtual void del(){
	}
};

inline static std::ostream &operator<<(std::ostream &o, const User &u){
	o<<"id: <"<<u.id<<">, username <"<<u.username<<">, password <"<<u.password<<">";
	return o;
}
