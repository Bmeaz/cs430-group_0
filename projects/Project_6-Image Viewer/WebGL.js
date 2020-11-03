//TODO: set texture coordinates, set uniforms, load shaders

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
  return keyBindings.filter(action =>
    action.keys.some(binding => keys[binding]));
}

function renderImage(time) {
    updateTransformations(); 
    draw();
    requestAnimationFrame(render);
  }
  requestAnimationFrame(render);
}

 function drawImage() {
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
    gl.clearColor(0, 0, 0, 0);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.useProgram(program);
    gl.bindVertexArray(vao);
    //apply uniforms
    gl.uniform2f(resolutionLocation, gl.canvas.width, gl.canvas.height);
    gl.uniform1i(textureLocation, 0);
    //transform uniforms
    gl.uniform2f(translationLocation, translation.x, translation.y);
    gl.uniform1f(rotationLocation, rotation);
    gl.uniform1f(scaleLocation, scale);
    gl.uniform2f(shearLocation, shear.x, shear.y);
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
    key: ["ArrowLeft"], 
    execute: () => translation.x -= TRANSLATE
  }, {
    description: "Move up",
    key: ["ArrowUp"], 
    execute: () => translation.y += TRANSLATE
  }, {
    description: "Move right",
    key: ["ArrowRight"], 
    execute: () => translation.x += TRANSLATE
  }, {
    description: "Move down",
    key: ["ArrowDown"], 
    execute: () => translation.y -= TRANSLATE
  }, {
    description: "Rotate clockwise",
    key: ["KeyQ"], 
    execute: () => rotation -= ROTATE
  }, {
    description: "Rotate counter-clockwise",
    key: ["KeyW"], 
    execute: () => rotation += ROTATE
  }, {
    description: "Shear x (positive)",
    key: ["KeyE"], 
    execute: () => shear.x += SHEAR
  }, {
    description: "Shear y (negative)",
    key: ["KeyR"],
    execute: () => shear.y -= SHEAR
  }, {
    description: "Shear x (negative)",
    key: ["KeyT"],
    execute: () => shear.x -= SHEAR
  }, {
    description: "Shear y (positive)",
    key: ["KeyY"],
    execute: () => shear.y += SHEAR
  }, {
    description: "Scale up",
    keyc: ["KeyU"],
    execute: () => scale += SCALE
  }, {
    description: "Scale Down",
    key: ["KeyI"], 
    execute: () => scale -= SCALE
  }
]

// save keypress
window.addEventListener("keydown", function(e) {
  key[e.code] = true;

  // prevent default browser behavior
  if (getPressedKeyBindings().length)
    e.preventDefault();
});
window.addEventListener("keyup", function(e) {
  delete key[e.code];
});

document.getElementById("js-keyBinds").innerHTML += keyBinds.map(action => `
    <div>
      ${action.description} - <b>[${action.key.join(", ")}]</b>
    </div>
  `).join("");

main();