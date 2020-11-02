function updateImage() {
  getPressedKeys().forEach(action => action.execute());
}

function getPressedKeys() {
  return keyBindings.filter(action =>
    action.keys.some(binding => keys[binding]));
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

var rotation = Math.PI;
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