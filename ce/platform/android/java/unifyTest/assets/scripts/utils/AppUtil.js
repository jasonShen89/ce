(function() {

    var AppUtil = this;

    this.checkEmail = function(email) {
        var re = /^([\w-]+(?:\.[\w-]+)*)@((?:[\w-]+\.)*\w[\w-]{0,66})\.([a-z]{2,6}(?:\.[a-z]{2})?)$/i;
        var ok = re.test(email);
        return {
            ok: ok,
            message: ok ? "OK" : "邮箱地址格式错误！",
        };
    };

}.call(this));
