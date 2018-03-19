var paletteManager = document.getElementById("tilePalette");
var selectedItem = null;

var ATLAS_WIDTH = 4;
var ATLAS_HEIGHT = 10;

// Draw a selectable tile pallette
for(var i = 0; i < ATLAS_WIDTH*ATLAS_HEIGHT; i++) {
    var tmpElem = document.createElement("div");
    tmpElem.style.background = 'url("../textures/iso2.png")';
    tmpElem.style.backgroundPositionX = -(i%ATLAS_WIDTH)*128 + "px";
    tmpElem.style.backgroundPositionY = -Math.floor(i/ATLAS_WIDTH)*128 + "px";
    tmpElem.style.width = "128px";
    tmpElem.style.height = "128px";
    tmpElem.style.float = "left";
    tmpElem.style.webkitBoxShadow= "inset 0 0 5px #000";

    tmpElem.addEventListener("click", function(x, y, elem) {
        return function() {
            if(selectedItem)
                selectedItem.style.webkitBoxShadow = "inset 0 0 0px #000";
            // Dispatch a native call
            nativeObject.paletteManager("setTile", x, y);
            selectedItem = elem;
            elem.style.webkitBoxShadow= "inset 0 0 0px 5px #992020";
        }
    }(i%ATLAS_WIDTH, Math.floor(i/ATLAS_WIDTH), tmpElem));
    paletteManager.appendChild(tmpElem);
}

paletteManager.addEventListener("mouseover", function() {
    nativeObject.engineManager("mouseCaptured");
});

paletteManager.addEventListener("mouseout", function(event) {
    //Fix for mouseout (chrome 19 does not support mouseleave)
    var e = event.toElement || event.relatedTarget;
    if (e.parentNode == this || e == this) {
        return;
    }
    nativeObject.engineManager("mouseReleased");
}, true);
