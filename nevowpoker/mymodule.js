// import Nevow.Athena

MyModule = {};

MyModule.MyWidget = Nevow.Athena.Widget.subclass('MyModule.MyWidget');

MyModule.MyWidget.method(
    'echo',
    function(self, argument) {
        alert("Echoing " + argument);
        return argument;
    });

MyModule.MyWidget.method(
    'clicked',
    function(self) {
        self.callRemote('echo', 'hello, world');
    });
