(function() {

    var models = require("test/models");

    var DetailViewModel = function() {
        this.init();
    };

    DetailViewModel.prototype = new unify.ViewModel();
    DetailViewModel.prototype.constructor = DetailViewModel;


    DetailViewModel.prototype.makeDetailInfo = function(id) {
        var data = {
            id: 1,
            name: "N7100",
            description: "Phone",
        };

        print("makeDetailInfo:", JSON.stringify(data));
        this.notify("DETAIL_INFO_CHANGED", data);
    };





    this.DetailViewModel = DetailViewModel;

}.call(this));