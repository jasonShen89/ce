(function() {

    var AppUtil = require("utils/AppUtil");
    var LoginViewModel = require("vm/LoginViewModel").LoginViewModel;


    var LoginProxy = function() {
        this.name = "LoginProxy";
        this.loginViewModel = new LoginViewModel();
    };


    LoginProxy.prototype.cleanup = function() {
        this.loginViewModel.cleanup();
    };

    LoginProxy.prototype.checkEmail = function(email) {
        return AppUtil.checkEmail(email);
    };

    LoginProxy.prototype.login = function(userName, password) {
        this.loginViewModel.login(userName, password);
    };


    this.LoginProxy = LoginProxy;

}.call(this));
