// function require(name) {
//     var cache = require.cache;
//     if (name in cache) return cache[name];
//     var exports = {name:name};
//     cache[name] = exports;
//     print("name:" + exports.name);
//     Function(io.read(name+'.js')).call(exports);
//     return exports;
// }
// require.cache = Object.create(null);


// Duktape.modSearch = function(name) {
//     var cache = Duktape.modSearch.cache;
//     if (name in cache) return cache[name];
//     var src = read(name+'.js');
//     cache[name] = src;
//     return src;
// };
// Duktape.modSearch.cache = Object.create(null);


var _ = require("unify/libs/lodash")._;
require("unify/init");


function main() {

    this.globalFunction = function(name) {
        var result = name + "::" + name;
        print("JS: ", result);
        return result;
    };


    function testDate() {
        print("===== testDate =====");
        print("CURRENT DATE1: " + Date.now());
        var date = new Date();
        print("CURRENT DATE2: " + date.getTime());
    }


    function testRequire() {
        print("===== testRequire =====");
        try {
            var f = require("inc");
            print("TESTREQUIRE: " + f.addFunc(1, 2));    
        } catch(e) {
            print(e.stack);
        }
    }


    function testJSON() {
        print("===== testJSON =====");
        var json = '{"array":[1,2,3],"boolean":true,"null":null,"number":123,"object":{"a":"b","c":"d","e":"f"},"string":"Hello World"}';
        var obj = JSON.parse(json);
        print("TESTJSON: " + obj.string);
    }


    function testRegular() {
        print("===== testRegular =====");
        var json = '{"array":[1,2,3],"boolean":true,"null":null,"number":123,"object":{"a":"b","c":"d","e":"f"},"string":"Hello World"}';
        var q = /(\d+)/g;
        var r = json.match(q);
        print("MATCH RESULT: " + r);
    }


    function testUnderScore() {
        print("===== testUnderScore =====");
        var _ = require("unify/libs/underscore")._;
        _.each(["A", "B", "C"], function(v) {
            print("ITEM: " + v);
        });
    }


    function testLodash() {
        print("===== testLodash =====");
        var _ = require("unify/libs/lodash")._;
        var a = _.assign({ 'a': 1 }, { 'b': 2 }, { 'c': 3 });
        var b = _.map([1, 2, 3], function(n) { return n * 3; });
        print(JSON.stringify(a));
        print(JSON.stringify(b));
    }


    function testRead() {
        print("===== testRead =====");
        print("READ: " + io.read("test.txt"));
    }


    function testReadFromZip() {
        print("READFROMZIP: " + io.readFromZip("zip.zip", "inc.js"));
    }


    function testWrite() {
        print("===== testWrite =====");
        print("WRITE: " + io.write("w.txt", "321中文abcDEF"));

        // var f = File.open("test.txt", "w");
        // f.write("www");
        // f.close();
    }


    /**
    call C function and C callback to JavaScript
    */
    function testMyCallback() {
        //testCallback(function(arg){ print("closure function!", arg); });
        //testfunc();
        
        try {
            testCallback(function(arg) {
                print("arg: " + arg);
                return 11;
            });
        } catch(e) {
            print(e.stack);
        }
    }


    function testPerformance() {
        print("===== testPerformance =====");
        print("-- A: " + Date.now());
        var c = 0;
        for (var i = 0; i < 1000000; i++) { c += i; }
        print("-- B: " + Date.now());

    }


    function testCrypto() {
        print("MD5(123456): " + Crypto.md5_hex("123456"));
        print("BASE64(123456): " + Crypto.base64_encode("123456"));
    }


    function testPlatformHelper()
    {
        print("LANGUAGE: " + PlatformHelper.getCurrentLanguage());
        print("PLATFORM: " + PlatformHelper.getTargetPlatform());
    }


    function testTimer()
    {
        var count = 0;
        var timer1 = setInterval(function() {
            count++;
            print("timer interval ...", count);

            if (count == 10) {
                clearInterval(timer1);
            }
        }, 300);

        var timer2 = setTimeout(function() {
            print("setTimeout_1...");
        }, 1000);

        setTimeout(function() {
            print("setTimeout_2...");
            clearTimeout(timer2);
        }, 700);
    }


    function testHttpRequest()
    {
        print("HTTP-GET");
        http.get("http://httpbin.org/ip", function(statusCode, response) {
            print("HTTP GET:", statusCode, response);
        });


        print("HTTP-DOWNLOAD");
        http.get("https://www.baidu.com/img/baidu_jgylogo3.gif", function(statusCode, response) {
            var file = unify.device.writeablePath + "test.gif";
            print("download file:", statusCode, file);
            io.write(file, response);
        });
        
        
        print("HTTP-POST");
        http.post("http://user.outer.pagewar.com/siteuser/user",
            {
                cmd: "login",
                email: "kaixuan1901",
                password: "123456",
                mac: "88888888"
            },
            function(statusCode, response) {
                print("HTTP POST:", statusCode, response);
                var data = JSON.parse(response);
                print(data.message);
            }
        );
    }



    this.gf = function(name) {
        return name + ";" + name;
    };


    this.gf2 = function() {
        print("js: gf2");
        // return { value: 10 };
    };


    // testDate();
    // testRead();
    // testReadFromZip();
    // testWrite();
    // testRequire();
    // testMyCallback();
    // testJSON();
    // testRegular();
    // testUnderScore();
    // testLodash();
    // testPerformance();
    // testCrypto();
    // testPlatformHelper();
    // testTimer();
    testHttpRequest();



    function ggg(arg, func) {
        if (arg == 1) {
            func("一");
        } else if (arg == 2) {
            func("二");
        }
    }
}


try {
    main.call(this.global);
} catch(e) {
    print(e.stack);
}
