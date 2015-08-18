(function() {

    var AppProxy = function() {
        this.name = "AppProxy";
    };

    AppProxy.prototype.init = function(arg) {
        print("on AppProxy init:", arg);
    };


    this.AppProxy = AppProxy;

}.call(this));
