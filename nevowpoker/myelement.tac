from nevow import appserver
from myelement import FakeRoot
site = appserver.NevowSite(FakeRoot())
from twisted.application import service, internet
application = service.Application("Athena Demo")
webService = internet.TCPServer(8080, site)
webService.setServiceParent(application)
