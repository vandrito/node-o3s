/*
 * Resolver.h
 *
 *  Created on: 23 nov. 2012
 *      Author: Victor Andritoiu
 */

#ifndef RESOLVER_H_
#define RESOLVER_H_

#include <node.h>

#include <O3SResolver.h>

class Resolver : public node::ObjectWrap {
public:
	static v8::Persistent<v8::FunctionTemplate> constructor;
	static void Init(v8::Handle<v8::Object> target);

protected:
	Resolver();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);

	static v8::Handle<v8::Value> getVariablesNames(const v8::Arguments& args);
	static v8::Handle<v8::Value> createVariable(const v8::Arguments& args);
	static v8::Handle<v8::Value> type(const v8::Arguments& args);
	static v8::Handle<v8::Value> timestamp(const v8::Arguments& args);

	static v8::Handle<v8::Value> get(const v8::Arguments& args);
	static v8::Handle<v8::Value> set(const v8::Arguments& args);

	O3SResolver* resolver_;
	std::string name_;
};

#endif /* RESOLVER_H_ */
