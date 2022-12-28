function update(obj) {
    obj.lives = 99;
}
var health_name = getObjectName("health_context");
var health_obj = gethandle(health_name);
//print("This is the current number of lives for the character " + health_obj.lives);
update(health_obj);
//print("This is the current number of lives for the character after the change " + health_obj.lives);