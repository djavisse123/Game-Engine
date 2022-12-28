#include "Event.h"
#include <iostream>
#include <filesystem>
#include <cstring>
#include "ScriptManager.h"
#include "v8helpers.h"

//Set the current_guid and the list
int Event::current_guid = 0;
std::vector<Event*> Event::listOfEvents;

/**
 * Constructs an event. A new event is given a guid and is added to the list of events
 */
Event::Event() {
    guid = "Event" + std::to_string(current_guid);
    current_guid++;
    listOfEvents.push_back(this);
}

/**
 * Destroy an event once it leaves scope or is removed from the eventQueue.
 */
Event::~Event() {
    for(std::vector<Event *>::iterator it = listOfEvents.begin(); it != listOfEvents.end(); ++it) {
        if((*it)->guid == this->guid) {
            listOfEvents.erase(it);
            break;
        }
    }
}

/**
 * This function is used to expose the Event to v8.
 * Certain fields for the object are also exposed so they can be
 * accessed and modfied.
 * @param isolate The isolate used to enter the V8 vitrual machine
 * @param context The context the object will be in
 * @param context_name The name of the context
 * @return v8::Local<v8::Object> The v8 local object
 */
v8::Local<v8::Object> Event::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context)
{
	std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
	v.push_back(v8helpers::ParamContainer("type", getEventType, setEventType));
    v.push_back(v8helpers::ParamContainer("guid", getEventGUID, setEventGUID));
	return v8helpers::exposeToV8(guid, this, v, isolate, context);
}

/**
 * An event factory that the scripts can use to create events.
 */
void Event::scriptedEventFactory(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	v8::EscapableHandleScope handle_scope(args.GetIsolate());
	v8::Context::Scope context_scope(context);
	std::string context_name("default");
	if(args.Length() == 1)
	{
		v8::String::Utf8Value str(args.GetIsolate(), args[0]);
		context_name = std::string(v8helpers::ToCString(str));
	}
	Event *new_event = new Event();
	v8::Local<v8::Object> v8_obj = new_event->exposeToV8(isolate, context);
	args.GetReturnValue().Set(handle_scope.Escape(v8_obj));
}

/**
 * Allows for a script in V8 to modify the Event type
 */
void Event::setEventType(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<Event*>(ptr)->type = static_cast<Event::EventType>(value->Int32Value());
}

/**
 * Allows for a script in V8 to retrieve the Event type
 */
void Event::getEventType(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int e_type = static_cast<Event*>(ptr)->type;
    info.GetReturnValue().Set(e_type);
}

/**
 * Allows for a script in V8 to modify the Event's guid
 */
void Event::setEventGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
	static_cast<Event*>(ptr)->guid = *utf8_str;
}

/**
 * Allows for a script in V8 to retrieve the Event's guid
 */
void Event::getEventGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	std::string guid = static_cast<Event*>(ptr)->guid;
	v8::Local<v8::String> v8_guid = v8::String::NewFromUtf8(info.GetIsolate(), guid.c_str(), v8::String::kNormalString);
	info.GetReturnValue().Set(v8_guid);
}