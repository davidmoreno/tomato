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
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <iostream>

#include "orm.hpp"
#include "resultset.hpp"

namespace ORM{
	
sqlite3 *db=NULL;

class ResultSetSqlite : public ResultSet{
	sqlite3_stmt *ppStmt;
	bool _atend;
public:
	ResultSetSqlite(const std::string &query) : _atend(false){
		ppStmt=NULL;
		int rc=sqlite3_prepare_v2(db, query.c_str(),-1, &ppStmt, NULL);
		if( rc!=SQLITE_OK ){
			throw(ORM::invalid_query(query));
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
					r.set((const char *)sqlite3_column_text(ppStmt, 0));
					break;
				case 2:
					r.set((const char *)sqlite3_column_text(ppStmt, 0),(const char *)sqlite3_column_text(ppStmt, 1));
					break;
				case 3:
					r.set((const char *)sqlite3_column_text(ppStmt, 0),(const char *)sqlite3_column_text(ppStmt, 1),(const char *)sqlite3_column_text(ppStmt, 2));
					break;
				case 4:
					r.set((const char *)sqlite3_column_text(ppStmt, 0),(const char *)sqlite3_column_text(ppStmt, 1),(const char *)sqlite3_column_text(ppStmt, 2),
						(const char *)sqlite3_column_text(ppStmt, 3));
					break;
				case 5:
					r.set((const char *)sqlite3_column_text(ppStmt, 0),(const char *)sqlite3_column_text(ppStmt, 1),(const char *)sqlite3_column_text(ppStmt, 2),
						(const char *)sqlite3_column_text(ppStmt, 3),(const char *)sqlite3_column_text(ppStmt, 4));
					break;
				case 6:
					r.set((const char *)sqlite3_column_text(ppStmt, 0),(const char *)sqlite3_column_text(ppStmt, 1),(const char *)sqlite3_column_text(ppStmt, 2),
						(const char *)sqlite3_column_text(ppStmt, 3),(const char *)sqlite3_column_text(ppStmt, 4),(const char *)sqlite3_column_text(ppStmt, 5));
					break;
			default:
					for (int i=0;i<c;i++){
						const char *v=(const char *)sqlite3_column_text(ppStmt, i);
						r.set(i, v);
					}
			}
		}
		catch(const ORM::Record::not_implemented &e){
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

void sqlite3_init(const std::string &dbname){
	int rc = sqlite3_open(dbname.c_str(), &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		throw(std::exception());
	}
}

void sqlite3_delete(){
	sqlite3_close(db);
}

ResultSet *sqlite3_resultset(const std::string &query){
	return new ResultSetSqlite(query);
}

void sqlite3_insert(const std::string &table, const ORM::fields_and_values &values){
	std::stringstream qi, qv;
	qi<<"INSERT INTO "<<table<<" (id,";
	int n=values.size();
	for (auto pair: values){
		n--;
		qi<<pair.first;
		qv<<'?';
		if (n>0){
			qi<<", ";
			qv<<", ";
		}
	}
	qi<<") VALUES ( (SELECT MAX(id)+1 FROM "<<table<<"), "<<qv.str()<<")";
	std::string query=qi.str();
	
	//std::cerr<<query<<std::endl;
	
	sqlite3_stmt *ppStmt=NULL;
	int rc=sqlite3_prepare_v2(db, query.c_str(),-1, &ppStmt, NULL);
	if( rc!=SQLITE_OK ){
		throw(ORM::invalid_query(query));
	}
	
	n=1;
	for(auto pair:values){
		sqlite3_bind_text(ppStmt, n, pair.second.c_str(), pair.second.length(), SQLITE_STATIC);
		//std::cerr<<"Bind to "<<n<<" "<<pair.second<<std::endl;
		n++;
	}

	rc = sqlite3_step(ppStmt);
	if (rc!=SQLITE_DONE)
		throw(ORM::exception(sqlite3_errstr(rc)));
}


void sqlite3_update(const std::string &table, int id, const ORM::fields_and_values &values){
	std::stringstream qi, qv;
	qi<<"UPDATE "<<table<<" SET ";
	int n=values.size();
	for (auto pair: values){
		n--;
		qi<<pair.first<<"="<<"?";
		qv<<'?';
		if (n>0){
			qi<<", ";
			qv<<", ";
		}
	}
	qi<<" WHERE id = "<<id;
	std::string query=qi.str();
	
	std::cerr<<query<<std::endl;
	
	sqlite3_stmt *ppStmt=NULL;
	int rc=sqlite3_prepare_v2(db, query.c_str(),-1, &ppStmt, NULL);
	if( rc!=SQLITE_OK ){
		throw(ORM::invalid_query(query));
	}
	
	n=1;
	for(auto pair:values){
		sqlite3_bind_text(ppStmt, n, pair.second.c_str(), pair.second.length(), SQLITE_STATIC);
		std::cerr<<"Bind to "<<n<<" "<<pair.second<<std::endl;
		n++;
	}

	rc = sqlite3_step(ppStmt);
	if (rc!=SQLITE_DONE)
		throw(ORM::exception(sqlite3_errstr(rc)));
}


} // namespace
