//TODO: set texture coordinates, set & load program, load shaders

  uniform vec2 resolution;
  uniform vec2 translation;
  uniform vec2 rotation;
  uniform vec2 scale;
  uniform vec2 shear;
  uniform vec2 texture

  //uniforms....prog is not set yet but it will be initialized 
  //as a GL program at some point
  var resCoord = gl.getUniformLocation(prog, "resolution");
  var textureCoord = gl.getUniformLocation(prog, "texture");
  var translateCoord = gl.getUniformLocation(prog, "translation");
  var rotationCoord = gl.getUniformLocation(prog, "rotation");
  var scaleCoord = gl.getUniformLocation(prog, "scale");
  var shearCoord = gl.getUniformLocation(prog, "shear");

function setTexture(url) {
    var texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE, new Uint8Array([255, 245, 157, 255]));
    var img = new Image();
    img.addEventListener("load", function() {
      gl.bindTexture(gl.TEXTURE_2D, texture);
      gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
      gl.generateMipmap(gl.TEXTURE_2D);
    });
    img.src = url;
    return texture;
  }

  var image = setTexture("picture.jpg");

function updateImage() {
  getPressedKeys().forEach(action => action.execute());
}

function getPressedKeys() {
  return keyBinds.filter(action =>
    action.buttons.some(binding => buttons[binding]));
}

function renderImage(time) {
    updateTransformations(); 
    draw();
    requestAnimationFrame(render);
  }
  requestAnimationFrame(render);
}

 function drawImage() {
    gl.viewport(0, 0, gl.page.width, gl.page.height);
    gl.clearColor(0, 0, 0, 0);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.useProgram(program);
    gl.bindVertexArray(vao);
    //apply uniforms
    gl.uniform2f(resCoord, gl.page.width, gl.page.height);
    gl.uniform1i(textureCoord, 0);
    //transform uniforms
    gl.uniform2f(translateCoord, translation.x, translation.y);
    gl.uniform1f(rotateCoord, rotation);
    gl.uniform1f(scaleCoord, scale);
    gl.uniform2f(shearCoord, shear.x, shear.y);
    //apply image to texture
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, image);
    gl.drawArrays(gl.TRIANGLES, 0, 6);
  }

const TRANSLATE = 0.01;
const ROTATE = 0.01;
const SHEAR = 0.01;
const SCALE = 0.01;

var key = {}; // keys being pressed

// Transformation trackers
var translation = {
  x: 0.5,
  y: 0.5
};

var rotation = 3.141593;
var scale = 1.0;
var shear = {
  x: 0.0,
  y: 0.0
};

var keyBinds = [
  {
    description: "Move left",
    button: ["ArrowLeft"], 
    execute: () => translation.x -= TRANSLATE
  }, {
    description: "Move up",
    button: ["ArrowUp"], 
    execute: () => translation.y += TRANSLATE
  }, {
    description: "Move right",
    button: ["ArrowRight"], 
    execute: () => translation.x += TRANSLATE
  }, {
    description: "Move down",
    button: ["ArrowDown"], 
    execute: () => translation.y -= TRANSLATE
  }, {
    description: "Rotate clockwise",
    button: ["KeyQ"], 
    execute: () => rotation -= ROTATE
  }, {
    description: "Rotate counter-clockwise",
    button: ["KeyW"], 
    execute: () => rotation += ROTATE
  }, {
    description: "Shear x (positive)",
    button: ["KeyE"], 
    execute: () => shear.x += SHEAR
  }, {
    description: "Shear y (negative)",
    button: ["KeyR"],
    execute: () => shear.y -= SHEAR
  }, {
    description: "Shear x (negative)",
    button: ["KeyT"],
    execute: () => shear.x -= SHEAR
  }, {
    description: "Shear y (positive)",
    button: ["KeyY"],
    execute: () => shear.y += SHEAR
  }, {
    description: "Scale up",
    button: ["KeyU"],
    execute: () => scale += SCALE
  }, {
    description: "Scale Down",
    button: ["KeyI"], 
    execute: () => scale -= SCALE
  }
]

// save keypress
window.addEventListener("keydown", function(e) {
  button[e.code] = true;

  // prevent default browser behavior
  if (getPressedKeys().length)
    e.preventDefault();
});
window.addEventListener("keyup", function(e) {
  delete button[e.code];
});

document.getElementById("js-Binds").innerHTML += keyBinds.map(action => `
    <div>
      ${action.description} - <b>[${action.button.join(", ")}]</b>
    </div>
  `).join("");

main();