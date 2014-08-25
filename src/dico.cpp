/*
 * dico.cpp
 *
 *  Created on: 3 d?c. 2012
 *      Author:  Victor Andritoiu
 */

#include "dico.h"

v8::Persistent<v8::FunctionTemplate> Dico::constructor;

Dico::Dico(): node::ObjectWrap(), dico_(NULL) { }

void Dico::Init(v8::Handle<v8::Object> target) {
	v8::HandleScope scope;

	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(Dico::New);
	v8::Local<v8::String> name = v8::String::NewSymbol("Dico");

	constructor = v8::Persistent<v8::FunctionTemplate>::New(tpl);

	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(name);

	NODE_SET_PROTOTYPE_METHOD(constructor, "getVariablesNames", Dico::getVariablesNames);
	NODE_SET_PROTOTYPE_METHOD(constructor, "createVariable", Dico::createVariable);
	NODE_SET_PROTOTYPE_METHOD(constructor, "type", Dico::type);
	NODE_SET_PROTOTYPE_METHOD(constructor, "timestamp", Dico::timestamp);
	NODE_SET_PROTOTYPE_METHOD(constructor, "set", Dico::set);
	NODE_SET_PROTOTYPE_METHOD(constructor, "get", Dico::get);

	target->Set(name, constructor->GetFunction());
}

v8::Handle<v8::Value> Dico::New(const v8::Arguments& args) {
	v8::HandleScope scope;

	if (!args.IsConstructCall()) {
		return ThrowException(v8::Exception::TypeError(
				v8::String::New("Use the new operator to create instances of this object."))
		);
	}

	if (args.Length() < 5) {
		return ThrowException(v8::Exception::TypeError(
				v8::String::New("See API for info: must have 5 arguments")));
	}

	Dico* obj = new Dico();
	v8::String::Utf8Value name(args[0]->ToString());
	int port = args[1]->Int32Value();
	v8::String::Utf8Value addr(args[2]->ToString());
	int mPort = args[3]->Int32Value();
	v8::String::Utf8Value mAddr(args[4]->ToString());
	
	obj->name_ = std::string(*name);
	std::cout<<obj->name_<<","<<port<<","<<std::string(*addr)<<","<<mPort<<","<<std::string(*mAddr)<<std::endl;
	obj->dico_ = new O3SDico(obj->name_, 
				 port, 
				 std::string(*addr), 
				 mPort,
				 std::string(*mAddr));

	if (obj->dico_) {
		std::cout<<"Dico created: wrapping it..."<<std::endl;
		obj->Wrap(args.This());
		return args.This();
	} else {
		std::cout<<"Dico creation failed"<<std::endl;
	}

	return v8::Undefined();
}


v8::Handle<v8::Value> Dico::getVariablesNames(const v8::Arguments& args) {
	v8::HandleScope scope;

	Dico* obj = node::ObjectWrap::Unwrap<Dico>(args.This());

	if (obj->dico_) {

		std::vector<std::string> vlist = obj->dico_->getVariablesNames();
		v8::Local<v8::Array> names = v8::Array::New(vlist.size());

		unsigned int i = 0;
		for (std::vector<std::string>::iterator it = vlist.begin();
				it != vlist.end(); ++it) {
			names->Set(i, v8::String::New((*it).c_str()));
			i++;
		}

		return scope.Close(names);
	} else {
		return v8::Undefined();
	}
}

v8::Handle<v8::Value> Dico::createVariable(const v8::Arguments& args) {
	v8::HandleScope scope;

	Dico* obj = node::ObjectWrap::Unwrap<Dico>(args.This());
	v8::String::Utf8Value name(args[0]->ToString());
	v8::String::Utf8Value type(args[1]->ToString());
	obj->dico_->createVariable(std::string(*name), std::string(*type),
			"nodejs");

	return scope.Close(args.This());
}

v8::Handle<v8::Value> Dico::type(const v8::Arguments& args) {
	v8::HandleScope scope;

	Dico* obj = node::ObjectWrap::Unwrap<Dico>(args.This());
	v8::String::Utf8Value name(args[0]->ToString());
	std::string t = obj->dico_->type(std::string(*name));

	return scope.Close(v8::String::New(t.c_str()));
}

v8::Handle<v8::Value> Dico::timestamp(const v8::Arguments& args) {
	v8::HandleScope scope;

	Dico* obj = node::ObjectWrap::Unwrap<Dico>(args.This());
	v8::String::Utf8Value name(args[0]->ToString());
	unsigned long t = obj->dico_->timestamp(std::string(*name));

	return scope.Close(v8::Number::New(t));
}

v8::Handle<v8::Value> Dico::get(const v8::Arguments& args) {
	v8::HandleScope scope;

	Dico* obj = node::ObjectWrap::Unwrap<Dico>(args.This());
	v8::String::Utf8Value avname_(args[0]->ToString());
	std::string name(*avname_);
	std::string type = obj->dico_->type(name);

	v8::Local<v8::Value> value;

	if (type == "bool") {
		bool v = obj->dico_->getBool(name);
		value = v8::Local<v8::Value>::New(v8::Boolean::New(v));
	} else if (type == "byte" || type == "short" || type == "int") {
		value = v8::Int32::New(obj->dico_->getInt(name));
	} else if (type == "long") {
		value = v8::Integer::New(obj->dico_->getLong(name));
	} else if (type == "double") {
		value = v8::Number::New(obj->dico_->getDouble(name));
	} else if (type == "string") {
		value = v8::String::New(obj->dico_->getString(name).c_str());
	} else {
		value = v8::Number::New(obj->dico_->getDouble(name));
	} 

	return scope.Close(value);
}

v8::Handle<v8::Value> Dico::set(const v8::Arguments& args) {
	v8::HandleScope scope;

	Dico* obj = node::ObjectWrap::Unwrap<Dico>(args.This());
	v8::String::Utf8Value avname_(args[0]->ToString());
	std::string name(*avname_);
	std::string type = obj->dico_->type(name);

	if (type == "bool") {
		obj->dico_->setBool(name, args[1]->BooleanValue());
	} else if (type == "byte" || type == "short" || type == "int") {
		obj->dico_->setInt(name, args[1]->Int32Value());
	} else if (type == "long") {
		obj->dico_->setLong(name, args[1]->IntegerValue());
	} else if (type == "double") {
		obj->dico_->setDouble(name, args[1]->NumberValue());
	} else if (type == "string") {
		v8::String::Utf8Value value(args[1]->ToString());
		obj->dico_->setString(name, std::string(*value));
	}

	return scope.Close(v8::Undefined());
}


