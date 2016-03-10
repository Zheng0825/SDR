#!/bin/sh

sh hopLinePingTest.sh
espeak "Ping Tests Complete"
sh smallerPacketLinePingTest.sh
espeak "smaller packet test complete"
sh TTLLinePingTest.sh
espeak "TTL Change Complete"
espeak "All Tests are Complete, please alert John, Joe, or Brad"
