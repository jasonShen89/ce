(function() {
    
    var Model = function() {};

    Model.prototype.notify = function(name, body) {
        unify.Notification.notify(name, body);
    };


    this.Model = Model;

}.call(this));