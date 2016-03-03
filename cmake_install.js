var AdmZip = require('adm-zip');
var https = require('https');
var fs = require('fs');
var tar = require('tar');
var child_process = require('child_process');
var http = require('http');

function onError(err) {
    console.error('An error occurred:', err)
}

function runCmakeJS(cmakeJSFile, cmakeFile, slash)
{
    const ls = child_process.spawn(cmakeJSFile, ['-c', __dirname + slash + 'cmake_binary' + slash + 'bin' + slash + cmakeFile, 'rebuild']);
                     
    ls.on("exit", function(exitCode) {
        console.log('process exited with code ' + exitCode);
    });

    ls.stdout.on("data", function(chunk) {
        console.log('received stdout chunk ' + chunk);
    });

    ls.stderr.on("data", function(chunk) {
        console.log('received stderr chunk ' + chunk);
    });

    ls.stdout.on("end", function() {
        console.log("finished collecting data chunks from stdout");
    });
}
    
function downloadAndRun(filename, type, folder, url) 
{
    var tmpFilePath = filename + "." + type;
  
    console.log(url);
  
    https.get(url, function(response) {
        response.on('data', function (data) {
            fs.appendFileSync(tmpFilePath, data)
        });

        response.on('end', function() {
            if(type == 'zip')
            {
                var zip = new AdmZip(tmpFilePath);
                zip.extractAllTo("./");
                fs.unlink(tmpFilePath);
                fs.renameSync('./' + folder, './cmake_binary');
                runCmakeJS('cmake-js', 'cmake.exe', '\\');
            }
            else if(type == "tar.gz")
            {
                const ls = child_process.spawn('tar', ['-xzf', tmpFilePath]);
                ls.stdout.on("end", function() {
                    fs.unlink(tmpFilePath);
                    fs.renameSync('./' + folder, './cmake_binary');
                    runCmakeJS('cmake-js', 'cmake', '/');
                });
            }
            else 
            {
                console.error("Error: Invalid Type");
            }
        });

    })
};

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
    
    if(process.platform == 'win32')
    {
        runCmakeJS('.\\node_modules\\.bin\\cmake-js.cmd', 'cmake.exe', '\\');
    }
    else runCmakeJS('./node_modules/cmake-js/bin/cmake-js', 'cmake', '/');
}
