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
#include <ormcpp/orm.hpp>
#include <iostream>

#include "users.hpp"

using namespace std;

ORM::QuerySet<User> User::objects("users");

int main(int argc, char **argv){
	ORM::init("users.sqlite");
	
	if (argc<2){
		cout<<"users <list|add|del|edit>\n";
		return 1;
	}
	std::string command{argv[1]};
	if (command=="list"){
		if (argc==3){
			auto u=User::objects.get("id",argv[2]);
			std::cout <<u<<std::endl;
		}
		else{
			for (auto u: User::objects.all()){
			std::cout <<u<<std::endl;
			}
		}
	}
	if (command=="add"){
		if (argc!=4){
			cout<<"users add <username> <password>"<<endl;
		}
		User u;
		u.username=argv[2];
		u.password=argv[3];
		cout<<"Saving "<<u<<endl;
		u.save();
		cout<<"Saved "<<u<<endl;
		
		u=User::objects.get("id", std::to_string(u.id));
		cout<<"From db "<<u<<endl;
	}
	if (command=="edit"){
		if (argc!=5){
			cout<<"users edit <id> <username> <password>"<<endl;
		}
		User u = User::objects.get("id",argv[2]);
		cout<<"Original "<<u<<endl;
		u.username=argv[3];
		u.password=argv[4];
		cout<<"Saving "<<u<<endl;
		u.save();
		cout<<"Saved "<<u<<endl;
		
		u=User::objects.get("id", std::to_string(u.id));
		cout<<"From db "<<u<<endl;
	}
	if (command=="del"){
		auto u=User::objects.get("id",argv[2]);
		cout<<"Delete "<<u<<endl;
		u.del();
	}
	ORM::free();
}
