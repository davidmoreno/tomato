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

#include <string>
#include "resultset.hpp"

namespace ORM{
	class ResultSet;
	
	class QuerySetBase{
		class QuerySetPrivate;
		QuerySetPrivate *p;
	public:
		QuerySetBase(const QuerySetBase &b);
		QuerySetBase(const std::string &table);
		virtual ~QuerySetBase();
		
		bool operator=(const QuerySetBase &b) = delete;
		
		QuerySetBase &all();
		QuerySetBase &filter(const std::string &query, const std::string &value);
		QuerySetBase &filter(const std::string &query, int value);
		
		QuerySetBase &exclude(const std::string &query, const std::string &value);
		QuerySetBase &exclude(const std::string &query, int value);
		
		void del();
		int count();
		
		ResultSet *prepare();
	};
	
	template<typename T>
	class QuerySet : public QuerySetBase{
	public:
		class iterator : public std::iterator<std::input_iterator_tag, T>{
			T value;
			ResultSet *qa;
		public:
			iterator(QuerySet<T> *q) {
				if (q){
					qa=q->prepare();
					qa->next(value);
				}
				else{
					qa=NULL;
				}
			}
			~iterator(){
				delete qa;
			};
			
			bool operator=(const QuerySet<T>::iterator &other) = delete;
			
			bool operator!=(const QuerySet<T>::iterator &other) const{
				if (other.qa==NULL){
					return (!qa->atend());
				}
				return !(value==other.value);
			}
			
			QuerySet<T>::iterator &operator++(){
				qa->next(value);
				return *this;
			}
			
			T &operator*(){
				return value;
			}
		};
		QuerySet(const std::string &table) : QuerySetBase(table){}
		
		QuerySet<T>::iterator begin(){ return iterator(this); }
		QuerySet<T>::iterator end(){ return iterator(nullptr); }
		
		
		QuerySet<T> &all(){ QuerySetBase::all(); return *this; }
		QuerySet<T> &filter(const std::string &query, const std::string &value){ QuerySetBase::filter(query,value); return *this; }
		QuerySet<T> &filter(const std::string &query, int value){ QuerySetBase::filter(query,value); return *this; }
		
		QuerySet<T> &exclude(const std::string &query, const std::string &value){ QuerySetBase::exclude(query,value); return *this; }
		QuerySet<T> &exclude(const std::string &query, int value){ QuerySetBase::exclude(query,value); return *this; }
		
		T get(const std::string &query, const std::string &value){ // TODO exception if more than one
			filter(query, value);
			return *begin();
		}
	};
}
