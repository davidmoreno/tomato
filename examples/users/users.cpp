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

#include <tmt/tmt.hpp>
#include <iostream>
#include <stdlib.h>

#include "users.hpp"

using namespace std;

tmt::QuerySet<User> User::objects("users");

int main(int argc, char **argv){
	tmt::init("users.sqlite");
	atexit(tmt::free);
	
	if (argc<2){
		cout<<"users <syncdb|list|add|del|edit>\n";
		return 1;
	}
	std::string command{argv[1]};
	if (command=="syncdb"){
		User::create_table<User>();
	}
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
			return -1;
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
	tmt::free();
}
