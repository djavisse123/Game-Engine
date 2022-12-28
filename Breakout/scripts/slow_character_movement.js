function update(obj) {
    obj.characterMovement = 100;
}
var char_string = getObjectName("sprite_context");
var characterObj = gethandle(char_string);
update(characterObj);
