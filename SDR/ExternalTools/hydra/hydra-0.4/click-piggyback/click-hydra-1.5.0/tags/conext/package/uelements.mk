# Generated by 'click-buildtool elem2make' on Mon Sep 15 13:55:21 CDT 2008
ELEMENT_OBJS__0 = \
activemactx.uo \
aggsetcrc32.uo \
bebpolicyarf.uo \
bebpolicy.uo \
calculaterssi.uo \
carriersense.uo \
checkcrcber.uo \
collisionpolicy.uo \
dcfscheduler1.uo \
dcfscheduler2.uo \
dcfscheduler.uo \
dcftransmitter.uo \
deaggregator.uo \
firstframe.uo \
fromlabviewphy1.uo \
fromlabviewphy.uo \
fromphy.uo \
grphy.uo \
hydradecap.uo \
hydraencap.uo \
hydrafromsocket.uo \
hydratosocket.uo \
lastframe.uo \
logtcp.uo \
macframe80211.uo \
macframeack.uo \
macframeackrbar.uo \
macframe.uo \
macframects.uo \
macframectsrbar.uo \
macframectsrssi.uo \
macframedata.uo \
macframedatarbar.uo \
macframerts.uo \
macframertsrbar.uo \
macnotifier.uo \
mactimer.uo \
mactransmitter.uo \
mactxscheduler.uo \
mpivector.uo \
passcsi.uo \
phyprocesstime1.uo \
phyprocesstime.uo \
powercontrol.uo \
powercontrolconst.uo \
powercontrolscen.uo \
prioritizer.uo \
probe.uo \
pullpusher.uo \
ratecontrolarf.uo \
ratecontrol.uo \
ratecontrolconst.uo \
ratecontrolrbar.uo \
ratecontrolrbarth.uo \
ratecontrolrssi.uo \
ratecontroltbar.uo \
rxstate.uo \
rxvector.uo \
rxvectorgr.uo \
rxvectorgrnew.uo \
setcrcber.uo \
setdefer.uo \
setduration1.uo \
setduration.uo \
setdurationrbar.uo \
setpower.uo \
setpowerscen.uo \
setratecontrol.uo \
setrtsthreshold.uo \
simchannel.uo \
simchannelper.uo \
simgrphy.uo \
simgrphynew.uo \
simphy1.uo \
simphy.uo \
staticroutetable.uo \
tolabviewphy1.uo \
tolabviewphy.uo \
tophy.uo \
txvector.uo \
txvectorgr.uo \
txvectorgrnew.uo \
ubaggregator1.uo \
ubaggregator2.uo \
ubaggregator.uo \
virtualcarriersense.uo \
virtualcarriersenserbar.uo \

ELEMENT_OBJS = \
$(ELEMENT_OBJS__0) \

$(ELEMENT_OBJS__0): %.uo: ./%.cc
	$(call cxxcompile,-c $< -o $@,CXX)
	$(FIXDEP)
