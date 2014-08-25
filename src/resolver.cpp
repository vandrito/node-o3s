/*
 * resolver.cpp
 *
 *  Created on: 24 nov. 2012
 *      Author: Victor Andritoiu
 */
#include "resolver.h"

#include "Poco/DNSSD/DNSSDException.h"
#include "Poco/Thread.h"

using namespace v8;

Persistent<FunctionTemplate> Resolver::constructor;

Resolver::Resolver(): ObjectWrap(), resolver_(NULL) { }

void Resolver::Init(v8::Handle<v8::Object> target) {
	HandleScope scope;

	Local<FunctionTemplate> tpl = FunctionTemplate::New(Resolver::New);
	Local<String> name = String::NewSymbol("Resolver");

	constructor = Persistent<FunctionTemplate>::New(tpl);

	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(name);

	NODE_SET_PROTOTYPE_METHOD(constructor, "getVariablesNames", Resolver::getVariablesNames);
	NODE_SET_PROTOTYPE_METHOD(constructor, "createVariable", Resolver::createVariable);
	NODE_SET_PROTOTYPE_METHOD(constructor, "type", Resolver::type);
	NODE_SET_PROTOTYPE_METHOD(constructor, "timestamp", Resolver::timestamp);
	NODE_SET_PROTOTYPE_METHOD(constructor, "set", Resolver::set);
	NODE_SET_PROTOTYPE_METHOD(constructor, "get", Resolver::get);

	target->Set(name, constructor->GetFunction());
}

Handle<Value> Resolver::New(const Arguments& args) {
	HandleScope scope;

	if (!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
		);
	}

	if (args.Length() < 2) {
		return ThrowException(Exception::TypeError(
				String::New("First argument must be a string, second a boolean")));
	}

	Resolver* obj = new Resolver();
	String::Utf8Value name(args[0]->ToString());
	obj->name_ = std::string(*name);
	obj->resolver_ = new O3SResolver(obj->name_, false);

	if (obj->resolver_) {
		while (!obj->resolver_->getDico()) {
			std::cout<<"Waiting for dico..."<<std::endl;
			Poco::Thread::sleep(1000);
		}
		obj->Wrap(args.This());

		Local<Function> cb = Local<Function>::Cast(args[2]);
		const unsigned argc = 1;
		Local<Value> argv[argc] = { args.This() };
		cb->Call(Context::GetCurrent()->Global(), argc, argv);

		return args.This();
	}

	return Undefined();
}


Handle<Value> Resolver::getVariablesNames(const Arguments& args) {
	HandleScope scope;

	Resolver* obj = ObjectWrap::Unwrap<Resolver>(args.This());

	if (obj->resolver_->getDico()) {

		std::vector<std::string> vlist = obj->resolver_->getDico()->getVariablesNames();
		Local<Array> names = Array::New(vlist.size());

		unsigned int i = 0;
		for (std::vector<std::string>::iterator it = vlist.begin();
				it != vlist.end(); ++it) {
			names->Set(i, String::New((*it).c_str()));
			i++;
		}

		return scope.Close(names);
	} else {
		return Undefined();
	}
}

Handle<Value> Resolver::createVariable(const Arguments& args) {
	HandleScope scope;

	Resolver* obj = ObjectWrap::Unwrap<Resolver>(args.This());
	String::Utf8Value name(args[0]->ToString());
	String::Utf8Value type(args[1]->ToString());
	obj->resolver_->getDico()->createVariable(std::string(*name), std::string(*type),
			"nodejs");

	return scope.Close(args.This());
}

Handle<Value> Resolver::type(const Arguments& args) {
	HandleScope scope;

	Resolver* obj = ObjectWrap::Unwrap<Resolver>(args.This());
	String::Utf8Value name(args[0]->ToString());
	std::string t = obj->resolver_->getDico()->type(std::string(*name));

	return scope.Close(String::New(t.c_str()));
}

Handle<Value> Resolver::timestamp(const Arguments& args) {
	HandleScope scope;

	Resolver* obj = ObjectWrap::Unwrap<Resolver>(args.This());
	String::Utf8Value name(args[0]->ToString());
	unsigned long t = obj->resolver_->getDico()->timestamp(std::string(*name));

	return scope.Close(Number::New(t));
}

Handle<Value> Resolver::get(const Arguments& args) {
	HandleScope scope;

	Resolver* obj = ObjectWrap::Unwrap<Resolver>(args.This());
	String::Utf8Value avname_(args[0]->ToString());
	std::string name(*avname_);
	std::string type = obj->resolver_->getDico()->type(name);

	Local<Value> value;

	if (type == "bool") {
		bool v = obj->resolver_->getDico()->getBool(name);
		value = Local<Value>::New(Boolean::New(v));
	} else if (type == "byte" || type == "short" || type == "int") {
		value = Int32::New(obj->resolver_->getDico()->getInt(name));
	} else if (type == "long") {
		value = Integer::New(obj->resolver_->getDico()->getLong(name));
	} else if (type == "double") {
		value = Number::New(obj->resolver_->getDico()->getDouble(name));
	} else if (type == "string") {
		value = String::New(obj->resolver_->getDico()->getString(name).c_str());
	}

	return scope.Close(value);
}

Handle<Value> Resolver::set(const Arguments& args) {
	HandleScope scope;

	Resolver* obj = ObjectWrap::Unwrap<Resolver>(args.This());
	String::Utf8Value avname_(args[0]->ToString());
	std::string name(*avname_);
	std::string type = obj->resolver_->getDico()->type(name);

	if (type == "bool") {
		obj->resolver_->getDico()->setBool(name, args[1]->BooleanValue());
	} else if (type == "byte" || type == "short" || type == "int") {
		obj->resolver_->getDico()->setInt(name, args[1]->Int32Value());
	} else if (type == "long") {
		obj->resolver_->getDico()->setLong(name, args[1]->IntegerValue());
	} else if (type == "double") {
		obj->resolver_->getDico()->setDouble(name, args[1]->NumberValue());
	} else if (type == "string") {
		String::Utf8Value value(args[1]->ToString());
		obj->resolver_->getDico()->setString(name, std::string(*value));
	}

	return scope.Close(Undefined());
}

