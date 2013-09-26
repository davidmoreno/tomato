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

#include <string>
#include "resultset.hpp"

namespace tmt{
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

			bool operator==(const QuerySet<T>::iterator &other) const{
				return (other.qa==qa && value==other.value);
			}
			
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

			bool atend() const{
				return qa->atend();
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
			QuerySet<T>::iterator I=begin();
			if (I.atend())
				throw(tmt::does_not_exist(query +" = "+value));
			T ret{*I};
			++I;
			if (!I.atend())
				throw(tmt::not_unique(query +" = "+value));
			return ret;
		}
	};
}
