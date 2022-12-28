#include "v8helpers.h"

//std::vector<v8::Local<v8::Object>> objects;

namespace v8helpers
{

	/**
	 * Extracts a C string from a V8 Utf8Value. 
	 * Hat tip: https://github.com/v8/v8/blob/master/samples/shell.cc
	 */
	const char* ToCString(const v8::String::Utf8Value& value) {
		return *value ? *value : "<string conversion failed>";
	}

	/**
	 * The callback that is invoked by v8 whenever the JavaScript 'print'
	 * function is called.  Prints its arguments on stdout separated by spaces
	 * and ending with a newline. 
	 * Hat tip: * https://github.com/v8/v8/blob/master/samples/shell.cc
	 */
	void Print(const v8::FunctionCallbackInfo<v8::Value>& args) {
		bool first = true;
		for (int i = 0; i < args.Length(); i++) {
			// v8::HandleScope handle_scope(args.GetIsolate());
			if (first) {
				first = false;
			} else {
				printf(" ");
			}
			v8::String::Utf8Value str(args.GetIsolate(), args[i]);
			const char* cstr = ToCString(str);
			printf("%s", cstr);
		}
		printf("\n");
		fflush(stdout);
	}

	/**
	 * This function is used to access the context container and retrieves 
	 * the name of the object inside a context. 
	 */
	void getObjectV8(const v8::FunctionCallbackInfo<v8::Value>& args) {
		//Get the isolate to handle scope
		v8::HandleScope handle_scope(args.GetIsolate());
		//Set the context name to default
		std::string context_name("default");
		//If the amount of arguments in the function is equal to 1 then its probably the name of the context
		if(args.Length() == 1)
		{
			v8::String::Utf8Value str(args.GetIsolate(), args[0]);
			//Set the name of the context to the value of the argument
			context_name = std::string(v8helpers::ToCString(str));
		}
		v8::Isolate *isolate = args.GetIsolate();
		//Get the current context
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		//Get an instance of the object container
		ObjectContainer *objContainer = ObjectContainer::getInstance();
		//Use the object container to return the name of the object in the context
		args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), objContainer->listOfV8Objs[context_name].c_str()));
	}

} // v8helpers namespace