'use strict';
var sceneText = require('bindings')('node_text_detector');
var util = require('util');
var result;

console.log('-----------------------------------------------------');
result = sceneText.GetTextSync('sample_images/3.jpg');
console.log(util.inspect(result, {depth: 9, colors: true}));

