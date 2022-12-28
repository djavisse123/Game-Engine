function update(obj) {
    obj.lives = 3;
}
var health_name = getObjectName("health_context");
var health_obj = gethandle(health_name);
update(health_obj);
print("The number of lives has been reset.");
