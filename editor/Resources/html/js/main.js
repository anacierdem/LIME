/**
 * Created by alinacierdem on 29/09/16.
 */
//https://google-webfonts-helper.herokuapp.com/fonts

//Do not do too much work unless necessary, this is invoked synchronously!
function handleNativeMessage() {
    // Example message handler;
    // if(arguments[0] == "Action") {
    //     document.body.style.backgroundColor = "black";
    // }
}

//Can say we are starting a new frame by dispatching a message to C++
var update = function () {
    // Example C++ message;
    // nativeObject.engineManager("test");
    window.webkitRequestAnimationFrame(update);
};

update();