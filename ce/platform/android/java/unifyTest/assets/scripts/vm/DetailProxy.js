(function() {

    var AppUtil = require("utils/AppUtil");
    var DetailViewModel = require("vm/DetailViewModel").DetailViewModel;


    var DetailProxy = function() {
        this.name = "DetailProxy";
        this.detailViewModel = new DetailViewModel();
    };


    DetailProxy.prototype.detailInfo = function(id) {
        this.detailViewModel.makeDetailInfo(id);
    };


    this.DetailProxy = DetailProxy;

}.call(this));
