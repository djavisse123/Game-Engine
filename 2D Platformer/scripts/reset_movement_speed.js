function update(obj) {
    obj.characterMovement = 150;
}
var char_string = getObjectName("sprite_context");
var characterObj = gethandle(char_string);
update(characterObj);
print("The speed of the character has been reset.");