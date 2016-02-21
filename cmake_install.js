var AdmZip = require('adm-zip');
var https = require('https');
var fs = require('fs');
var exec = require('child_process').exec, child;

function onError(err) {
  console.error('An error occurred:', err)
}

function onEnd() {
  console.log('Extracted!')
}
    
var downloadAndRun = function(filename, type, folder, url) {
			var tmpFilePath = filename + "." + type;
			https.get(url, function(response) {
		 		response.on('data', function (data) {
		 			fs.appendFileSync(tmpFilePath, data)
				});
		 		response.on('end', function() {
		 			 if(type == 'zip')
                     {
                        var zip = new AdmZip(tmpFilePath);
                        zip.extractAllTo("./");
                     }
                     else if(type == "tar.gz")
                     {
                        var extractor = tar.Extract({path: "./"})
                            .on('error', onError)
                            .on('end', onEnd);

                        fs.createReadStream(tmpFilePath)
                          .on('error', onError)
                          .pipe(extractor);
                     }
                     else 
                     {
                         console.error("Error: Invalid Type");
                     }
		 			 fs.unlink(tmpFilePath);
                     fs.renameSync('./' + folder, './cmake_binary');
                     
                     var current_dir = __dirname;
                     
                     var command = '';
                     if(process.platform == 'win32')
                         command = 'cmake-js -c ' + current_dir + '\\cmake_binary\\bin\\cmake.exe rebuild';
                     else command = command = 'cmake-js -c ' + current_dir + '\\cmake_binary\\bin\\cmake rebuild';
                     
                     console.log("Running command: " + command);
                     
                     child = exec(command, function (error, stdout, stderr) {
                                                                            console.log('stdout: ' + stdout);
                                                                            console.log('stderr: ' + stderr);
                                                                            if (error !== null) {
                                                                                console.log('exec error: ' + error);
                                                                            }
                                                                        });

			 	})
		 	});
		}

var toDownload = false;        
try
{
    var stats = fs.lstatSync('./cmake_binary');
    if(!stats.isDirectory())
        download = true;
}
catch(e)
{
    toDownload = true;
}

if (toDownload) 
{
    console.log("Downloading cmake for platform: " + process.platform);
    var folder = '';
    if(process.platform == 'win32')        
    {
        downloadAndRun('cmake_binary', 'zip', 'cmake-3.3.2-win32-x86', 'https://cmake.org/files/v3.3/cmake-3.3.2-win32-x86.zip');
    }
    else
    {
        downloadAndRun('cmake_binary', 'tar.gz', 'cmake-3.3.2-Linux-x86_64', 'https://cmake.org/files/v3.3/cmake-3.3.2-Linux-x86_64.tar.gz');
    }
}
else 
{
    console.log("Cmake binaries already exists.");
    var current_dir = __dirname;
    var command = '';
                     if(process.platform == 'win32')
                         command = 'cmake-js -c ' + current_dir + '\\cmake_binary\\bin\\cmake.exe rebuild';
                     else command = command = 'cmake-js -c ' + current_dir + '\\cmake_binary\\bin\\cmake rebuild';
                     
                     console.log("Running command: " + command);
                     
                     child = exec(command, function (error, stdout, stderr) {
                                                                            console.log('stdout: ' + stdout);
                                                                            console.log('stderr: ' + stderr);
                                                                            if (error !== null) {
                                                                                console.log('exec error: ' + error);
                                                                            }
                                                                        });
}