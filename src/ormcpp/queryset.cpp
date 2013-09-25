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

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>

#include "orm.hpp"
#include "queryset.hpp"
#include "resultset.hpp"

namespace ORM{
	class QuerySetBase::QuerySetPrivate{
		public:
			std::vector<std::string> filter;
			std::vector<std::string> exclude;
			std::string prepared;
			std::string table;
	};
	ResultSet *sqlite3_resultset(const std::string &query);
}

using namespace ORM;

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
	
	return sqlite3_resultset(p->prepared);
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
