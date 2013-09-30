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
#include "queryset.hpp"
#include "resultset.hpp"
#include "database.hpp"

namespace tmt{
	class QuerySetBase::QuerySetPrivate{
		public:
			std::vector<std::string> filter;
			std::vector<std::string> exclude;
			std::string prepared;
			std::string table;
	};
}

using namespace tmt;

QuerySetBase::QuerySetBase(const QuerySetBase &b)
{
	p=new QuerySetBase::QuerySetPrivate;
	p->table=b.p->table;
	p->filter=b.p->filter;
	p->exclude=b.p->exclude;
}

QuerySetBase::QuerySetBase(const std::string& table)
{
	p=new QuerySetBase::QuerySetPrivate;
	p->table=table;
}

QuerySetBase::~QuerySetBase()
{
	delete p;
}
QuerySetBase& QuerySetBase::all(){
	return *this;
}

QuerySetBase& QuerySetBase::filter(const std::string& query, const std::string& value)
{
	p->filter.push_back(query+'='+value);
	return *this;
}

QuerySetBase& QuerySetBase::filter(const std::string& query, int value)
{
	std::stringstream ss;
	ss<<value;
	return filter(query, ss.str());
}

QuerySetBase& QuerySetBase::exclude(const std::string& query, const std::string& value)
{
	p->exclude.push_back(query+'='+value);
	return *this;
}

QuerySetBase& QuerySetBase::exclude(const std::string& query, int value)
{
	std::stringstream ss;
	ss<<value;
	return exclude(query, ss.str());
}

template <class T, class A>
T join(const A &begin, const A &end, const T &t)
{
  T result;
  A it = begin;
  if (it != end) 
   result.append(*it++);

  for( ; it!=end; ++it)
   result.append(t).append(*it);
  return result;
}



ResultSet *QuerySetBase::prepare()
{
	std::stringstream sql;
	sql<<"SELECT * FROM "<<(p->table);
	if (p->filter.size()>0 || p->exclude.size()>0){
		sql<<" WHERE ";
		sql<<join(p->filter.begin(), p->filter.end(), std::string(" AND "));
		if (p->exclude.size()>0)
			sql<<" NOT "<<join(p->filter.begin(), p->filter.end(), std::string(" AND NOT "));
	}
	sql<<";";
	p->prepared=sql.str();
	
	return Database::singleton()->resultset(p->prepared);
}

int QuerySetBase::count()
{
	std::cerr<<"Not implemented\n";
	throw(std::exception());
}

void QuerySetBase::del()
{
	std::cerr<<"Not implemented\n";
	throw(std::exception());
}
