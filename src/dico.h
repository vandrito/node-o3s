/*
 * dico.h
 *
 *  Created on: 3 d?c. 2012
 *      Author:  Victor Andritoiu
 */

#ifndef DICO_H_
#define DICO_H_

#include <node.h>

#include <O3SDico.h>

class Dico : public node::ObjectWrap {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Init(v8::Handle<v8::Object> target);

protected:
	Dico();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> getVariablesNames(const v8::Arguments& args);
	static v8::Handle<v8::Value> createVariable(const v8::Arguments& args);
	static v8::Handle<v8::Value> type(const v8::Arguments& args);
	static v8::Handle<v8::Value> timestamp(const v8::Arguments& args);

	static v8::Handle<v8::Value> get(const v8::Arguments& args);
	static v8::Handle<v8::Value> set(const v8::Arguments& args);

	O3SDico* dico_;
	std::string name_;
};

#endif /* DICO_H_ */
