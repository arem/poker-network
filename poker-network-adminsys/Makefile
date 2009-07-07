check: checkserverinfo

checkserverinfo:
	echo 'DROP DATABASE IF EXISTS  testserverinfo' | mysql -u root
	echo 'CREATE DATABASE testserverinfo' | mysql -u root
	mysql -u root testserverinfo < resthost.sql
	mysql -u root testserverinfo < route.sql
	mysql -u root testserverinfo < user2table.sql
	test "$(shell make HOST=poker1 DATABASE=testserverinfo -f server-info.mk serials | xargs)" = "1 3"
	test "$(shell make HOST=poker1 DATABASE=testserverinfo -f server-info.mk ports | xargs)" = "19380 20380"
	test "$(shell make HOST=poker1 DATABASE=testserverinfo -f server-info.mk tables | xargs)" = "282245 0 1246986544 1 282243 0 1246985872 1 282241 0 1246986540 1 282239 0 1246986540 1 282237 0 1246986540 1 282235 0 1246986530 1 282233 0 1246986541 1 282231 0 1246986543 1 282229 0 1246986542 1 282227 0 1246986544 1 282225 0 1246986539 1 282223 0 1246986544 1 282221 0 1246986544 1 282271 0 1246984666 3 282269 0 1246973874 3 282267 0 1246982583 3 282265 0 1246971192 3 282263 0 1246976580 3 282261 0 1246984467 3 282259 0 1246985185 3 282257 0 1246975884 3 282255 0 1246985777 3 282253 0 1246984649 3 282251 0 1246985495 3 282249 0 1246982341 3 282247 0 1246986543 3"
	test "$(shell make HOST=poker1 DATABASE=testserverinfo -f server-info.mk players | xargs)" = "95 8"
	test "$(shell make HOST=poker1 DATABASE=testserverinfo CONFDIR=conf -f server-info.mk servers | xargs)" = "conf/poker.server1.xml: <resthost host=poker1 port=19384 path=/POKER_REST /> conf/poker.server2.xml: <resthost host=poker2 port=19384 path=/POKER_REST /> conf/poker.server3.xml: <resthost host=poker2 port=20384 path=/POKER_REST /> conf/poker.server4.xml: <resthost host=poker2 port=20384 path=/POKER_REST />"
