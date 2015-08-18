(function() {

    /**
    LoginInfo
    */
    var LoginInfo = function() {
        this.userId = 0;
        this.userName = "";
    };
    LoginInfo.prototype = new unify.Model();
    LoginInfo.prototype.constructor = this.LoginInfo;



    /**
    ProductDetail
    */
    var ProductDetail = function() {
        this.id = 0;
        this.name = "";
        this.description = "";
    };
    ProductDetail.prototype = new unify.Model();
    ProductDetail.prototype.constructor = this.ProductDetail;



    /**
    exports
    */
    this.LoginInfo = LoginInfo;
    this.ProductDetail = ProductDetail;

}.call(this));

