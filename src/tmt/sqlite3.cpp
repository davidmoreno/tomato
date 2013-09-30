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
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <iostream>

#include "sqlite3.hpp"
#include "tmt.hpp"
#include "resultset.hpp"

namespace tmt{
	class SQLite3;
}

using namespace tmt;

static const char* nullToEmpty( char const* s){
	return (s ? s : "");
}


class ResultSetSqlite : public ResultSet{
	sqlite3_stmt *ppStmt;
	bool _atend;
public:
	ResultSetSqlite(SQLite3 *sqlite, const std::string &query) : _atend(false){
		ppStmt=NULL;
		int rc=sqlite3_prepare_v2(sqlite->db, query.c_str(),-1, &ppStmt, NULL);
		if( rc!=SQLITE_OK ){
			throw(tmt::invalid_query(query));
		}

	}
	~ResultSetSqlite(){
		sqlite3_finalize(ppStmt);
	}
	void next(Record &r){
		if (_atend)
			return;
		auto rc = sqlite3_step(ppStmt);
		if( rc!=SQLITE_ROW ){
			_atend=true;
			return;
		}
		int c=sqlite3_column_count(ppStmt);
		fields_and_refs fields=r.field_refs();
		
		if (c!=fields.size()+1)
			throw(tmt::invalid_count_of_columns(std::string("Required ")+std::to_string(fields.size())+std::string(", but was ")+std::to_string(c)));
		
		r.id=sqlite3_column_int(ppStmt, 0);
		
		for (int i=1;i<c;i++){
			fields[i].second=nullToEmpty((const char *)sqlite3_column_text(ppStmt, i));
		}
	}
	bool atend() const{
		return _atend;
	}
};

SQLite3::SQLite3(const std::string &dbname){
	int rc = sqlite3_open(dbname.c_str(), &db);
	if( rc ){
		throw(tmt::initialization_exception(std::string("Can't open database: ")+sqlite3_errmsg(db)));
	}
}

SQLite3::~SQLite3(){
	sqlite3_close(db);
}

ResultSet *SQLite3::resultset(const std::string &query){
	return new ResultSetSqlite(this, query);
}

int SQLite3::query(const std::string& query, const fields_and_values& values){
	sqlite3_stmt *ppStmt=NULL;
	int rc=sqlite3_prepare_v2(db, query.c_str(),-1, &ppStmt, NULL);
	if( rc!=SQLITE_OK ){
		throw(tmt::invalid_query(query));
	}
	
	if (!values.empty()){
		int n=1;
		for(auto pair:values){
			sqlite3_bind_text(ppStmt, n, pair.second.c_str(), pair.second.length(), SQLITE_STATIC);
			//std::cerr<<"Bind to "<<n<<" "<<pair.second<<std::endl;
			n++;
		}
	}

	rc = sqlite3_step(ppStmt);
	if (rc!=SQLITE_DONE)
		throw(tmt::exception(sqlite3_errstr(rc)));
	sqlite3_free(ppStmt);
}

