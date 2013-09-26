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

#include <exception>
#include <string>

namespace tmt{
	class Record{
	public:
		class not_implemented : public std::exception{};
		virtual void set(
				int n,
				const std::string &a
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b, const std::string &c
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b, const std::string &c,
				const std::string &d
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b, const std::string &c,
				const std::string &d, const std::string &e
			){ throw not_implemented(); };
		virtual void set(
				const std::string &a, const std::string &b, const std::string &c,
				const std::string &d, const std::string &e, const std::string &f
			){ throw not_implemented(); };
	};
}
