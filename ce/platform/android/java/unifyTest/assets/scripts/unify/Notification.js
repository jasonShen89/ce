(function() {

    var Notification = this;
    Notification.observers = {};

    Notification.addObserver = function(name, observer) {
        this.observers[name] = this.observers[name] || [];
        this.observers[name].push({
            name: name,
            observer: observer,
        });
    };


    Notification.removeObserver = function(name, observer) {
        var obs = this.observers[name];
        if (obs && obs.length > 0) {
            var i, delIndexs = [];
            for (i = 0; i < obs.length; i++) {
                if (obs[i].observer == observer) {
                    delIndexs.push(i);
                }
            }

            for (i = 0; i < delIndexs.length; i++) {
                obs.splice(delIndexs, 1);
            }
        }
    };


    Notification.notify = function(name, body) {
        var obs = this.observers[name];
        if (obs && obs.length > 0) {
            for (var i in obs) {
                var observer = obs[i].observer;
                observer.handleNotification({ name : name , body: body });
            }    
        }
    };


}.call(this));