function update(obj) {
    obj.characterMovement = 1000;
}
var char_string = getObjectName("sprite_context");
var characterObj = gethandle(char_string);
//print("This is the current speed of the character " + characterObj.characterMovement);
update(characterObj);
//print("This is the current speed of the character after the change " + characterObj.characterMovement);