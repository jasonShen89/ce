require("unify/init");

var _ = require("unify/libs/lodash")._;
var AppProxy = require("vm/AppProxy").AppProxy;
var LoginProxy = require("vm/LoginProxy").LoginProxy;
var DetailProxy = require("vm/DetailProxy").DetailProxy;


function main() {

    unify.Facade.registerProxy(new AppProxy());
    unify.Facade.registerProxy(new LoginProxy());
    unify.Facade.registerProxy(new DetailProxy());

    this.globalFunction = function(name) {
        var result = name + "::" + name;
        print("JS: ", result);
        return result;
    };
    
    // 提供给Native层调用Proxy的公用方法
    this.callProxy = function(proxyName, funcName, $) {
        var proxy = unify.Facade.retrieveProxy(proxyName);
        if (proxy && funcName in proxy) {
            return proxy[funcName].apply(proxy, _.slice(arguments, 2));
        } else {
            print("Proxy or function not found!", proxyName, funcName);
        }
    };
}


try {
    main.call(this.global);
} catch(e) {
    print(e.stack);
}