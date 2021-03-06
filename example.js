'use strict';
var sceneText = require('bindings')('scene_text_detector');
var util = require('util');
var fs = require('fs');
var result;

console.log('-----------------------------------------------------');

fs.readFile('./sample_images/3.jpg', function(error, data) {
    if (!error) {
        var result = sceneText.GetTextCropSync(data, __dirname, '0123456789ABCDEFGHJKLMNPRSTUVWXYZ', 0, 0, 440, 330, 90);
        console.log(util.inspect(result, {depth: 9, colors: true}));
    } else {
        console.log('Load Image from Path failed');
        return undefined;
    }
});

//var result2 = sceneText.GetTextCropSync('./sample_images/3.jpg', __dirname, '0123456789ABCDEFGHJKLMNPRSTUVWXYZ', 140, 133, 150, 33);
//console.log(util.inspect(result2, {depth: 9, colors: true}));