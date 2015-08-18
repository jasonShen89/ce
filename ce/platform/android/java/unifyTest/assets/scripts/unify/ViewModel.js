(function() {

    var ViewModel = function() {
    };


    ViewModel.prototype.init = function() {
        var interests = this.listNotificationInterests();
        for (var i in interests) {
            unify.Notification.addObserver(interests[i], this);
        }
    };


    /**
    notify to native
    @param name notice name
    @param body notice data
    */
    ViewModel.prototype.notify = function(name, body) {
        // var NC = NotificationCenter.getInstance();
        // NotificationCenter.notify(NC, name, body);

        notify.notify(name, body);
    };


    ViewModel.prototype.cleanup = function() {
        var interests = this.listNotificationInterests();
        for (var i in interests) {
            unify.Notification.removeObserver(interests[i], this);
        }
    };


    ViewModel.prototype.listNotificationInterests = function() {
        return [];
    };


    ViewModel.prototype.handleNotification = function(notification) {
        print("receive notification:", notification.name);
    };


    this.ViewModel = ViewModel;

}.call(this));