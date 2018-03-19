var objectEditor = document.getElementById("objectEditor");

var draw = SVG(objectEditor).size('100%', '100%')
var polygon = draw.polygon().fill('#f06').stroke({ width: 1 });
var handleGroup = draw.group()
var vertices = [];

function addDrag(item, updateCallback) {
    var lastX, lastY;
    item.on("mousedown", function (event) {
        lastX = (event.clientX - objectEditor.offsetLeft + objectEditor.offsetWidth / 2);
        lastY = (event.clientY - objectEditor.offsetTop + objectEditor.offsetHeight / 2);

        SVG.on(document.body, "mousemove", function (event) {
            var x = (event.clientX - objectEditor.offsetLeft + objectEditor.offsetWidth / 2);
            var y = (event.clientY - objectEditor.offsetTop + objectEditor.offsetHeight / 2);
            var dx = x - lastX;
            var dy = y - lastY;

            updateCallback(dx, dy);

            lastX = x;
            lastY = y;
        });
    });

    SVG.on(document.body, "mouseup", function (item, event) {
        SVG.off(document.body, "mousemove");
    }.bind(null, item));
}

function updateCircle(index, dx, dy) {
    var x = vertices[index][0] + dx;
    var y = vertices[index][1] + dy;

    x = x < 0 ? 0 : x;
    y = y < 0 ? 0 : y;

    x = x > objectEditor.offsetWidth ? objectEditor.offsetWidth : x;
    y = y > objectEditor.offsetHeight ? objectEditor.offsetHeight : y;

    vertices[index][0] = x;
    vertices[index][1] = y;

    handleGroup.get(index).move(x - 5, y - 5);

    polygon.plot(vertices);
}

function drawPolygon() {

	handleGroup.each(function (i) {
		this.remove();
	})

	polygon.plot(vertices);

	var circle;
	for (var i = 0; i < vertices.length; i++) {
		circle = draw.circle(10);
		circle.move(vertices[i][0] - 5, vertices[i][1] - 5);
		circle.addClass("handle");
		handleGroup.add(circle);

		addDrag(circle, updateCircle.bind(null, i));
	}
}

function distanceSqr(first, second) {
    return (first[0] - second[0]) * (first[0] - second[0]) + (first[1] - second[1]) * (first[1] - second[1]);
}

function dotProduct(first, second) {
    return first[0] * second[0] + first[1] * second[1];
}

addDrag(polygon, function (dx, dy) {
    for (var i = 0; i < vertices.length; i++) {
        updateCircle(i, dx, dy);
    }
});

objectEditor.addEventListener("dblclick", function (event) {
	var x = event.pageX - objectEditor.offsetLeft;
	var y = event.pageY - objectEditor.offsetTop;

	var insertIndex = -1;
	var distance = Number.MAX_VALUE;
	for (var i = 0; i < vertices.length; i++) {
	    var nextIndex = i + 1;
	    nextIndex = nextIndex > vertices.length - 1 ? 0 : nextIndex;
	    var norm = [vertices[i][1] - vertices[nextIndex][1], vertices[nextIndex][0] - vertices[i][0]];
	    var vecToPoint = [x - vertices[i][0], y - vertices[i][1]];

	    var currDist = Math.abs(dotProduct(vecToPoint, norm));
	    if (currDist < distance) {
	        distance = currDist
	        insertIndex = nextIndex;
	    }
	}

	if (vertices.length > 2) {
	    vertices.splice(insertIndex, 0, [x, y]);
	} else {
	    vertices.push([x, y]);
	}

	drawPolygon();
});

//TODO: draggable that can update arbitrary data