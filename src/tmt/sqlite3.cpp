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
	
static const char* nullToEmpty( char const* s){
	return (s ? s : "");
}


class ResultSetSqlite : public ResultSet{
	sqlite3_stmt *ppStmt;
	bool _atend;
public:
	ResultSetSqlite(const std::string &query) : _atend(false){
		ppStmt=NULL;
		int rc=sqlite3_prepare_v2(db, query.c_str(),-1, &ppStmt, NULL);
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
		try{
			switch(c){
				case 0:
					break;
				case 1:
					r.set(nullToEmpty((const char *)sqlite3_column_text(ppStmt, 0)));
					break;
				case 2:
					r.set(nullToEmpty((const char *)sqlite3_column_text(ppStmt, 0)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 1)));
					break;
				case 3:
					r.set(nullToEmpty((const char *)sqlite3_column_text(ppStmt, 0)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 1)),
								nullToEmpty((const char *)sqlite3_column_text(ppStmt, 2)));
					break;
				case 4:
					r.set(nullToEmpty((const char *)sqlite3_column_text(ppStmt, 0)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 1)),
								nullToEmpty((const char *)sqlite3_column_text(ppStmt, 2)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 3)));
					break;
				case 5:
					r.set(nullToEmpty((const char *)sqlite3_column_text(ppStmt, 0)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 1)),
								nullToEmpty((const char *)sqlite3_column_text(ppStmt, 2)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 3)),
								nullToEmpty((const char *)sqlite3_column_text(ppStmt, 4)));
					break;
				case 6:
					r.set(nullToEmpty((const char *)sqlite3_column_text(ppStmt, 0)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 1)),
								nullToEmpty((const char *)sqlite3_column_text(ppStmt, 2)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 3)),
								nullToEmpty((const char *)sqlite3_column_text(ppStmt, 4)),nullToEmpty((const char *)sqlite3_column_text(ppStmt, 5)));
					break;
			default:
					for (int i=0;i<c;i++){
						const char *v=(const char *)sqlite3_column_text(ppStmt, i);
						r.set(i, v);
					}
			}
		}
		catch(const tmt::Record::not_implemented &e){
			for (int i=0;i<c;i++){
				const char *v=(const char *)sqlite3_column_text(ppStmt, i);
				r.set(i, v);
			}
		}
	}
	bool atend() const{
		return _atend;
	}
};

SQLite::SQLite(const std::string &dbname){
	int rc = sqlite3_open(dbname.c_str(), &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		throw(std::exception());
	}
}

SQLite::~SQLite(){
	sqlite3_close(db);
}

ResultSet *SQLite3::resultset(const std::string &query){
	return new ResultSetSqlite(query);
}

int SQLite::query(const std::string& query, const fields_and_values& values){
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

