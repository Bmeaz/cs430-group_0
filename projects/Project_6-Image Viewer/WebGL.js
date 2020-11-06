const vertexShaderSource = `#version 300 es

in vec2 a_pos;
in vec2 a_texturecoord;
out vec2 vec_texturecoord;
uniform vec2 resolution;
uniform vec2 translation;
uniform float rotation;
uniform float scale;
uniform vec2 shear;

void main() {
  float rotation_sin = sin(rotation);
  float rotation_cos = cos(rotation);

  //matrices for transformations
  mat2 rotationMatrix = mat2(rotation_cos, -rotation_sin, rotation_sin,  rotation_cos);
  mat2 shearMatrix = mat2(1, shear.y, shear.x, 1);

  //how transformations change the image
  vec2 position = shearMatrix * (rotationMatrix * (a_pos * scale)) + translation;
  gl_pos = vec4(position.x / (resolution.x / resolution.y), position.y, 0, 1);
  vec_texturecoord = a_texturecoord;
}`;

const fragmentShaderSource = `#version 300 es
precision mediump float;
in vec2 vec_texturecoord;
uniform sampler2D texture;
out vec4 colorOutPut;

void main() {
   colorOutPut = texture(texture, vec_texturecoord);
}`;


function loadShader(webGL, shaderSource, shaderType) {
  var imgShader = webGL.createShader(shaderType);

  webGL.shaderSource(imgShader, shaderSource);
  webGL.compileShader(imgShader);

  return imgShader;
}

function loadProgram(webGL) {
  var glProg = webGL.createProgram();
  var imgShader = loadShader(webGL, vertexShaderSource, webGL.VERTEX_SHADER);

  webGL.attachShader(glProg, imgShader);
  imgShader = loadShader(webGL, fragmentShaderSource, webGL.FRAGMENT_SHADER);
  webGL.attachShader(glProg, imgShader);
  webGL.linkProgram(glProg);

  return glProg;
}

function main() {

  var page = document.getElementById("canvas");
  var webGL = page.getContext("webgl2");

  if (!webGL) {
    return;
  }
  var glProg = loadProgram(webGL);
  var positionLocation = webGL.getAttribLocation(glProg, "a_pos");
  var texcoordLocation = webGL.getAttribLocation(glProg, "a_texturecoord");
  //get the uniforms
  var resolutionLocation = webGL.getUniformLocation(glProg, "resolution");
  var textureLocation = webGL.getUniformLocation(glProg, "texture");
  var translationLocation = webGL.getUniformLocation(glProg, "translation");
  var rotationLocation = webGL.getUniformLocation(glProg, "rotation");
  var scaleLocation = webGL.getUniformLocation(glProg, "scale");
  var shearLocation = webGL.getUniformLocation(glProg, "shear");
  var vertexArr = webGL.createVertexArray();

  gl.bindVertexArray(vertexArr);

  var positions = [
    0, 0,
    0, 1,
    1, 0,
    1, 0,
    0, 1,
    1, 1,
  ];
  var positionBuffer = webGL.createBuffer();

  webGL.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  webGL.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), webGL.STATIC_DRAW);
  webGL.enableVertexAttribArray(positionLocation);
  webGL.vertexAttribPointer(positionLocation, 2, webGL.FLOAT, false, 0, 0);

  var texcoords = [
    0, 0,
    0, 1,
    1, 0,
    1, 0,
    0, 1,
    1, 1,
  ];
  var texcoordBuffer = webGL.createBuffer();

  webGL.bindBuffer(webGL.ARRAY_BUFFER, texcoordBuffer);
  webGL.bufferData(webGL.ARRAY_BUFFER, new Float32Array(texcoords), webGL.STATIC_DRAW);
  webGL.enableVertexAttribArray(texcoordLocation);
  webGL.vertexAttribPointer(texcoordLocation, 2, webGL.FLOAT, true, 0, 0);

  function loadTexture(url) {
    var texture = webGL.createTexture();

    webGL.bindTexture(webGL.TEXTURE_2D, texture);
    webGL.texImage2D(webGL.TEXTURE_2D, 0, webGL.RGBA, 1, 1, 0, webGL.RGBA, webGL.UNSIGNED_BYTE, new Uint8Array([255, 245, 157, 255]));

    var img = new Image();

    img.addEventListener("load", function() {
      webGL.bindTexture(webGL.TEXTURE_2D, texture);
      webGL.texImage2D(webGL.TEXTURE_2D, 0, webGL.RGBA, webGL.RGBA, webGL.UNSIGNED_BYTE, img);
      webGL.generateMipmap(webGL.TEXTURE_2D);
    });
    img.src = url;

    return texture;
  }
  var image = loadTexture("world.jpg");

  function draw() {
    webGL.viewport(0, 0, webGL.canvas.width, webGL.canvas.height);
    webGL.clearColor(0, 0, 0, 0);
    webGL.clear(webGL.COLOR_BUFFER_BIT | webGL.DEPTH_BUFFER_BIT);
    webGL.useProgram(glProg);
    webGL.bindVertexArray(vertexArr);
    //apply uniforms
    webGL.uniform2f(resolutionLocation, webGL.canvas.width, webGL.canvas.height);
    webGL.uniform1i(textureLocation, 0);
    //uniforms
    webGL.uniform2f(translationLocation, translation.x, translation.y);
    webGL.uniform1f(rotationLocation, rotation);
    webGL.uniform1f(scaleLocation, scale);
    webGL.uniform2f(shearLocation, shear.x, shear.y);
    //add image to textures
    webGL.activeTexture(webGL.TEXTURE0);
    webGL.bindTexture(webGL.TEXTURE_2D, image);
    webGL.drawArrays(webGL.TRIANGLES, 0, 6);
  }

  function render(time) {
    updateTransformations();
    draw();
    requestAnimationFrame(render);
  }
  requestAnimationFrame(render);
}


function updateTransformations() {
  getPressedKeyBindings().forEach(action => action.action());
}

function getPressedKeyBindings() {
  return keyBindings.filter(action =>
    action.keyPress.some(binding => keyArr[binding]));
}


const TRANSLATION = 0.01;
const ROTATION = 0.01;
const SHEAR = 0.01;
const SCALE = 0.01;

var keyArr = {}; //all keys being pressed

//track transformation
var translation = {
  x: 0.5,
  y: 0.5
};

var rotation = Math.PI;
var scale = 1.0;
var shear = {
  x: 0.0,
  y: 0.0
};

var keyBindings = [
  {
    transformationDescription: "Move left",
    keyPress: ["Q"],
    action: () => translation.x -= TRANSLATION
  }, {
    transformationDescription: "Move up",
    keyPress: ["W"],
    action: () => translation.y += TRANSLATION
  }, {
    transformationDescription: "Move right",
    keyPress: ["R"],
    action: () => translation.x += TRANSLATION
  }, {
    transformationDescription: "Move down",
    keyPress: ["T"],
    action: () => translation.y -= TRANSLATION
  }, {
    transformationDescription: "Rotate clockwise",
    keyPress: ["Y"],
    action: () => rotation -= ROTATION
  }, {
    transformationDescription: "Rotate counter-clockwise",
    keyPress: ["A"],
    action: () => rotation += ROTATION
  }, {
    transformationDescription: "Shear x (positive)",
    keyPress: ["S"],
    action: () => shear.x += SHEAR
  }, {
    transformationDescription: "Shear y (negative)",
    keyPress: ["D"],
    action: () => shear.y -= SHEAR
  }, {
    transformationDescription: "Shear x (negative)",
    keyPress: ["F"],
    action: () => shear.x -= SHEAR
  }, {
    transformationDescription: "Shear y (positive)",
    keyPress: ["G"],
    action: () => shear.y += SHEAR
  }, {
    transformationDescription: "Scale up",
    keyPress: ["H"],
    action: () => scale += SCALE
  }, {
    transformationDescription: "Scale Down",
    keyPress: ["Z"],
    action: () => scale -= SCALE
  }
]

//save keypress
window.addEventListener("keydown", function(e) {
  keyPress[e.code] = true;

  //prevent default browser behavior
  if (getPressedKeyBindings().length)
    e.preventDefault();
});
window.addEventListener("keyup", function(e) {
  delete keyPress[e.code];
});

document.getElementById("js-keyBindings").innerHTML += keyBindings.map(action => `
    <div>
      ${action.transformationDescription} - <b>[${action.keyPress.join(", ")}]</b>
    </div>
  `).join("");

main();
