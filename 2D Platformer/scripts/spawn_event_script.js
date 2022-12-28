function update(e) {
    e.type = 6;
}
var new_Event = eventFactory();
//print("This is the guid of the event " + new_Event.guid);
update(new_Event);
//print("This is the type for the event " + new_Event.type);