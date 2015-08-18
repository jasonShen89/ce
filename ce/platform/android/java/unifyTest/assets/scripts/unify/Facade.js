(function() {

    var Notification = require("unify/Notification");
    
    var Facade = this;
    Facade.proxys = {};
    
    Facade.registerProxy = function(proxy) {
        if (!this.retrieveProxy(proxy.name)) {
            this.proxys[proxy.name] = proxy;      
        } else {
            print("Can't register proxy, (" + proxy.name + ") is registered! ");
        }
    };


    Facade.unregisterProxy = function(proxy) {
        if (!proxy || !this.retrieveProxy(proxy.name)) return;

        if (typeof proxy.cleanup == "function") { proxy.cleanup(); }
        delete this.proxys[proxy.name];
    };


    Facade.retrieveProxy = function(proxyName) {
        return this.proxys[proxyName];
    };

}).call(this);