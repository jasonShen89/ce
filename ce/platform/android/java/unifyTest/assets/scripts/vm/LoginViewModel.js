(function() {

    var models = require("test/models");

    var LoginViewModel = function() {
        this.init();
    };

    LoginViewModel.prototype = new unify.ViewModel();
    LoginViewModel.prototype.constructor = LoginViewModel;


    LoginViewModel.prototype.login = function(userName, password) {
        // login...
        var data =  new models.LoginInfo();
        data.userId = 1;
        data.userName = "UTF8字符串";
        data.notify("LOGIN_INFO_CHANGED", data);
    };


    LoginViewModel.prototype.resultHandler = function(data) {
        var result = data;
        this.notify("LOGIN_VM_CHANGED", result);
    };


    LoginViewModel.prototype.listNotificationInterests = function() {
        return [
            "LOGIN_INFO_CHANGED",
        ];
    };

    LoginViewModel.prototype.handleNotification = function(notification) {
        if (notification.name == "LOGIN_INFO_CHANGED") {
            var loginInfo = notification.body;
            loginInfo.userId = loginInfo.userId + 1000;
            this.resultHandler(loginInfo);
        }
    };




    this.LoginViewModel = LoginViewModel;

}.call(this));