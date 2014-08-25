/*
 * nodeo3s.cpp
 *
 *  Created on: 23 nov. 2012
 *      Author: Victor Andritoiu
 */
#include <node.h>

#include "dico.h"
#include "resolver.h"

void RegisterModule(v8::Handle<v8::Object> target) {
	Dico::Init(target);
	Resolver::Init(target);
}

NODE_MODULE(nodeo3s, RegisterModule)
