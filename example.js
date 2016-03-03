'use strict';
var sceneText = require('bindings')('scene_text_detector');
var util = require('util');
var fs = require('fs');
var result;

console.log('-----------------------------------------------------');
fs.readFile('./sample_images/3.jpg', function(error, data) {
    if (!error) {
        var result = sceneText.GetTextSync(data, __dirname, '0123456789ABCDEFGHJKLMNPRSTUVWXYZ');
        console.log(util.inspect(result, {depth: 9, colors: true}));
    } else {
        console.log('Load Image from Path failed');
        return undefined;
    }
});