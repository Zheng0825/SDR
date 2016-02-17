<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="14" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="53" name="tGND_GNDA" color="7" fill="9" visible="no" active="no"/>
<layer number="54" name="bGND_GNDA" color="1" fill="9" visible="no" active="no"/>
<layer number="56" name="wert" color="7" fill="1" visible="no" active="no"/>
<layer number="57" name="tCAD" color="7" fill="1" visible="no" active="no"/>
<layer number="59" name="tCarbon" color="7" fill="1" visible="no" active="no"/>
<layer number="60" name="bCarbon" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="7" fill="1" visible="yes" active="yes"/>
<layer number="100" name="Muster" color="7" fill="1" visible="no" active="no"/>
<layer number="101" name="Patch_Top" color="12" fill="4" visible="yes" active="yes"/>
<layer number="102" name="Vscore" color="7" fill="1" visible="yes" active="yes"/>
<layer number="103" name="tMap" color="7" fill="1" visible="yes" active="yes"/>
<layer number="104" name="Name" color="16" fill="1" visible="yes" active="yes"/>
<layer number="105" name="tPlate" color="7" fill="1" visible="yes" active="yes"/>
<layer number="106" name="bPlate" color="7" fill="1" visible="yes" active="yes"/>
<layer number="107" name="Crop" color="7" fill="1" visible="yes" active="yes"/>
<layer number="108" name="fp8" color="7" fill="1" visible="yes" active="yes"/>
<layer number="109" name="fp9" color="7" fill="1" visible="yes" active="yes"/>
<layer number="110" name="fp0" color="7" fill="1" visible="yes" active="yes"/>
<layer number="111" name="LPC17xx" color="7" fill="1" visible="yes" active="yes"/>
<layer number="112" name="tSilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="113" name="IDFDebug" color="4" fill="1" visible="no" active="yes"/>
<layer number="116" name="Patch_BOT" color="9" fill="4" visible="yes" active="yes"/>
<layer number="118" name="Rect_Pads" color="7" fill="1" visible="yes" active="yes"/>
<layer number="121" name="_tsilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="122" name="_bsilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="123" name="tTestmark" color="7" fill="1" visible="yes" active="yes"/>
<layer number="124" name="bTestmark" color="7" fill="1" visible="yes" active="yes"/>
<layer number="125" name="_tNames" color="7" fill="1" visible="yes" active="yes"/>
<layer number="126" name="_bNames" color="7" fill="1" visible="yes" active="yes"/>
<layer number="127" name="_tValues" color="7" fill="1" visible="yes" active="yes"/>
<layer number="128" name="_bValues" color="7" fill="1" visible="yes" active="yes"/>
<layer number="129" name="Mask" color="7" fill="1" visible="yes" active="yes"/>
<layer number="131" name="tAdjust" color="7" fill="1" visible="yes" active="yes"/>
<layer number="132" name="bAdjust" color="7" fill="1" visible="yes" active="yes"/>
<layer number="144" name="Drill_legend" color="7" fill="1" visible="yes" active="yes"/>
<layer number="150" name="Notes" color="7" fill="1" visible="no" active="yes"/>
<layer number="151" name="HeatSink" color="7" fill="1" visible="yes" active="yes"/>
<layer number="152" name="_bDocu" color="7" fill="1" visible="yes" active="yes"/>
<layer number="153" name="FabDoc1" color="7" fill="1" visible="no" active="yes"/>
<layer number="154" name="FabDoc2" color="7" fill="1" visible="no" active="yes"/>
<layer number="155" name="FabDoc3" color="7" fill="1" visible="no" active="yes"/>
<layer number="199" name="Contour" color="7" fill="1" visible="yes" active="yes"/>
<layer number="200" name="200bmp" color="1" fill="10" visible="yes" active="yes"/>
<layer number="201" name="201bmp" color="2" fill="10" visible="yes" active="yes"/>
<layer number="202" name="202bmp" color="3" fill="10" visible="yes" active="yes"/>
<layer number="203" name="203bmp" color="4" fill="10" visible="yes" active="yes"/>
<layer number="204" name="204bmp" color="5" fill="10" visible="yes" active="yes"/>
<layer number="205" name="205bmp" color="6" fill="10" visible="yes" active="yes"/>
<layer number="206" name="206bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="207" name="207bmp" color="8" fill="10" visible="yes" active="yes"/>
<layer number="208" name="208bmp" color="9" fill="10" visible="yes" active="yes"/>
<layer number="209" name="209bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="210" name="210bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="211" name="211bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="212" name="212bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="213" name="213bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="214" name="214bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="215" name="215bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="216" name="216bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="217" name="217bmp" color="18" fill="1" visible="no" active="no"/>
<layer number="218" name="218bmp" color="19" fill="1" visible="no" active="no"/>
<layer number="219" name="219bmp" color="20" fill="1" visible="no" active="no"/>
<layer number="220" name="220bmp" color="21" fill="1" visible="no" active="no"/>
<layer number="221" name="221bmp" color="22" fill="1" visible="no" active="no"/>
<layer number="222" name="222bmp" color="23" fill="1" visible="no" active="no"/>
<layer number="223" name="223bmp" color="24" fill="1" visible="no" active="no"/>
<layer number="224" name="224bmp" color="25" fill="1" visible="no" active="no"/>
<layer number="225" name="225bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="226" name="226bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="227" name="227bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="228" name="228bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="229" name="229bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="230" name="230bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="231" name="231bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="248" name="Housing" color="7" fill="1" visible="yes" active="yes"/>
<layer number="249" name="Edge" color="7" fill="1" visible="yes" active="yes"/>
<layer number="250" name="Descript" color="3" fill="1" visible="no" active="no"/>
<layer number="251" name="SMDround" color="12" fill="11" visible="no" active="no"/>
<layer number="254" name="cooling" color="7" fill="1" visible="yes" active="yes"/>
<layer number="255" name="routoute" color="7" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="GeekAmmo">
<description>&lt;h3&gt;GeekAmmo  Library&lt;/h3&gt;

These are parts used by the GeekAmmo company (now part of SparkFun Electronics)!

 If you enjoy using this library, please buy one of our products at www.sparkfun.com.
&lt;br&gt;&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt;Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/
&lt;br&gt;&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
</packages>
<symbols>
<symbol name="DOCFIELD1">
<wire x1="-48.26" y1="-20.32" x2="22.86" y2="-20.32" width="0.1016" layer="94"/>
<wire x1="53.34" y1="-5.08" x2="39.37" y2="-5.08" width="0.1016" layer="94"/>
<wire x1="-48.26" y1="-20.32" x2="-48.26" y2="-15.24" width="0.1016" layer="94"/>
<wire x1="-48.26" y1="-15.24" x2="22.86" y2="-15.24" width="0.1016" layer="94"/>
<wire x1="-48.26" y1="-15.24" x2="-48.26" y2="-5.08" width="0.1016" layer="94"/>
<wire x1="53.34" y1="-5.08" x2="53.34" y2="-15.24" width="0.1016" layer="94"/>
<wire x1="22.86" y1="-15.24" x2="22.86" y2="-20.32" width="0.1016" layer="94"/>
<wire x1="22.86" y1="-15.24" x2="39.37" y2="-15.24" width="0.1016" layer="94"/>
<wire x1="22.86" y1="-20.32" x2="53.34" y2="-20.32" width="0.1016" layer="94"/>
<wire x1="39.37" y1="-5.08" x2="39.37" y2="-15.24" width="0.1016" layer="94"/>
<wire x1="39.37" y1="-5.08" x2="-48.26" y2="-5.08" width="0.1016" layer="94"/>
<wire x1="39.37" y1="-15.24" x2="53.34" y2="-15.24" width="0.1016" layer="94"/>
<wire x1="53.34" y1="-15.24" x2="53.34" y2="-20.32" width="0.1016" layer="94"/>
<wire x1="-48.26" y1="-5.08" x2="-48.26" y2="2.54" width="0.1016" layer="94"/>
<wire x1="53.34" y1="15.24" x2="-48.26" y2="15.24" width="0.1016" layer="94"/>
<wire x1="53.34" y1="15.24" x2="53.34" y2="2.54" width="0.1016" layer="94"/>
<wire x1="-48.26" y1="2.54" x2="53.34" y2="2.54" width="0.1016" layer="94"/>
<wire x1="-48.26" y1="2.54" x2="-48.26" y2="15.24" width="0.1016" layer="94"/>
<wire x1="53.34" y1="2.54" x2="53.34" y2="-5.08" width="0.1016" layer="94"/>
<text x="-46.99" y="-19.05" size="2.54" layer="94" font="vector">Date:</text>
<text x="-35.56" y="-19.05" size="2.54" layer="94" font="vector">&gt;LAST_DATE_TIME</text>
<text x="24.13" y="-19.05" size="2.54" layer="94" font="vector">Sheet:</text>
<text x="38.1" y="-19.05" size="2.54" layer="94" font="vector">&gt;SHEET</text>
<text x="40.64" y="-8.89" size="2.54" layer="94" font="vector">REV:</text>
<text x="-45.72" y="-2.54" size="2.54" layer="94" font="vector">SKU :</text>
<text x="-45.72" y="-8.636" size="2.54" layer="94" font="vector">Document Location :</text>
<text x="-33.02" y="-2.54" size="2.54" layer="94" font="vector">&gt;DRAWING_NAME</text>
<text x="40.64" y="-12.7" size="2.1844" layer="94">&gt;REV</text>
<text x="-45.72" y="-12.7" size="1.778" layer="94">&gt;URL</text>
<rectangle x1="-40.5117" y1="3.6894" x2="-40.2272" y2="3.7072" layer="94"/>
<rectangle x1="-40.5472" y1="3.7072" x2="-40.1738" y2="3.725" layer="94"/>
<rectangle x1="-40.5472" y1="3.725" x2="-40.1383" y2="3.7427" layer="94"/>
<rectangle x1="-40.5472" y1="3.7427" x2="-40.1205" y2="3.7605" layer="94"/>
<rectangle x1="-40.5472" y1="3.7605" x2="-40.1027" y2="3.7783" layer="94"/>
<rectangle x1="-40.5472" y1="3.7783" x2="-40.1027" y2="3.7961" layer="94"/>
<rectangle x1="-40.5472" y1="3.7961" x2="-40.4939" y2="3.8139" layer="94"/>
<rectangle x1="-40.2805" y1="3.7961" x2="-40.0849" y2="3.8139" layer="94"/>
<rectangle x1="-40.2627" y1="3.8139" x2="-40.0849" y2="3.8316" layer="94"/>
<rectangle x1="-40.245" y1="3.8316" x2="-40.0849" y2="3.8494" layer="94"/>
<rectangle x1="-40.2272" y1="3.8494" x2="-40.0849" y2="3.8672" layer="94"/>
<rectangle x1="-40.2272" y1="3.8672" x2="-40.0849" y2="3.885" layer="94"/>
<rectangle x1="-40.405" y1="3.885" x2="-40.2805" y2="3.9028" layer="94"/>
<rectangle x1="-40.2094" y1="3.885" x2="-40.0849" y2="3.9028" layer="94"/>
<rectangle x1="-39.8182" y1="3.885" x2="-39.516" y2="3.9028" layer="94"/>
<rectangle x1="-39.2848" y1="3.885" x2="-38.9648" y2="3.9028" layer="94"/>
<rectangle x1="-38.8759" y1="3.885" x2="-38.7337" y2="3.9028" layer="94"/>
<rectangle x1="-38.5025" y1="3.885" x2="-38.3425" y2="3.9028" layer="94"/>
<rectangle x1="-38.1647" y1="3.885" x2="-37.9513" y2="3.9028" layer="94"/>
<rectangle x1="-37.8802" y1="3.885" x2="-37.8091" y2="3.9028" layer="94"/>
<rectangle x1="-37.6669" y1="3.885" x2="-37.5602" y2="3.9028" layer="94"/>
<rectangle x1="-37.329" y1="3.885" x2="-37.2224" y2="3.9028" layer="94"/>
<rectangle x1="-36.9734" y1="3.885" x2="-36.8845" y2="3.9028" layer="94"/>
<rectangle x1="-36.7423" y1="3.885" x2="-36.6534" y2="3.9028" layer="94"/>
<rectangle x1="-36.4045" y1="3.885" x2="-36.2978" y2="3.9028" layer="94"/>
<rectangle x1="-36.0667" y1="3.885" x2="-35.96" y2="3.9028" layer="94"/>
<rectangle x1="-35.6933" y1="3.885" x2="-35.4799" y2="3.9028" layer="94"/>
<rectangle x1="-40.4761" y1="3.9028" x2="-40.0849" y2="3.9205" layer="94"/>
<rectangle x1="-39.8716" y1="3.9028" x2="-39.516" y2="3.9205" layer="94"/>
<rectangle x1="-39.3204" y1="3.9028" x2="-38.9648" y2="3.9205" layer="94"/>
<rectangle x1="-38.8759" y1="3.9028" x2="-38.7337" y2="3.9205" layer="94"/>
<rectangle x1="-38.5203" y1="3.9028" x2="-38.3425" y2="3.9205" layer="94"/>
<rectangle x1="-38.2003" y1="3.9028" x2="-37.8091" y2="3.9205" layer="94"/>
<rectangle x1="-37.6669" y1="3.9028" x2="-37.5602" y2="3.9205" layer="94"/>
<rectangle x1="-37.329" y1="3.9028" x2="-37.2224" y2="3.9205" layer="94"/>
<rectangle x1="-36.9734" y1="3.9028" x2="-36.8845" y2="3.9205" layer="94"/>
<rectangle x1="-36.7423" y1="3.9028" x2="-36.6534" y2="3.9205" layer="94"/>
<rectangle x1="-36.4045" y1="3.9028" x2="-36.2978" y2="3.9205" layer="94"/>
<rectangle x1="-36.0667" y1="3.9028" x2="-35.96" y2="3.9205" layer="94"/>
<rectangle x1="-35.7466" y1="3.9028" x2="-35.4444" y2="3.9205" layer="94"/>
<rectangle x1="-40.5117" y1="3.9205" x2="-40.0849" y2="3.9383" layer="94"/>
<rectangle x1="-39.9071" y1="3.9205" x2="-39.516" y2="3.9383" layer="94"/>
<rectangle x1="-39.356" y1="3.9205" x2="-38.9648" y2="3.9383" layer="94"/>
<rectangle x1="-38.8759" y1="3.9205" x2="-38.7337" y2="3.9383" layer="94"/>
<rectangle x1="-38.5203" y1="3.9205" x2="-38.3603" y2="3.9383" layer="94"/>
<rectangle x1="-38.218" y1="3.9205" x2="-37.8091" y2="3.9383" layer="94"/>
<rectangle x1="-37.6669" y1="3.9205" x2="-37.5602" y2="3.9383" layer="94"/>
<rectangle x1="-37.329" y1="3.9205" x2="-37.2224" y2="3.9383" layer="94"/>
<rectangle x1="-36.9734" y1="3.9205" x2="-36.8845" y2="3.9383" layer="94"/>
<rectangle x1="-36.7423" y1="3.9205" x2="-36.6534" y2="3.9383" layer="94"/>
<rectangle x1="-36.4045" y1="3.9205" x2="-36.2978" y2="3.9383" layer="94"/>
<rectangle x1="-36.0667" y1="3.9205" x2="-35.96" y2="3.9383" layer="94"/>
<rectangle x1="-35.7644" y1="3.9205" x2="-35.4088" y2="3.9383" layer="94"/>
<rectangle x1="-40.5294" y1="3.9383" x2="-40.0849" y2="3.9561" layer="94"/>
<rectangle x1="-39.9249" y1="3.9383" x2="-39.516" y2="3.9561" layer="94"/>
<rectangle x1="-39.3737" y1="3.9383" x2="-38.9648" y2="3.9561" layer="94"/>
<rectangle x1="-38.8759" y1="3.9383" x2="-38.7337" y2="3.9561" layer="94"/>
<rectangle x1="-38.5381" y1="3.9383" x2="-38.3781" y2="3.9561" layer="94"/>
<rectangle x1="-38.2358" y1="3.9383" x2="-37.8091" y2="3.9561" layer="94"/>
<rectangle x1="-37.6669" y1="3.9383" x2="-37.5602" y2="3.9561" layer="94"/>
<rectangle x1="-37.329" y1="3.9383" x2="-37.2224" y2="3.9561" layer="94"/>
<rectangle x1="-36.9734" y1="3.9383" x2="-36.8845" y2="3.9561" layer="94"/>
<rectangle x1="-36.7423" y1="3.9383" x2="-36.6534" y2="3.9561" layer="94"/>
<rectangle x1="-36.4045" y1="3.9383" x2="-36.2978" y2="3.9561" layer="94"/>
<rectangle x1="-36.0667" y1="3.9383" x2="-35.96" y2="3.9561" layer="94"/>
<rectangle x1="-35.7822" y1="3.9383" x2="-35.391" y2="3.9561" layer="94"/>
<rectangle x1="-40.5472" y1="3.9561" x2="-40.0849" y2="3.9739" layer="94"/>
<rectangle x1="-39.9427" y1="3.9561" x2="-39.516" y2="3.9739" layer="94"/>
<rectangle x1="-39.3915" y1="3.9561" x2="-38.9648" y2="3.9739" layer="94"/>
<rectangle x1="-38.8759" y1="3.9561" x2="-38.7337" y2="3.9739" layer="94"/>
<rectangle x1="-38.5559" y1="3.9561" x2="-38.3958" y2="3.9739" layer="94"/>
<rectangle x1="-38.2536" y1="3.9561" x2="-38.1114" y2="3.9739" layer="94"/>
<rectangle x1="-37.9869" y1="3.9561" x2="-37.8091" y2="3.9739" layer="94"/>
<rectangle x1="-37.6669" y1="3.9561" x2="-37.5602" y2="3.9739" layer="94"/>
<rectangle x1="-37.329" y1="3.9561" x2="-37.2224" y2="3.9739" layer="94"/>
<rectangle x1="-36.9734" y1="3.9561" x2="-36.8845" y2="3.9739" layer="94"/>
<rectangle x1="-36.7423" y1="3.9561" x2="-36.6534" y2="3.9739" layer="94"/>
<rectangle x1="-36.4045" y1="3.9561" x2="-36.2978" y2="3.9739" layer="94"/>
<rectangle x1="-36.0667" y1="3.9561" x2="-35.96" y2="3.9739" layer="94"/>
<rectangle x1="-35.8" y1="3.9561" x2="-35.6399" y2="3.9739" layer="94"/>
<rectangle x1="-35.5333" y1="3.9561" x2="-35.3732" y2="3.9739" layer="94"/>
<rectangle x1="-40.565" y1="3.9739" x2="-40.0849" y2="3.9917" layer="94"/>
<rectangle x1="-39.9605" y1="3.9739" x2="-39.516" y2="3.9917" layer="94"/>
<rectangle x1="-39.4093" y1="3.9739" x2="-38.9648" y2="3.9917" layer="94"/>
<rectangle x1="-38.8759" y1="3.9739" x2="-38.7337" y2="3.9917" layer="94"/>
<rectangle x1="-38.5736" y1="3.9739" x2="-38.3958" y2="3.9917" layer="94"/>
<rectangle x1="-38.2536" y1="3.9739" x2="-38.1469" y2="3.9917" layer="94"/>
<rectangle x1="-37.9336" y1="3.9739" x2="-37.8091" y2="3.9917" layer="94"/>
<rectangle x1="-37.6669" y1="3.9739" x2="-37.5602" y2="3.9917" layer="94"/>
<rectangle x1="-37.329" y1="3.9739" x2="-37.2224" y2="3.9917" layer="94"/>
<rectangle x1="-36.9734" y1="3.9739" x2="-36.8845" y2="3.9917" layer="94"/>
<rectangle x1="-36.7423" y1="3.9739" x2="-36.6534" y2="3.9917" layer="94"/>
<rectangle x1="-36.4045" y1="3.9739" x2="-36.2978" y2="3.9917" layer="94"/>
<rectangle x1="-36.0667" y1="3.9739" x2="-35.96" y2="3.9917" layer="94"/>
<rectangle x1="-35.8177" y1="3.9739" x2="-35.6933" y2="3.9917" layer="94"/>
<rectangle x1="-35.4977" y1="3.9739" x2="-35.3732" y2="3.9917" layer="94"/>
<rectangle x1="-40.5828" y1="3.9917" x2="-40.0849" y2="4.0094" layer="94"/>
<rectangle x1="-39.9605" y1="3.9917" x2="-39.7827" y2="4.0094" layer="94"/>
<rectangle x1="-39.5338" y1="3.9917" x2="-39.516" y2="4.0094" layer="94"/>
<rectangle x1="-39.4093" y1="3.9917" x2="-39.2315" y2="4.0094" layer="94"/>
<rectangle x1="-38.9826" y1="3.9917" x2="-38.9648" y2="4.0094" layer="94"/>
<rectangle x1="-38.8759" y1="3.9917" x2="-38.7337" y2="4.0094" layer="94"/>
<rectangle x1="-38.5736" y1="3.9917" x2="-38.4136" y2="4.0094" layer="94"/>
<rectangle x1="-38.2536" y1="3.9917" x2="-38.1469" y2="4.0094" layer="94"/>
<rectangle x1="-37.898" y1="3.9917" x2="-37.8091" y2="4.0094" layer="94"/>
<rectangle x1="-37.6669" y1="3.9917" x2="-37.5602" y2="4.0094" layer="94"/>
<rectangle x1="-37.329" y1="3.9917" x2="-37.2224" y2="4.0094" layer="94"/>
<rectangle x1="-36.9734" y1="3.9917" x2="-36.8845" y2="4.0094" layer="94"/>
<rectangle x1="-36.7423" y1="3.9917" x2="-36.6534" y2="4.0094" layer="94"/>
<rectangle x1="-36.4045" y1="3.9917" x2="-36.2978" y2="4.0094" layer="94"/>
<rectangle x1="-36.0667" y1="3.9917" x2="-35.96" y2="4.0094" layer="94"/>
<rectangle x1="-35.8177" y1="3.9917" x2="-35.7111" y2="4.0094" layer="94"/>
<rectangle x1="-35.4799" y1="3.9917" x2="-35.3555" y2="4.0094" layer="94"/>
<rectangle x1="-40.5828" y1="4.0094" x2="-40.405" y2="4.0272" layer="94"/>
<rectangle x1="-40.245" y1="4.0094" x2="-40.0849" y2="4.0272" layer="94"/>
<rectangle x1="-39.9783" y1="4.0094" x2="-39.8182" y2="4.0272" layer="94"/>
<rectangle x1="-39.4271" y1="4.0094" x2="-39.2671" y2="4.0272" layer="94"/>
<rectangle x1="-38.8759" y1="4.0094" x2="-38.7337" y2="4.0272" layer="94"/>
<rectangle x1="-38.5914" y1="4.0094" x2="-38.4314" y2="4.0272" layer="94"/>
<rectangle x1="-38.2536" y1="4.0094" x2="-38.1647" y2="4.0272" layer="94"/>
<rectangle x1="-37.898" y1="4.0094" x2="-37.8091" y2="4.0272" layer="94"/>
<rectangle x1="-37.6669" y1="4.0094" x2="-37.5602" y2="4.0272" layer="94"/>
<rectangle x1="-37.329" y1="4.0094" x2="-37.2224" y2="4.0272" layer="94"/>
<rectangle x1="-36.9734" y1="4.0094" x2="-36.8845" y2="4.0272" layer="94"/>
<rectangle x1="-36.7423" y1="4.0094" x2="-36.6534" y2="4.0272" layer="94"/>
<rectangle x1="-36.4045" y1="4.0094" x2="-36.2978" y2="4.0272" layer="94"/>
<rectangle x1="-36.0667" y1="4.0094" x2="-35.96" y2="4.0272" layer="94"/>
<rectangle x1="-35.8355" y1="4.0094" x2="-35.7288" y2="4.0272" layer="94"/>
<rectangle x1="-35.4621" y1="4.0094" x2="-35.3555" y2="4.0272" layer="94"/>
<rectangle x1="-40.5828" y1="4.0272" x2="-40.4228" y2="4.045" layer="94"/>
<rectangle x1="-40.2272" y1="4.0272" x2="-40.0849" y2="4.045" layer="94"/>
<rectangle x1="-39.9783" y1="4.0272" x2="-39.836" y2="4.045" layer="94"/>
<rectangle x1="-39.4271" y1="4.0272" x2="-39.2848" y2="4.045" layer="94"/>
<rectangle x1="-38.8759" y1="4.0272" x2="-38.7337" y2="4.045" layer="94"/>
<rectangle x1="-38.6092" y1="4.0272" x2="-38.4492" y2="4.045" layer="94"/>
<rectangle x1="-38.2714" y1="4.0272" x2="-38.1647" y2="4.045" layer="94"/>
<rectangle x1="-37.898" y1="4.0272" x2="-37.8091" y2="4.045" layer="94"/>
<rectangle x1="-37.6669" y1="4.0272" x2="-37.5602" y2="4.045" layer="94"/>
<rectangle x1="-37.329" y1="4.0272" x2="-37.2224" y2="4.045" layer="94"/>
<rectangle x1="-36.9734" y1="4.0272" x2="-36.8845" y2="4.045" layer="94"/>
<rectangle x1="-36.7423" y1="4.0272" x2="-36.6534" y2="4.045" layer="94"/>
<rectangle x1="-36.4045" y1="4.0272" x2="-36.2978" y2="4.045" layer="94"/>
<rectangle x1="-36.0667" y1="4.0272" x2="-35.96" y2="4.045" layer="94"/>
<rectangle x1="-35.8355" y1="4.0272" x2="-35.7288" y2="4.045" layer="94"/>
<rectangle x1="-35.4444" y1="4.0272" x2="-35.3377" y2="4.045" layer="94"/>
<rectangle x1="-40.6006" y1="4.045" x2="-40.4405" y2="4.0628" layer="94"/>
<rectangle x1="-40.2094" y1="4.045" x2="-40.0849" y2="4.0628" layer="94"/>
<rectangle x1="-39.9783" y1="4.045" x2="-39.8538" y2="4.0628" layer="94"/>
<rectangle x1="-39.4449" y1="4.045" x2="-39.3026" y2="4.0628" layer="94"/>
<rectangle x1="-38.8759" y1="4.045" x2="-38.7337" y2="4.0628" layer="94"/>
<rectangle x1="-38.627" y1="4.045" x2="-38.4492" y2="4.0628" layer="94"/>
<rectangle x1="-38.2714" y1="4.045" x2="-38.1647" y2="4.0628" layer="94"/>
<rectangle x1="-37.898" y1="4.045" x2="-37.8091" y2="4.0628" layer="94"/>
<rectangle x1="-37.6669" y1="4.045" x2="-37.5602" y2="4.0628" layer="94"/>
<rectangle x1="-37.329" y1="4.045" x2="-37.2224" y2="4.0628" layer="94"/>
<rectangle x1="-36.9734" y1="4.045" x2="-36.8845" y2="4.0628" layer="94"/>
<rectangle x1="-36.7423" y1="4.045" x2="-36.6534" y2="4.0628" layer="94"/>
<rectangle x1="-36.4045" y1="4.045" x2="-36.2978" y2="4.0628" layer="94"/>
<rectangle x1="-36.0667" y1="4.045" x2="-35.96" y2="4.0628" layer="94"/>
<rectangle x1="-35.8355" y1="4.045" x2="-35.7466" y2="4.0628" layer="94"/>
<rectangle x1="-35.4444" y1="4.045" x2="-35.3377" y2="4.0628" layer="94"/>
<rectangle x1="-40.6006" y1="4.0628" x2="-40.4583" y2="4.0806" layer="94"/>
<rectangle x1="-40.2094" y1="4.0628" x2="-40.0849" y2="4.0806" layer="94"/>
<rectangle x1="-39.996" y1="4.0628" x2="-39.8538" y2="4.0806" layer="94"/>
<rectangle x1="-39.4449" y1="4.0628" x2="-39.3026" y2="4.0806" layer="94"/>
<rectangle x1="-38.8759" y1="4.0628" x2="-38.7337" y2="4.0806" layer="94"/>
<rectangle x1="-38.627" y1="4.0628" x2="-38.467" y2="4.0806" layer="94"/>
<rectangle x1="-38.2536" y1="4.0628" x2="-38.1647" y2="4.0806" layer="94"/>
<rectangle x1="-37.898" y1="4.0628" x2="-37.8091" y2="4.0806" layer="94"/>
<rectangle x1="-37.6669" y1="4.0628" x2="-37.5602" y2="4.0806" layer="94"/>
<rectangle x1="-37.329" y1="4.0628" x2="-37.2224" y2="4.0806" layer="94"/>
<rectangle x1="-36.9734" y1="4.0628" x2="-36.8845" y2="4.0806" layer="94"/>
<rectangle x1="-36.7423" y1="4.0628" x2="-36.6534" y2="4.0806" layer="94"/>
<rectangle x1="-36.4045" y1="4.0628" x2="-36.2978" y2="4.0806" layer="94"/>
<rectangle x1="-36.0667" y1="4.0628" x2="-35.96" y2="4.0806" layer="94"/>
<rectangle x1="-35.8355" y1="4.0628" x2="-35.7466" y2="4.0806" layer="94"/>
<rectangle x1="-35.4444" y1="4.0628" x2="-35.3377" y2="4.0806" layer="94"/>
<rectangle x1="-40.6006" y1="4.0806" x2="-40.4583" y2="4.0983" layer="94"/>
<rectangle x1="-40.2094" y1="4.0806" x2="-40.0849" y2="4.0983" layer="94"/>
<rectangle x1="-39.996" y1="4.0806" x2="-39.8538" y2="4.0983" layer="94"/>
<rectangle x1="-39.4449" y1="4.0806" x2="-39.3026" y2="4.0983" layer="94"/>
<rectangle x1="-38.8759" y1="4.0806" x2="-38.7337" y2="4.0983" layer="94"/>
<rectangle x1="-38.6448" y1="4.0806" x2="-38.4847" y2="4.0983" layer="94"/>
<rectangle x1="-38.2536" y1="4.0806" x2="-38.1647" y2="4.0983" layer="94"/>
<rectangle x1="-37.898" y1="4.0806" x2="-37.8091" y2="4.0983" layer="94"/>
<rectangle x1="-37.6669" y1="4.0806" x2="-37.5602" y2="4.0983" layer="94"/>
<rectangle x1="-37.329" y1="4.0806" x2="-37.2224" y2="4.0983" layer="94"/>
<rectangle x1="-36.9734" y1="4.0806" x2="-36.8845" y2="4.0983" layer="94"/>
<rectangle x1="-36.7423" y1="4.0806" x2="-36.6534" y2="4.0983" layer="94"/>
<rectangle x1="-36.4045" y1="4.0806" x2="-36.2978" y2="4.0983" layer="94"/>
<rectangle x1="-36.0667" y1="4.0806" x2="-35.96" y2="4.0983" layer="94"/>
<rectangle x1="-35.8533" y1="4.0806" x2="-35.7466" y2="4.0983" layer="94"/>
<rectangle x1="-35.4266" y1="4.0806" x2="-35.3377" y2="4.0983" layer="94"/>
<rectangle x1="-40.6006" y1="4.0983" x2="-40.4583" y2="4.1161" layer="94"/>
<rectangle x1="-40.2094" y1="4.0983" x2="-40.0849" y2="4.1161" layer="94"/>
<rectangle x1="-39.996" y1="4.0983" x2="-39.4982" y2="4.1161" layer="94"/>
<rectangle x1="-39.4449" y1="4.0983" x2="-38.947" y2="4.1161" layer="94"/>
<rectangle x1="-38.8759" y1="4.0983" x2="-38.7337" y2="4.1161" layer="94"/>
<rectangle x1="-38.6625" y1="4.0983" x2="-38.4492" y2="4.1161" layer="94"/>
<rectangle x1="-38.2536" y1="4.0983" x2="-38.1469" y2="4.1161" layer="94"/>
<rectangle x1="-37.898" y1="4.0983" x2="-37.8091" y2="4.1161" layer="94"/>
<rectangle x1="-37.6669" y1="4.0983" x2="-37.5602" y2="4.1161" layer="94"/>
<rectangle x1="-37.329" y1="4.0983" x2="-37.2224" y2="4.1161" layer="94"/>
<rectangle x1="-36.9734" y1="4.0983" x2="-36.8845" y2="4.1161" layer="94"/>
<rectangle x1="-36.7423" y1="4.0983" x2="-36.6534" y2="4.1161" layer="94"/>
<rectangle x1="-36.4045" y1="4.0983" x2="-36.2978" y2="4.1161" layer="94"/>
<rectangle x1="-36.0667" y1="4.0983" x2="-35.96" y2="4.1161" layer="94"/>
<rectangle x1="-35.8533" y1="4.0983" x2="-35.7466" y2="4.1161" layer="94"/>
<rectangle x1="-35.4266" y1="4.0983" x2="-35.3377" y2="4.1161" layer="94"/>
<rectangle x1="-40.6006" y1="4.1161" x2="-40.4583" y2="4.1339" layer="94"/>
<rectangle x1="-40.2094" y1="4.1161" x2="-40.0849" y2="4.1339" layer="94"/>
<rectangle x1="-39.996" y1="4.1161" x2="-39.4982" y2="4.1339" layer="94"/>
<rectangle x1="-39.4449" y1="4.1161" x2="-38.947" y2="4.1339" layer="94"/>
<rectangle x1="-38.8759" y1="4.1161" x2="-38.7337" y2="4.1339" layer="94"/>
<rectangle x1="-38.6625" y1="4.1161" x2="-38.4314" y2="4.1339" layer="94"/>
<rectangle x1="-38.2536" y1="4.1161" x2="-38.1114" y2="4.1339" layer="94"/>
<rectangle x1="-37.898" y1="4.1161" x2="-37.8091" y2="4.1339" layer="94"/>
<rectangle x1="-37.6669" y1="4.1161" x2="-37.5602" y2="4.1339" layer="94"/>
<rectangle x1="-37.329" y1="4.1161" x2="-37.2224" y2="4.1339" layer="94"/>
<rectangle x1="-36.9734" y1="4.1161" x2="-36.8845" y2="4.1339" layer="94"/>
<rectangle x1="-36.7423" y1="4.1161" x2="-36.6534" y2="4.1339" layer="94"/>
<rectangle x1="-36.4045" y1="4.1161" x2="-36.2978" y2="4.1339" layer="94"/>
<rectangle x1="-36.0667" y1="4.1161" x2="-35.96" y2="4.1339" layer="94"/>
<rectangle x1="-35.8533" y1="4.1161" x2="-35.7466" y2="4.1339" layer="94"/>
<rectangle x1="-35.4266" y1="4.1161" x2="-35.3377" y2="4.1339" layer="94"/>
<rectangle x1="-40.6006" y1="4.1339" x2="-40.4761" y2="4.1517" layer="94"/>
<rectangle x1="-40.2094" y1="4.1339" x2="-40.0849" y2="4.1517" layer="94"/>
<rectangle x1="-39.996" y1="4.1339" x2="-39.4982" y2="4.1517" layer="94"/>
<rectangle x1="-39.4449" y1="4.1339" x2="-38.947" y2="4.1517" layer="94"/>
<rectangle x1="-38.8759" y1="4.1339" x2="-38.7337" y2="4.1517" layer="94"/>
<rectangle x1="-38.6625" y1="4.1339" x2="-38.3958" y2="4.1517" layer="94"/>
<rectangle x1="-38.2358" y1="4.1339" x2="-37.8091" y2="4.1517" layer="94"/>
<rectangle x1="-37.6669" y1="4.1339" x2="-37.5602" y2="4.1517" layer="94"/>
<rectangle x1="-37.329" y1="4.1339" x2="-37.2224" y2="4.1517" layer="94"/>
<rectangle x1="-36.9734" y1="4.1339" x2="-36.8845" y2="4.1517" layer="94"/>
<rectangle x1="-36.7423" y1="4.1339" x2="-36.6534" y2="4.1517" layer="94"/>
<rectangle x1="-36.4045" y1="4.1339" x2="-36.2978" y2="4.1517" layer="94"/>
<rectangle x1="-36.0667" y1="4.1339" x2="-35.96" y2="4.1517" layer="94"/>
<rectangle x1="-35.8533" y1="4.1339" x2="-35.7466" y2="4.1517" layer="94"/>
<rectangle x1="-35.4266" y1="4.1339" x2="-35.3377" y2="4.1517" layer="94"/>
<rectangle x1="-40.6006" y1="4.1517" x2="-40.4761" y2="4.1695" layer="94"/>
<rectangle x1="-40.2094" y1="4.1517" x2="-40.0849" y2="4.1695" layer="94"/>
<rectangle x1="-39.996" y1="4.1517" x2="-39.4982" y2="4.1695" layer="94"/>
<rectangle x1="-39.4449" y1="4.1517" x2="-38.947" y2="4.1695" layer="94"/>
<rectangle x1="-38.8759" y1="4.1517" x2="-38.7337" y2="4.1695" layer="94"/>
<rectangle x1="-38.6092" y1="4.1517" x2="-38.3781" y2="4.1695" layer="94"/>
<rectangle x1="-38.218" y1="4.1517" x2="-37.8091" y2="4.1695" layer="94"/>
<rectangle x1="-37.6669" y1="4.1517" x2="-37.5602" y2="4.1695" layer="94"/>
<rectangle x1="-37.329" y1="4.1517" x2="-37.2224" y2="4.1695" layer="94"/>
<rectangle x1="-36.9734" y1="4.1517" x2="-36.8845" y2="4.1695" layer="94"/>
<rectangle x1="-36.7423" y1="4.1517" x2="-36.6534" y2="4.1695" layer="94"/>
<rectangle x1="-36.4045" y1="4.1517" x2="-36.2978" y2="4.1695" layer="94"/>
<rectangle x1="-36.0667" y1="4.1517" x2="-35.96" y2="4.1695" layer="94"/>
<rectangle x1="-35.8533" y1="4.1517" x2="-35.7466" y2="4.1695" layer="94"/>
<rectangle x1="-35.4266" y1="4.1517" x2="-35.3377" y2="4.1695" layer="94"/>
<rectangle x1="-40.6006" y1="4.1695" x2="-40.4761" y2="4.1872" layer="94"/>
<rectangle x1="-40.2094" y1="4.1695" x2="-40.0849" y2="4.1872" layer="94"/>
<rectangle x1="-39.996" y1="4.1695" x2="-39.4982" y2="4.1872" layer="94"/>
<rectangle x1="-39.4449" y1="4.1695" x2="-38.947" y2="4.1872" layer="94"/>
<rectangle x1="-38.8759" y1="4.1695" x2="-38.7337" y2="4.1872" layer="94"/>
<rectangle x1="-38.5381" y1="4.1695" x2="-38.3603" y2="4.1872" layer="94"/>
<rectangle x1="-38.2003" y1="4.1695" x2="-37.8091" y2="4.1872" layer="94"/>
<rectangle x1="-37.6669" y1="4.1695" x2="-37.5602" y2="4.1872" layer="94"/>
<rectangle x1="-37.329" y1="4.1695" x2="-37.2224" y2="4.1872" layer="94"/>
<rectangle x1="-36.9734" y1="4.1695" x2="-36.8845" y2="4.1872" layer="94"/>
<rectangle x1="-36.7423" y1="4.1695" x2="-36.6534" y2="4.1872" layer="94"/>
<rectangle x1="-36.4045" y1="4.1695" x2="-36.2978" y2="4.1872" layer="94"/>
<rectangle x1="-36.0667" y1="4.1695" x2="-35.96" y2="4.1872" layer="94"/>
<rectangle x1="-35.8533" y1="4.1695" x2="-35.7466" y2="4.1872" layer="94"/>
<rectangle x1="-35.4266" y1="4.1695" x2="-35.3377" y2="4.1872" layer="94"/>
<rectangle x1="-40.6006" y1="4.1872" x2="-40.4583" y2="4.205" layer="94"/>
<rectangle x1="-40.2094" y1="4.1872" x2="-40.0849" y2="4.205" layer="94"/>
<rectangle x1="-39.996" y1="4.1872" x2="-39.4982" y2="4.205" layer="94"/>
<rectangle x1="-39.4449" y1="4.1872" x2="-38.947" y2="4.205" layer="94"/>
<rectangle x1="-38.8759" y1="4.1872" x2="-38.7337" y2="4.205" layer="94"/>
<rectangle x1="-38.5203" y1="4.1872" x2="-38.3603" y2="4.205" layer="94"/>
<rectangle x1="-38.1647" y1="4.1872" x2="-37.8091" y2="4.205" layer="94"/>
<rectangle x1="-37.6669" y1="4.1872" x2="-37.5602" y2="4.205" layer="94"/>
<rectangle x1="-37.329" y1="4.1872" x2="-37.2224" y2="4.205" layer="94"/>
<rectangle x1="-36.9734" y1="4.1872" x2="-36.8845" y2="4.205" layer="94"/>
<rectangle x1="-36.7423" y1="4.1872" x2="-36.6534" y2="4.205" layer="94"/>
<rectangle x1="-36.4045" y1="4.1872" x2="-36.2978" y2="4.205" layer="94"/>
<rectangle x1="-36.0667" y1="4.1872" x2="-35.96" y2="4.205" layer="94"/>
<rectangle x1="-35.8533" y1="4.1872" x2="-35.7466" y2="4.205" layer="94"/>
<rectangle x1="-35.4266" y1="4.1872" x2="-35.3377" y2="4.205" layer="94"/>
<rectangle x1="-40.6006" y1="4.205" x2="-40.4583" y2="4.2228" layer="94"/>
<rectangle x1="-40.2094" y1="4.205" x2="-40.0849" y2="4.2228" layer="94"/>
<rectangle x1="-39.996" y1="4.205" x2="-39.8538" y2="4.2228" layer="94"/>
<rectangle x1="-39.6404" y1="4.205" x2="-39.4982" y2="4.2228" layer="94"/>
<rectangle x1="-39.4449" y1="4.205" x2="-39.3026" y2="4.2228" layer="94"/>
<rectangle x1="-39.0893" y1="4.205" x2="-38.947" y2="4.2228" layer="94"/>
<rectangle x1="-38.8759" y1="4.205" x2="-38.7337" y2="4.2228" layer="94"/>
<rectangle x1="-38.5025" y1="4.205" x2="-38.3425" y2="4.2228" layer="94"/>
<rectangle x1="-38.0936" y1="4.205" x2="-37.9691" y2="4.2228" layer="94"/>
<rectangle x1="-37.898" y1="4.205" x2="-37.8091" y2="4.2228" layer="94"/>
<rectangle x1="-37.6669" y1="4.205" x2="-37.5602" y2="4.2228" layer="94"/>
<rectangle x1="-37.329" y1="4.205" x2="-37.2224" y2="4.2228" layer="94"/>
<rectangle x1="-36.9734" y1="4.205" x2="-36.8845" y2="4.2228" layer="94"/>
<rectangle x1="-36.7423" y1="4.205" x2="-36.6534" y2="4.2228" layer="94"/>
<rectangle x1="-36.4045" y1="4.205" x2="-36.2978" y2="4.2228" layer="94"/>
<rectangle x1="-36.0667" y1="4.205" x2="-35.96" y2="4.2228" layer="94"/>
<rectangle x1="-35.8533" y1="4.205" x2="-35.7466" y2="4.2228" layer="94"/>
<rectangle x1="-35.4266" y1="4.205" x2="-35.3377" y2="4.2228" layer="94"/>
<rectangle x1="-40.6006" y1="4.2228" x2="-40.4583" y2="4.2406" layer="94"/>
<rectangle x1="-40.2094" y1="4.2228" x2="-40.0849" y2="4.2406" layer="94"/>
<rectangle x1="-39.996" y1="4.2228" x2="-39.8538" y2="4.2406" layer="94"/>
<rectangle x1="-39.6404" y1="4.2228" x2="-39.4982" y2="4.2406" layer="94"/>
<rectangle x1="-39.4449" y1="4.2228" x2="-39.3026" y2="4.2406" layer="94"/>
<rectangle x1="-39.0893" y1="4.2228" x2="-38.947" y2="4.2406" layer="94"/>
<rectangle x1="-38.8759" y1="4.2228" x2="-38.7337" y2="4.2406" layer="94"/>
<rectangle x1="-38.4847" y1="4.2228" x2="-38.3425" y2="4.2406" layer="94"/>
<rectangle x1="-37.898" y1="4.2228" x2="-37.8091" y2="4.2406" layer="94"/>
<rectangle x1="-37.6669" y1="4.2228" x2="-37.5602" y2="4.2406" layer="94"/>
<rectangle x1="-37.329" y1="4.2228" x2="-37.2224" y2="4.2406" layer="94"/>
<rectangle x1="-36.9734" y1="4.2228" x2="-36.8845" y2="4.2406" layer="94"/>
<rectangle x1="-36.7423" y1="4.2228" x2="-36.6534" y2="4.2406" layer="94"/>
<rectangle x1="-36.4045" y1="4.2228" x2="-36.2978" y2="4.2406" layer="94"/>
<rectangle x1="-36.0667" y1="4.2228" x2="-35.96" y2="4.2406" layer="94"/>
<rectangle x1="-35.8355" y1="4.2228" x2="-35.7466" y2="4.2406" layer="94"/>
<rectangle x1="-35.4444" y1="4.2228" x2="-35.3377" y2="4.2406" layer="94"/>
<rectangle x1="-40.6006" y1="4.2406" x2="-40.4405" y2="4.2584" layer="94"/>
<rectangle x1="-40.2094" y1="4.2406" x2="-40.0849" y2="4.2584" layer="94"/>
<rectangle x1="-39.996" y1="4.2406" x2="-39.8538" y2="4.2584" layer="94"/>
<rectangle x1="-39.6404" y1="4.2406" x2="-39.4982" y2="4.2584" layer="94"/>
<rectangle x1="-39.4449" y1="4.2406" x2="-39.3026" y2="4.2584" layer="94"/>
<rectangle x1="-39.0893" y1="4.2406" x2="-38.947" y2="4.2584" layer="94"/>
<rectangle x1="-38.8759" y1="4.2406" x2="-38.7337" y2="4.2584" layer="94"/>
<rectangle x1="-38.4847" y1="4.2406" x2="-38.3425" y2="4.2584" layer="94"/>
<rectangle x1="-37.898" y1="4.2406" x2="-37.8091" y2="4.2584" layer="94"/>
<rectangle x1="-37.6669" y1="4.2406" x2="-37.5602" y2="4.2584" layer="94"/>
<rectangle x1="-37.329" y1="4.2406" x2="-37.2224" y2="4.2584" layer="94"/>
<rectangle x1="-36.9912" y1="4.2406" x2="-36.8845" y2="4.2584" layer="94"/>
<rectangle x1="-36.7423" y1="4.2406" x2="-36.6534" y2="4.2584" layer="94"/>
<rectangle x1="-36.4045" y1="4.2406" x2="-36.2978" y2="4.2584" layer="94"/>
<rectangle x1="-36.0667" y1="4.2406" x2="-35.96" y2="4.2584" layer="94"/>
<rectangle x1="-35.8355" y1="4.2406" x2="-35.7466" y2="4.2584" layer="94"/>
<rectangle x1="-35.4444" y1="4.2406" x2="-35.3377" y2="4.2584" layer="94"/>
<rectangle x1="-40.5828" y1="4.2584" x2="-40.4405" y2="4.2761" layer="94"/>
<rectangle x1="-40.2094" y1="4.2584" x2="-40.0849" y2="4.2761" layer="94"/>
<rectangle x1="-39.9783" y1="4.2584" x2="-39.836" y2="4.2761" layer="94"/>
<rectangle x1="-39.6404" y1="4.2584" x2="-39.4982" y2="4.2761" layer="94"/>
<rectangle x1="-39.4271" y1="4.2584" x2="-39.2848" y2="4.2761" layer="94"/>
<rectangle x1="-39.0893" y1="4.2584" x2="-38.947" y2="4.2761" layer="94"/>
<rectangle x1="-38.8759" y1="4.2584" x2="-38.7159" y2="4.2761" layer="94"/>
<rectangle x1="-38.4847" y1="4.2584" x2="-38.3425" y2="4.2761" layer="94"/>
<rectangle x1="-37.898" y1="4.2584" x2="-37.8091" y2="4.2761" layer="94"/>
<rectangle x1="-37.6669" y1="4.2584" x2="-37.5602" y2="4.2761" layer="94"/>
<rectangle x1="-37.329" y1="4.2584" x2="-37.2224" y2="4.2761" layer="94"/>
<rectangle x1="-36.9912" y1="4.2584" x2="-36.8845" y2="4.2761" layer="94"/>
<rectangle x1="-36.7423" y1="4.2584" x2="-36.6534" y2="4.2761" layer="94"/>
<rectangle x1="-36.4045" y1="4.2584" x2="-36.2978" y2="4.2761" layer="94"/>
<rectangle x1="-36.0667" y1="4.2584" x2="-35.96" y2="4.2761" layer="94"/>
<rectangle x1="-35.8355" y1="4.2584" x2="-35.7288" y2="4.2761" layer="94"/>
<rectangle x1="-35.4444" y1="4.2584" x2="-35.3377" y2="4.2761" layer="94"/>
<rectangle x1="-40.5828" y1="4.2761" x2="-40.4228" y2="4.2939" layer="94"/>
<rectangle x1="-40.2094" y1="4.2761" x2="-40.0849" y2="4.2939" layer="94"/>
<rectangle x1="-39.9783" y1="4.2761" x2="-39.8182" y2="4.2939" layer="94"/>
<rectangle x1="-39.6582" y1="4.2761" x2="-39.516" y2="4.2939" layer="94"/>
<rectangle x1="-39.4271" y1="4.2761" x2="-39.2671" y2="4.2939" layer="94"/>
<rectangle x1="-39.107" y1="4.2761" x2="-38.9648" y2="4.2939" layer="94"/>
<rectangle x1="-38.8759" y1="4.2761" x2="-38.6981" y2="4.2939" layer="94"/>
<rectangle x1="-38.5025" y1="4.2761" x2="-38.3425" y2="4.2939" layer="94"/>
<rectangle x1="-37.898" y1="4.2761" x2="-37.8091" y2="4.2939" layer="94"/>
<rectangle x1="-37.6669" y1="4.2761" x2="-37.5602" y2="4.2939" layer="94"/>
<rectangle x1="-37.329" y1="4.2761" x2="-37.2046" y2="4.2939" layer="94"/>
<rectangle x1="-36.9912" y1="4.2761" x2="-36.8845" y2="4.2939" layer="94"/>
<rectangle x1="-36.7423" y1="4.2761" x2="-36.6356" y2="4.2939" layer="94"/>
<rectangle x1="-36.4223" y1="4.2761" x2="-36.2978" y2="4.2939" layer="94"/>
<rectangle x1="-36.0667" y1="4.2761" x2="-35.9778" y2="4.2939" layer="94"/>
<rectangle x1="-35.8355" y1="4.2761" x2="-35.7288" y2="4.2939" layer="94"/>
<rectangle x1="-35.4621" y1="4.2761" x2="-35.3555" y2="4.2939" layer="94"/>
<rectangle x1="-40.5828" y1="4.2939" x2="-40.3872" y2="4.3117" layer="94"/>
<rectangle x1="-40.2272" y1="4.2939" x2="-40.0849" y2="4.3117" layer="94"/>
<rectangle x1="-39.9783" y1="4.2939" x2="-39.8005" y2="4.3117" layer="94"/>
<rectangle x1="-39.676" y1="4.2939" x2="-39.516" y2="4.3117" layer="94"/>
<rectangle x1="-39.4271" y1="4.2939" x2="-39.2493" y2="4.3117" layer="94"/>
<rectangle x1="-39.1426" y1="4.2939" x2="-38.9648" y2="4.3117" layer="94"/>
<rectangle x1="-38.8759" y1="4.2939" x2="-38.6448" y2="4.3117" layer="94"/>
<rectangle x1="-38.5203" y1="4.2939" x2="-38.3425" y2="4.3117" layer="94"/>
<rectangle x1="-37.9158" y1="4.2939" x2="-37.8091" y2="4.3117" layer="94"/>
<rectangle x1="-37.6669" y1="4.2939" x2="-37.5246" y2="4.3117" layer="94"/>
<rectangle x1="-37.3468" y1="4.2939" x2="-37.1868" y2="4.3117" layer="94"/>
<rectangle x1="-37.009" y1="4.2939" x2="-36.8845" y2="4.3117" layer="94"/>
<rectangle x1="-36.7423" y1="4.2939" x2="-36.6178" y2="4.3117" layer="94"/>
<rectangle x1="-36.4223" y1="4.2939" x2="-36.2622" y2="4.3117" layer="94"/>
<rectangle x1="-36.0844" y1="4.2939" x2="-35.9778" y2="4.3117" layer="94"/>
<rectangle x1="-35.8177" y1="4.2939" x2="-35.7111" y2="4.3117" layer="94"/>
<rectangle x1="-35.4799" y1="4.2939" x2="-35.3555" y2="4.3117" layer="94"/>
<rectangle x1="-40.565" y1="4.3117" x2="-40.0849" y2="4.3295" layer="94"/>
<rectangle x1="-39.9605" y1="4.3117" x2="-39.516" y2="4.3295" layer="94"/>
<rectangle x1="-39.4093" y1="4.3117" x2="-38.9648" y2="4.3295" layer="94"/>
<rectangle x1="-38.8759" y1="4.3117" x2="-38.3603" y2="4.3295" layer="94"/>
<rectangle x1="-37.9336" y1="4.3117" x2="-37.8091" y2="4.3295" layer="94"/>
<rectangle x1="-37.6669" y1="4.3117" x2="-37.5068" y2="4.3295" layer="94"/>
<rectangle x1="-37.3646" y1="4.3117" x2="-37.169" y2="4.3295" layer="94"/>
<rectangle x1="-37.009" y1="4.3117" x2="-36.9023" y2="4.3295" layer="94"/>
<rectangle x1="-36.7423" y1="4.3117" x2="-36.5823" y2="4.3295" layer="94"/>
<rectangle x1="-36.44" y1="4.3117" x2="-36.2445" y2="4.3295" layer="94"/>
<rectangle x1="-36.1022" y1="4.3117" x2="-35.9778" y2="4.3295" layer="94"/>
<rectangle x1="-35.8177" y1="4.3117" x2="-35.6755" y2="4.3295" layer="94"/>
<rectangle x1="-35.4977" y1="4.3117" x2="-35.3732" y2="4.3295" layer="94"/>
<rectangle x1="-40.5472" y1="4.3295" x2="-40.0849" y2="4.3473" layer="94"/>
<rectangle x1="-39.9427" y1="4.3295" x2="-39.5338" y2="4.3473" layer="94"/>
<rectangle x1="-39.3915" y1="4.3295" x2="-38.9826" y2="4.3473" layer="94"/>
<rectangle x1="-38.8759" y1="4.3295" x2="-38.3603" y2="4.3473" layer="94"/>
<rectangle x1="-38.218" y1="4.3295" x2="-38.1291" y2="4.3473" layer="94"/>
<rectangle x1="-37.9869" y1="4.3295" x2="-37.8269" y2="4.3473" layer="94"/>
<rectangle x1="-37.6669" y1="4.3295" x2="-37.4535" y2="4.3473" layer="94"/>
<rectangle x1="-37.4002" y1="4.3295" x2="-37.1157" y2="4.3473" layer="94"/>
<rectangle x1="-37.0446" y1="4.3295" x2="-36.9023" y2="4.3473" layer="94"/>
<rectangle x1="-36.7423" y1="4.3295" x2="-36.5467" y2="4.3473" layer="94"/>
<rectangle x1="-36.4756" y1="4.3295" x2="-36.1911" y2="4.3473" layer="94"/>
<rectangle x1="-36.1378" y1="4.3295" x2="-35.9778" y2="4.3473" layer="94"/>
<rectangle x1="-35.8" y1="4.3295" x2="-35.6399" y2="4.3473" layer="94"/>
<rectangle x1="-35.5333" y1="4.3295" x2="-35.3732" y2="4.3473" layer="94"/>
<rectangle x1="-40.5294" y1="4.3473" x2="-40.0849" y2="4.365" layer="94"/>
<rectangle x1="-39.9249" y1="4.3473" x2="-39.5515" y2="4.365" layer="94"/>
<rectangle x1="-39.3915" y1="4.3473" x2="-39.0004" y2="4.365" layer="94"/>
<rectangle x1="-38.8759" y1="4.3473" x2="-38.3781" y2="4.365" layer="94"/>
<rectangle x1="-38.218" y1="4.3473" x2="-37.8447" y2="4.365" layer="94"/>
<rectangle x1="-37.6669" y1="4.3473" x2="-37.2579" y2="4.365" layer="94"/>
<rectangle x1="-37.2401" y1="4.3473" x2="-36.9201" y2="4.365" layer="94"/>
<rectangle x1="-36.7423" y1="4.3473" x2="-36.3334" y2="4.365" layer="94"/>
<rectangle x1="-36.3156" y1="4.3473" x2="-35.9955" y2="4.365" layer="94"/>
<rectangle x1="-35.7822" y1="4.3473" x2="-35.391" y2="4.365" layer="94"/>
<rectangle x1="-40.5117" y1="4.365" x2="-40.0849" y2="4.3828" layer="94"/>
<rectangle x1="-39.9071" y1="4.365" x2="-39.5515" y2="4.3828" layer="94"/>
<rectangle x1="-39.356" y1="4.365" x2="-39.0181" y2="4.3828" layer="94"/>
<rectangle x1="-38.8759" y1="4.365" x2="-38.3781" y2="4.3828" layer="94"/>
<rectangle x1="-38.218" y1="4.365" x2="-37.8624" y2="4.3828" layer="94"/>
<rectangle x1="-37.6669" y1="4.365" x2="-37.578" y2="4.3828" layer="94"/>
<rectangle x1="-37.5602" y1="4.365" x2="-37.2757" y2="4.3828" layer="94"/>
<rectangle x1="-37.2224" y1="4.365" x2="-36.9201" y2="4.3828" layer="94"/>
<rectangle x1="-36.7423" y1="4.365" x2="-36.6712" y2="4.3828" layer="94"/>
<rectangle x1="-36.6356" y1="4.365" x2="-36.3511" y2="4.3828" layer="94"/>
<rectangle x1="-36.2978" y1="4.365" x2="-36.0133" y2="4.3828" layer="94"/>
<rectangle x1="-35.7644" y1="4.365" x2="-35.4088" y2="4.3828" layer="94"/>
<rectangle x1="-40.4761" y1="4.3828" x2="-40.0849" y2="4.4006" layer="94"/>
<rectangle x1="-39.8894" y1="4.3828" x2="-39.5871" y2="4.4006" layer="94"/>
<rectangle x1="-39.3382" y1="4.3828" x2="-39.0359" y2="4.4006" layer="94"/>
<rectangle x1="-38.8759" y1="4.3828" x2="-38.7337" y2="4.4006" layer="94"/>
<rectangle x1="-38.7159" y1="4.3828" x2="-38.4136" y2="4.4006" layer="94"/>
<rectangle x1="-38.218" y1="4.3828" x2="-37.8802" y2="4.4006" layer="94"/>
<rectangle x1="-37.6669" y1="4.3828" x2="-37.5957" y2="4.4006" layer="94"/>
<rectangle x1="-37.5246" y1="4.3828" x2="-37.2935" y2="4.4006" layer="94"/>
<rectangle x1="-37.1868" y1="4.3828" x2="-36.9379" y2="4.4006" layer="94"/>
<rectangle x1="-36.7423" y1="4.3828" x2="-36.6712" y2="4.4006" layer="94"/>
<rectangle x1="-36.6001" y1="4.3828" x2="-36.3689" y2="4.4006" layer="94"/>
<rectangle x1="-36.2622" y1="4.3828" x2="-36.0311" y2="4.4006" layer="94"/>
<rectangle x1="-35.7466" y1="4.3828" x2="-35.4444" y2="4.4006" layer="94"/>
<rectangle x1="-40.4228" y1="4.4006" x2="-40.1383" y2="4.4184" layer="94"/>
<rectangle x1="-39.836" y1="4.4006" x2="-39.6227" y2="4.4184" layer="94"/>
<rectangle x1="-39.2848" y1="4.4006" x2="-39.0715" y2="4.4184" layer="94"/>
<rectangle x1="-38.8759" y1="4.4006" x2="-38.7337" y2="4.4184" layer="94"/>
<rectangle x1="-38.6625" y1="4.4006" x2="-38.4492" y2="4.4184" layer="94"/>
<rectangle x1="-38.1825" y1="4.4006" x2="-37.9336" y2="4.4184" layer="94"/>
<rectangle x1="-37.6669" y1="4.4006" x2="-37.5957" y2="4.4184" layer="94"/>
<rectangle x1="-37.4891" y1="4.4006" x2="-37.3113" y2="4.4184" layer="94"/>
<rectangle x1="-37.1335" y1="4.4006" x2="-36.9734" y2="4.4184" layer="94"/>
<rectangle x1="-36.7423" y1="4.4006" x2="-36.6712" y2="4.4184" layer="94"/>
<rectangle x1="-36.5645" y1="4.4006" x2="-36.4045" y2="4.4184" layer="94"/>
<rectangle x1="-36.2267" y1="4.4006" x2="-36.0489" y2="4.4184" layer="94"/>
<rectangle x1="-35.6933" y1="4.4006" x2="-35.4799" y2="4.4184" layer="94"/>
<rectangle x1="-38.8759" y1="4.4184" x2="-38.7337" y2="4.4362" layer="94"/>
<rectangle x1="-38.8759" y1="4.4362" x2="-38.7337" y2="4.4539" layer="94"/>
<rectangle x1="-38.8759" y1="4.4539" x2="-38.7337" y2="4.4717" layer="94"/>
<rectangle x1="-38.8759" y1="4.4717" x2="-38.7337" y2="4.4895" layer="94"/>
<rectangle x1="-38.8759" y1="4.4895" x2="-38.7337" y2="4.5073" layer="94"/>
<rectangle x1="-38.8759" y1="4.5073" x2="-38.7337" y2="4.5251" layer="94"/>
<rectangle x1="-38.8759" y1="4.5251" x2="-38.7337" y2="4.5428" layer="94"/>
<rectangle x1="-38.8759" y1="4.5428" x2="-38.7337" y2="4.5606" layer="94"/>
<rectangle x1="-38.8759" y1="4.5606" x2="-38.7337" y2="4.5784" layer="94"/>
<rectangle x1="-38.8759" y1="4.5784" x2="-38.7337" y2="4.5962" layer="94"/>
<rectangle x1="-38.8759" y1="4.5962" x2="-38.7337" y2="4.614" layer="94"/>
<rectangle x1="-38.3603" y1="5.3429" x2="-37.578" y2="5.3607" layer="94"/>
<rectangle x1="-38.5914" y1="5.3607" x2="-37.3468" y2="5.3785" layer="94"/>
<rectangle x1="-38.7514" y1="5.3785" x2="-37.1868" y2="5.3963" layer="94"/>
<rectangle x1="-38.8759" y1="5.3963" x2="-37.0623" y2="5.4141" layer="94"/>
<rectangle x1="-39.0004" y1="5.4141" x2="-36.9379" y2="5.4318" layer="94"/>
<rectangle x1="-39.107" y1="5.4318" x2="-36.8312" y2="5.4496" layer="94"/>
<rectangle x1="-39.1959" y1="5.4496" x2="-36.7423" y2="5.4674" layer="94"/>
<rectangle x1="-39.2848" y1="5.4674" x2="-36.6534" y2="5.4852" layer="94"/>
<rectangle x1="-39.3737" y1="5.4852" x2="-36.5645" y2="5.503" layer="94"/>
<rectangle x1="-39.4449" y1="5.503" x2="-36.4934" y2="5.5207" layer="94"/>
<rectangle x1="-39.516" y1="5.5207" x2="-36.4223" y2="5.5385" layer="94"/>
<rectangle x1="-39.5871" y1="5.5385" x2="-36.3511" y2="5.5563" layer="94"/>
<rectangle x1="-39.6582" y1="5.5563" x2="-36.28" y2="5.5741" layer="94"/>
<rectangle x1="-39.7116" y1="5.5741" x2="-36.2267" y2="5.5919" layer="94"/>
<rectangle x1="-39.7827" y1="5.5919" x2="-36.1556" y2="5.6096" layer="94"/>
<rectangle x1="-39.836" y1="5.6096" x2="-36.1022" y2="5.6274" layer="94"/>
<rectangle x1="-39.9071" y1="5.6274" x2="-36.0311" y2="5.6452" layer="94"/>
<rectangle x1="-39.9427" y1="5.6452" x2="-35.9955" y2="5.663" layer="94"/>
<rectangle x1="-39.9427" y1="5.663" x2="-35.9955" y2="5.6808" layer="94"/>
<rectangle x1="-39.9427" y1="5.6808" x2="-35.9955" y2="5.6985" layer="94"/>
<rectangle x1="-39.9427" y1="5.6985" x2="-35.9955" y2="5.7163" layer="94"/>
<rectangle x1="-39.9427" y1="5.7163" x2="-35.9955" y2="5.7341" layer="94"/>
<rectangle x1="-39.9427" y1="5.7341" x2="-35.9955" y2="5.7519" layer="94"/>
<rectangle x1="-39.9427" y1="5.7519" x2="-35.9955" y2="5.7697" layer="94"/>
<rectangle x1="-39.9427" y1="5.7697" x2="-35.9955" y2="5.7874" layer="94"/>
<rectangle x1="-39.9427" y1="5.7874" x2="-35.9955" y2="5.8052" layer="94"/>
<rectangle x1="-39.9427" y1="5.8052" x2="-35.9955" y2="5.823" layer="94"/>
<rectangle x1="-39.9427" y1="5.823" x2="-35.9955" y2="5.8408" layer="94"/>
<rectangle x1="-39.9427" y1="5.8408" x2="-38.3069" y2="5.8586" layer="94"/>
<rectangle x1="-37.6313" y1="5.8408" x2="-35.9955" y2="5.8586" layer="94"/>
<rectangle x1="-39.9427" y1="5.8586" x2="-38.5381" y2="5.8763" layer="94"/>
<rectangle x1="-37.4002" y1="5.8586" x2="-35.9955" y2="5.8763" layer="94"/>
<rectangle x1="-39.9427" y1="5.8763" x2="-38.7159" y2="5.8941" layer="94"/>
<rectangle x1="-37.2224" y1="5.8763" x2="-35.9955" y2="5.8941" layer="94"/>
<rectangle x1="-39.9427" y1="5.8941" x2="-38.8403" y2="5.9119" layer="94"/>
<rectangle x1="-37.0979" y1="5.8941" x2="-35.9955" y2="5.9119" layer="94"/>
<rectangle x1="-39.9427" y1="5.9119" x2="-38.9648" y2="5.9297" layer="94"/>
<rectangle x1="-36.9734" y1="5.9119" x2="-35.9955" y2="5.9297" layer="94"/>
<rectangle x1="-39.9427" y1="5.9297" x2="-39.0715" y2="5.9475" layer="94"/>
<rectangle x1="-36.8668" y1="5.9297" x2="-35.9955" y2="5.9475" layer="94"/>
<rectangle x1="-39.9427" y1="5.9475" x2="-39.1604" y2="5.9652" layer="94"/>
<rectangle x1="-36.7779" y1="5.9475" x2="-35.9955" y2="5.9652" layer="94"/>
<rectangle x1="-39.9427" y1="5.9652" x2="-39.2493" y2="5.983" layer="94"/>
<rectangle x1="-36.689" y1="5.9652" x2="-35.9955" y2="5.983" layer="94"/>
<rectangle x1="-39.9427" y1="5.983" x2="-39.3204" y2="6.0008" layer="94"/>
<rectangle x1="-36.6178" y1="5.983" x2="-35.9955" y2="6.0008" layer="94"/>
<rectangle x1="-39.9427" y1="6.0008" x2="-39.4093" y2="6.0186" layer="94"/>
<rectangle x1="-36.5289" y1="6.0008" x2="-35.9955" y2="6.0186" layer="94"/>
<rectangle x1="-39.9427" y1="6.0186" x2="-39.4804" y2="6.0364" layer="94"/>
<rectangle x1="-36.4578" y1="6.0186" x2="-35.9955" y2="6.0364" layer="94"/>
<rectangle x1="-39.9427" y1="6.0364" x2="-39.5515" y2="6.0541" layer="94"/>
<rectangle x1="-36.3867" y1="6.0364" x2="-35.9955" y2="6.0541" layer="94"/>
<rectangle x1="-39.9427" y1="6.0541" x2="-39.6049" y2="6.0719" layer="94"/>
<rectangle x1="-36.3334" y1="6.0541" x2="-35.9955" y2="6.0719" layer="94"/>
<rectangle x1="-39.9427" y1="6.0719" x2="-39.676" y2="6.0897" layer="94"/>
<rectangle x1="-36.2622" y1="6.0719" x2="-35.9955" y2="6.0897" layer="94"/>
<rectangle x1="-39.9427" y1="6.0897" x2="-39.7293" y2="6.1075" layer="94"/>
<rectangle x1="-36.2089" y1="6.0897" x2="-35.9955" y2="6.1075" layer="94"/>
<rectangle x1="-39.9427" y1="6.1075" x2="-39.7827" y2="6.1253" layer="94"/>
<rectangle x1="-38.2358" y1="6.1075" x2="-37.7024" y2="6.1253" layer="94"/>
<rectangle x1="-36.1556" y1="6.1075" x2="-35.9955" y2="6.1253" layer="94"/>
<rectangle x1="-39.9427" y1="6.1253" x2="-39.8538" y2="6.143" layer="94"/>
<rectangle x1="-38.467" y1="6.1253" x2="-37.4713" y2="6.143" layer="94"/>
<rectangle x1="-36.0844" y1="6.1253" x2="-35.9955" y2="6.143" layer="94"/>
<rectangle x1="-39.9427" y1="6.143" x2="-39.9071" y2="6.1608" layer="94"/>
<rectangle x1="-38.627" y1="6.143" x2="-37.3113" y2="6.1608" layer="94"/>
<rectangle x1="-36.0311" y1="6.143" x2="-35.9955" y2="6.1608" layer="94"/>
<rectangle x1="-38.7514" y1="6.1608" x2="-37.1868" y2="6.1786" layer="94"/>
<rectangle x1="-38.8759" y1="6.1786" x2="-37.0623" y2="6.1964" layer="94"/>
<rectangle x1="-38.9826" y1="6.1964" x2="-36.9557" y2="6.2142" layer="94"/>
<rectangle x1="-39.0715" y1="6.2142" x2="-36.8668" y2="6.2319" layer="94"/>
<rectangle x1="-39.1604" y1="6.2319" x2="-36.7779" y2="6.2497" layer="94"/>
<rectangle x1="-39.2315" y1="6.2497" x2="-36.7067" y2="6.2675" layer="94"/>
<rectangle x1="-39.3026" y1="6.2675" x2="-36.6356" y2="6.2853" layer="94"/>
<rectangle x1="-39.3737" y1="6.2853" x2="-36.5645" y2="6.3031" layer="94"/>
<rectangle x1="-39.4449" y1="6.3031" x2="-36.4934" y2="6.3208" layer="94"/>
<rectangle x1="-39.4982" y1="6.3208" x2="-36.44" y2="6.3386" layer="94"/>
<rectangle x1="-39.5693" y1="6.3386" x2="-36.3689" y2="6.3564" layer="94"/>
<rectangle x1="-39.6227" y1="6.3564" x2="-38.218" y2="6.3742" layer="94"/>
<rectangle x1="-37.7202" y1="6.3564" x2="-36.3156" y2="6.3742" layer="94"/>
<rectangle x1="-39.6582" y1="6.3742" x2="-38.4847" y2="6.392" layer="94"/>
<rectangle x1="-37.4535" y1="6.3742" x2="-36.28" y2="6.392" layer="94"/>
<rectangle x1="-39.6582" y1="6.392" x2="-38.6625" y2="6.4097" layer="94"/>
<rectangle x1="-37.2757" y1="6.392" x2="-36.28" y2="6.4097" layer="94"/>
<rectangle x1="-39.6582" y1="6.4097" x2="-38.787" y2="6.4275" layer="94"/>
<rectangle x1="-37.1512" y1="6.4097" x2="-36.28" y2="6.4275" layer="94"/>
<rectangle x1="-39.6582" y1="6.4275" x2="-38.9115" y2="6.4453" layer="94"/>
<rectangle x1="-37.0268" y1="6.4275" x2="-36.28" y2="6.4453" layer="94"/>
<rectangle x1="-39.6582" y1="6.4453" x2="-39.0004" y2="6.4631" layer="94"/>
<rectangle x1="-36.9379" y1="6.4453" x2="-36.28" y2="6.4631" layer="94"/>
<rectangle x1="-39.6582" y1="6.4631" x2="-39.0893" y2="6.4809" layer="94"/>
<rectangle x1="-36.849" y1="6.4631" x2="-36.28" y2="6.4809" layer="94"/>
<rectangle x1="-39.6582" y1="6.4809" x2="-39.1604" y2="6.4986" layer="94"/>
<rectangle x1="-36.7779" y1="6.4809" x2="-36.28" y2="6.4986" layer="94"/>
<rectangle x1="-39.6582" y1="6.4986" x2="-39.2315" y2="6.5164" layer="94"/>
<rectangle x1="-36.7067" y1="6.4986" x2="-36.28" y2="6.5164" layer="94"/>
<rectangle x1="-39.6582" y1="6.5164" x2="-39.3026" y2="6.5342" layer="94"/>
<rectangle x1="-36.6356" y1="6.5164" x2="-36.28" y2="6.5342" layer="94"/>
<rectangle x1="-39.6582" y1="6.5342" x2="-39.356" y2="6.552" layer="94"/>
<rectangle x1="-36.5823" y1="6.5342" x2="-36.28" y2="6.552" layer="94"/>
<rectangle x1="-39.6582" y1="6.552" x2="-39.4093" y2="6.5698" layer="94"/>
<rectangle x1="-36.5289" y1="6.552" x2="-36.28" y2="6.5698" layer="94"/>
<rectangle x1="-39.6582" y1="6.5698" x2="-39.4271" y2="6.5875" layer="94"/>
<rectangle x1="-36.5112" y1="6.5698" x2="-36.28" y2="6.5875" layer="94"/>
<rectangle x1="-39.6582" y1="6.5875" x2="-39.4271" y2="6.6053" layer="94"/>
<rectangle x1="-36.5112" y1="6.5875" x2="-36.28" y2="6.6053" layer="94"/>
<rectangle x1="-39.6582" y1="6.6053" x2="-39.4271" y2="6.6231" layer="94"/>
<rectangle x1="-36.5112" y1="6.6053" x2="-36.28" y2="6.6231" layer="94"/>
<rectangle x1="-39.6582" y1="6.6231" x2="-39.4271" y2="6.6409" layer="94"/>
<rectangle x1="-36.5112" y1="6.6231" x2="-36.28" y2="6.6409" layer="94"/>
<rectangle x1="-39.6582" y1="6.6409" x2="-39.4271" y2="6.6587" layer="94"/>
<rectangle x1="-36.5112" y1="6.6409" x2="-36.28" y2="6.6587" layer="94"/>
<rectangle x1="-39.6582" y1="6.6587" x2="-39.4271" y2="6.6764" layer="94"/>
<rectangle x1="-36.5112" y1="6.6587" x2="-36.28" y2="6.6764" layer="94"/>
<rectangle x1="-39.6582" y1="6.6764" x2="-39.4271" y2="6.6942" layer="94"/>
<rectangle x1="-36.5112" y1="6.6764" x2="-36.28" y2="6.6942" layer="94"/>
<rectangle x1="-39.6582" y1="6.6942" x2="-39.4271" y2="6.712" layer="94"/>
<rectangle x1="-36.5112" y1="6.6942" x2="-36.28" y2="6.712" layer="94"/>
<rectangle x1="-39.6582" y1="6.712" x2="-39.4271" y2="6.7298" layer="94"/>
<rectangle x1="-36.5112" y1="6.712" x2="-36.28" y2="6.7298" layer="94"/>
<rectangle x1="-39.6582" y1="6.7298" x2="-39.4271" y2="6.7476" layer="94"/>
<rectangle x1="-36.5112" y1="6.7298" x2="-36.28" y2="6.7476" layer="94"/>
<rectangle x1="-39.6582" y1="6.7476" x2="-39.4271" y2="6.7653" layer="94"/>
<rectangle x1="-36.5112" y1="6.7476" x2="-36.28" y2="6.7653" layer="94"/>
<rectangle x1="-39.6582" y1="6.7653" x2="-39.4271" y2="6.7831" layer="94"/>
<rectangle x1="-36.5112" y1="6.7653" x2="-36.28" y2="6.7831" layer="94"/>
<rectangle x1="-39.6582" y1="6.7831" x2="-39.4271" y2="6.8009" layer="94"/>
<rectangle x1="-36.7956" y1="6.7831" x2="-36.7779" y2="6.8009" layer="94"/>
<rectangle x1="-36.5112" y1="6.7831" x2="-36.28" y2="6.8009" layer="94"/>
<rectangle x1="-39.6582" y1="6.8009" x2="-39.4271" y2="6.8187" layer="94"/>
<rectangle x1="-36.7956" y1="6.8009" x2="-36.7779" y2="6.8187" layer="94"/>
<rectangle x1="-36.5112" y1="6.8009" x2="-36.28" y2="6.8187" layer="94"/>
<rectangle x1="-39.6582" y1="6.8187" x2="-39.4271" y2="6.8365" layer="94"/>
<rectangle x1="-36.7956" y1="6.8187" x2="-36.7779" y2="6.8365" layer="94"/>
<rectangle x1="-36.5112" y1="6.8187" x2="-36.28" y2="6.8365" layer="94"/>
<rectangle x1="-39.6582" y1="6.8365" x2="-39.4271" y2="6.8542" layer="94"/>
<rectangle x1="-36.7956" y1="6.8365" x2="-36.7779" y2="6.8542" layer="94"/>
<rectangle x1="-36.5112" y1="6.8365" x2="-36.28" y2="6.8542" layer="94"/>
<rectangle x1="-39.6582" y1="6.8542" x2="-39.4271" y2="6.872" layer="94"/>
<rectangle x1="-36.7956" y1="6.8542" x2="-36.7779" y2="6.872" layer="94"/>
<rectangle x1="-36.5112" y1="6.8542" x2="-36.28" y2="6.872" layer="94"/>
<rectangle x1="-39.6582" y1="6.872" x2="-39.4271" y2="6.8898" layer="94"/>
<rectangle x1="-36.7956" y1="6.872" x2="-36.7779" y2="6.8898" layer="94"/>
<rectangle x1="-36.5112" y1="6.872" x2="-36.28" y2="6.8898" layer="94"/>
<rectangle x1="-39.6582" y1="6.8898" x2="-39.4271" y2="6.9076" layer="94"/>
<rectangle x1="-36.8134" y1="6.8898" x2="-36.7779" y2="6.9076" layer="94"/>
<rectangle x1="-36.5112" y1="6.8898" x2="-36.28" y2="6.9076" layer="94"/>
<rectangle x1="-39.6582" y1="6.9076" x2="-39.4271" y2="6.9254" layer="94"/>
<rectangle x1="-36.8134" y1="6.9076" x2="-36.7779" y2="6.9254" layer="94"/>
<rectangle x1="-36.5112" y1="6.9076" x2="-36.28" y2="6.9254" layer="94"/>
<rectangle x1="-39.6582" y1="6.9254" x2="-39.4271" y2="6.9431" layer="94"/>
<rectangle x1="-36.8134" y1="6.9254" x2="-36.7779" y2="6.9431" layer="94"/>
<rectangle x1="-36.5112" y1="6.9254" x2="-36.28" y2="6.9431" layer="94"/>
<rectangle x1="-39.6582" y1="6.9431" x2="-39.4271" y2="6.9609" layer="94"/>
<rectangle x1="-36.8134" y1="6.9431" x2="-36.7779" y2="6.9609" layer="94"/>
<rectangle x1="-36.5112" y1="6.9431" x2="-36.28" y2="6.9609" layer="94"/>
<rectangle x1="-39.6582" y1="6.9609" x2="-39.4271" y2="6.9787" layer="94"/>
<rectangle x1="-36.8134" y1="6.9609" x2="-36.7779" y2="6.9787" layer="94"/>
<rectangle x1="-36.5112" y1="6.9609" x2="-36.28" y2="6.9787" layer="94"/>
<rectangle x1="-39.6582" y1="6.9787" x2="-39.4271" y2="6.9965" layer="94"/>
<rectangle x1="-36.8312" y1="6.9787" x2="-36.7779" y2="6.9965" layer="94"/>
<rectangle x1="-36.5112" y1="6.9787" x2="-36.28" y2="6.9965" layer="94"/>
<rectangle x1="-39.6582" y1="6.9965" x2="-39.4271" y2="7.0143" layer="94"/>
<rectangle x1="-36.8312" y1="6.9965" x2="-36.7779" y2="7.0143" layer="94"/>
<rectangle x1="-36.5112" y1="6.9965" x2="-36.28" y2="7.0143" layer="94"/>
<rectangle x1="-39.6582" y1="7.0143" x2="-39.4271" y2="7.032" layer="94"/>
<rectangle x1="-36.8312" y1="7.0143" x2="-36.7779" y2="7.032" layer="94"/>
<rectangle x1="-36.5112" y1="7.0143" x2="-36.28" y2="7.032" layer="94"/>
<rectangle x1="-39.6582" y1="7.032" x2="-39.4271" y2="7.0498" layer="94"/>
<rectangle x1="-36.8312" y1="7.032" x2="-36.7779" y2="7.0498" layer="94"/>
<rectangle x1="-36.5112" y1="7.032" x2="-36.28" y2="7.0498" layer="94"/>
<rectangle x1="-39.6582" y1="7.0498" x2="-39.4271" y2="7.0676" layer="94"/>
<rectangle x1="-36.8312" y1="7.0498" x2="-36.7779" y2="7.0676" layer="94"/>
<rectangle x1="-36.5112" y1="7.0498" x2="-36.28" y2="7.0676" layer="94"/>
<rectangle x1="-39.6582" y1="7.0676" x2="-39.4271" y2="7.0854" layer="94"/>
<rectangle x1="-36.849" y1="7.0676" x2="-36.7779" y2="7.0854" layer="94"/>
<rectangle x1="-36.5112" y1="7.0676" x2="-36.28" y2="7.0854" layer="94"/>
<rectangle x1="-39.6582" y1="7.0854" x2="-39.4271" y2="7.1032" layer="94"/>
<rectangle x1="-36.849" y1="7.0854" x2="-36.7779" y2="7.1032" layer="94"/>
<rectangle x1="-36.5112" y1="7.0854" x2="-36.28" y2="7.1032" layer="94"/>
<rectangle x1="-39.6582" y1="7.1032" x2="-39.4271" y2="7.1209" layer="94"/>
<rectangle x1="-36.849" y1="7.1032" x2="-36.7779" y2="7.1209" layer="94"/>
<rectangle x1="-36.5112" y1="7.1032" x2="-36.28" y2="7.1209" layer="94"/>
<rectangle x1="-39.6582" y1="7.1209" x2="-39.4271" y2="7.1387" layer="94"/>
<rectangle x1="-36.849" y1="7.1209" x2="-36.7779" y2="7.1387" layer="94"/>
<rectangle x1="-36.5112" y1="7.1209" x2="-36.28" y2="7.1387" layer="94"/>
<rectangle x1="-39.6582" y1="7.1387" x2="-39.4271" y2="7.1565" layer="94"/>
<rectangle x1="-36.849" y1="7.1387" x2="-36.7779" y2="7.1565" layer="94"/>
<rectangle x1="-36.5112" y1="7.1387" x2="-36.28" y2="7.1565" layer="94"/>
<rectangle x1="-39.6582" y1="7.1565" x2="-39.4271" y2="7.1743" layer="94"/>
<rectangle x1="-36.8668" y1="7.1565" x2="-36.7779" y2="7.1743" layer="94"/>
<rectangle x1="-36.5112" y1="7.1565" x2="-36.28" y2="7.1743" layer="94"/>
<rectangle x1="-39.6582" y1="7.1743" x2="-39.4271" y2="7.1921" layer="94"/>
<rectangle x1="-36.8668" y1="7.1743" x2="-36.7779" y2="7.1921" layer="94"/>
<rectangle x1="-36.5112" y1="7.1743" x2="-36.28" y2="7.1921" layer="94"/>
<rectangle x1="-39.6582" y1="7.1921" x2="-39.4271" y2="7.2098" layer="94"/>
<rectangle x1="-36.8668" y1="7.1921" x2="-36.7779" y2="7.2098" layer="94"/>
<rectangle x1="-36.5112" y1="7.1921" x2="-36.28" y2="7.2098" layer="94"/>
<rectangle x1="-39.6582" y1="7.2098" x2="-39.4271" y2="7.2276" layer="94"/>
<rectangle x1="-36.8668" y1="7.2098" x2="-36.7779" y2="7.2276" layer="94"/>
<rectangle x1="-36.5112" y1="7.2098" x2="-36.28" y2="7.2276" layer="94"/>
<rectangle x1="-39.6582" y1="7.2276" x2="-39.4271" y2="7.2454" layer="94"/>
<rectangle x1="-36.8845" y1="7.2276" x2="-36.7779" y2="7.2454" layer="94"/>
<rectangle x1="-36.5112" y1="7.2276" x2="-36.28" y2="7.2454" layer="94"/>
<rectangle x1="-39.6582" y1="7.2454" x2="-39.4271" y2="7.2632" layer="94"/>
<rectangle x1="-36.8845" y1="7.2454" x2="-36.7779" y2="7.2632" layer="94"/>
<rectangle x1="-36.5112" y1="7.2454" x2="-36.28" y2="7.2632" layer="94"/>
<rectangle x1="-39.6582" y1="7.2632" x2="-39.4271" y2="7.281" layer="94"/>
<rectangle x1="-36.8845" y1="7.2632" x2="-36.7779" y2="7.281" layer="94"/>
<rectangle x1="-36.5112" y1="7.2632" x2="-36.28" y2="7.281" layer="94"/>
<rectangle x1="-39.6582" y1="7.281" x2="-39.4271" y2="7.2987" layer="94"/>
<rectangle x1="-36.8845" y1="7.281" x2="-36.7779" y2="7.2987" layer="94"/>
<rectangle x1="-36.5112" y1="7.281" x2="-36.28" y2="7.2987" layer="94"/>
<rectangle x1="-39.6582" y1="7.2987" x2="-39.4271" y2="7.3165" layer="94"/>
<rectangle x1="-36.9023" y1="7.2987" x2="-36.7779" y2="7.3165" layer="94"/>
<rectangle x1="-36.5112" y1="7.2987" x2="-36.28" y2="7.3165" layer="94"/>
<rectangle x1="-39.6582" y1="7.3165" x2="-39.4271" y2="7.3343" layer="94"/>
<rectangle x1="-36.9023" y1="7.3165" x2="-36.7779" y2="7.3343" layer="94"/>
<rectangle x1="-36.5112" y1="7.3165" x2="-36.28" y2="7.3343" layer="94"/>
<rectangle x1="-39.6582" y1="7.3343" x2="-39.4271" y2="7.3521" layer="94"/>
<rectangle x1="-36.9023" y1="7.3343" x2="-36.7779" y2="7.3521" layer="94"/>
<rectangle x1="-36.5112" y1="7.3343" x2="-36.28" y2="7.3521" layer="94"/>
<rectangle x1="-39.6582" y1="7.3521" x2="-39.4271" y2="7.3699" layer="94"/>
<rectangle x1="-36.9023" y1="7.3521" x2="-36.7779" y2="7.3699" layer="94"/>
<rectangle x1="-36.5112" y1="7.3521" x2="-36.28" y2="7.3699" layer="94"/>
<rectangle x1="-39.6582" y1="7.3699" x2="-39.4271" y2="7.3876" layer="94"/>
<rectangle x1="-36.9201" y1="7.3699" x2="-36.7779" y2="7.3876" layer="94"/>
<rectangle x1="-36.5112" y1="7.3699" x2="-36.28" y2="7.3876" layer="94"/>
<rectangle x1="-39.6582" y1="7.3876" x2="-39.4271" y2="7.4054" layer="94"/>
<rectangle x1="-36.9201" y1="7.3876" x2="-36.7779" y2="7.4054" layer="94"/>
<rectangle x1="-36.5112" y1="7.3876" x2="-36.28" y2="7.4054" layer="94"/>
<rectangle x1="-39.6582" y1="7.4054" x2="-39.4271" y2="7.4232" layer="94"/>
<rectangle x1="-36.9201" y1="7.4054" x2="-36.7779" y2="7.4232" layer="94"/>
<rectangle x1="-36.5112" y1="7.4054" x2="-36.28" y2="7.4232" layer="94"/>
<rectangle x1="-39.6582" y1="7.4232" x2="-39.4271" y2="7.441" layer="94"/>
<rectangle x1="-36.9379" y1="7.4232" x2="-36.7779" y2="7.441" layer="94"/>
<rectangle x1="-36.5112" y1="7.4232" x2="-36.28" y2="7.441" layer="94"/>
<rectangle x1="-39.6582" y1="7.441" x2="-39.4271" y2="7.4588" layer="94"/>
<rectangle x1="-36.9379" y1="7.441" x2="-36.7779" y2="7.4588" layer="94"/>
<rectangle x1="-36.5112" y1="7.441" x2="-36.28" y2="7.4588" layer="94"/>
<rectangle x1="-39.6582" y1="7.4588" x2="-39.4271" y2="7.4765" layer="94"/>
<rectangle x1="-36.9379" y1="7.4588" x2="-36.7779" y2="7.4765" layer="94"/>
<rectangle x1="-36.5112" y1="7.4588" x2="-36.28" y2="7.4765" layer="94"/>
<rectangle x1="-39.6582" y1="7.4765" x2="-39.4271" y2="7.4943" layer="94"/>
<rectangle x1="-36.9557" y1="7.4765" x2="-36.7779" y2="7.4943" layer="94"/>
<rectangle x1="-36.5112" y1="7.4765" x2="-36.28" y2="7.4943" layer="94"/>
<rectangle x1="-39.6582" y1="7.4943" x2="-39.4271" y2="7.5121" layer="94"/>
<rectangle x1="-36.9557" y1="7.4943" x2="-36.7779" y2="7.5121" layer="94"/>
<rectangle x1="-36.5112" y1="7.4943" x2="-36.28" y2="7.5121" layer="94"/>
<rectangle x1="-39.6582" y1="7.5121" x2="-39.4271" y2="7.5299" layer="94"/>
<rectangle x1="-36.9557" y1="7.5121" x2="-36.7779" y2="7.5299" layer="94"/>
<rectangle x1="-36.5112" y1="7.5121" x2="-36.28" y2="7.5299" layer="94"/>
<rectangle x1="-39.6582" y1="7.5299" x2="-39.4271" y2="7.5477" layer="94"/>
<rectangle x1="-36.9734" y1="7.5299" x2="-36.7779" y2="7.5477" layer="94"/>
<rectangle x1="-36.5112" y1="7.5299" x2="-36.28" y2="7.5477" layer="94"/>
<rectangle x1="-39.6582" y1="7.5477" x2="-39.4271" y2="7.5654" layer="94"/>
<rectangle x1="-36.9734" y1="7.5477" x2="-36.7779" y2="7.5654" layer="94"/>
<rectangle x1="-36.5112" y1="7.5477" x2="-36.28" y2="7.5654" layer="94"/>
<rectangle x1="-39.6582" y1="7.5654" x2="-39.4271" y2="7.5832" layer="94"/>
<rectangle x1="-36.9734" y1="7.5654" x2="-36.7779" y2="7.5832" layer="94"/>
<rectangle x1="-36.5112" y1="7.5654" x2="-36.28" y2="7.5832" layer="94"/>
<rectangle x1="-39.6582" y1="7.5832" x2="-39.4271" y2="7.601" layer="94"/>
<rectangle x1="-36.9912" y1="7.5832" x2="-36.7779" y2="7.601" layer="94"/>
<rectangle x1="-36.5112" y1="7.5832" x2="-36.28" y2="7.601" layer="94"/>
<rectangle x1="-39.6582" y1="7.601" x2="-39.4271" y2="7.6188" layer="94"/>
<rectangle x1="-36.9912" y1="7.601" x2="-36.7779" y2="7.6188" layer="94"/>
<rectangle x1="-36.5112" y1="7.601" x2="-36.28" y2="7.6188" layer="94"/>
<rectangle x1="-39.6582" y1="7.6188" x2="-39.4271" y2="7.6366" layer="94"/>
<rectangle x1="-36.9912" y1="7.6188" x2="-36.7779" y2="7.6366" layer="94"/>
<rectangle x1="-36.5112" y1="7.6188" x2="-36.28" y2="7.6366" layer="94"/>
<rectangle x1="-39.6582" y1="7.6366" x2="-39.4271" y2="7.6543" layer="94"/>
<rectangle x1="-37.009" y1="7.6366" x2="-36.7779" y2="7.6543" layer="94"/>
<rectangle x1="-36.5112" y1="7.6366" x2="-36.28" y2="7.6543" layer="94"/>
<rectangle x1="-39.6582" y1="7.6543" x2="-39.4271" y2="7.6721" layer="94"/>
<rectangle x1="-37.009" y1="7.6543" x2="-36.7779" y2="7.6721" layer="94"/>
<rectangle x1="-36.5112" y1="7.6543" x2="-36.28" y2="7.6721" layer="94"/>
<rectangle x1="-39.6582" y1="7.6721" x2="-39.4271" y2="7.6899" layer="94"/>
<rectangle x1="-37.009" y1="7.6721" x2="-36.7779" y2="7.6899" layer="94"/>
<rectangle x1="-36.5112" y1="7.6721" x2="-36.28" y2="7.6899" layer="94"/>
<rectangle x1="-39.6582" y1="7.6899" x2="-39.4271" y2="7.7077" layer="94"/>
<rectangle x1="-37.0268" y1="7.6899" x2="-36.7779" y2="7.7077" layer="94"/>
<rectangle x1="-36.5112" y1="7.6899" x2="-36.28" y2="7.7077" layer="94"/>
<rectangle x1="-39.6582" y1="7.7077" x2="-39.4271" y2="7.7255" layer="94"/>
<rectangle x1="-37.0268" y1="7.7077" x2="-36.7779" y2="7.7255" layer="94"/>
<rectangle x1="-36.5112" y1="7.7077" x2="-36.28" y2="7.7255" layer="94"/>
<rectangle x1="-39.6582" y1="7.7255" x2="-39.4271" y2="7.7432" layer="94"/>
<rectangle x1="-37.0268" y1="7.7255" x2="-36.7779" y2="7.7432" layer="94"/>
<rectangle x1="-36.5112" y1="7.7255" x2="-36.28" y2="7.7432" layer="94"/>
<rectangle x1="-39.6582" y1="7.7432" x2="-39.4271" y2="7.761" layer="94"/>
<rectangle x1="-37.0446" y1="7.7432" x2="-36.7779" y2="7.761" layer="94"/>
<rectangle x1="-36.5112" y1="7.7432" x2="-36.28" y2="7.761" layer="94"/>
<rectangle x1="-39.6582" y1="7.761" x2="-39.4271" y2="7.7788" layer="94"/>
<rectangle x1="-37.0446" y1="7.761" x2="-36.7779" y2="7.7788" layer="94"/>
<rectangle x1="-36.5112" y1="7.761" x2="-36.28" y2="7.7788" layer="94"/>
<rectangle x1="-39.6582" y1="7.7788" x2="-39.4271" y2="7.7966" layer="94"/>
<rectangle x1="-37.0623" y1="7.7788" x2="-36.7779" y2="7.7966" layer="94"/>
<rectangle x1="-36.5112" y1="7.7788" x2="-36.28" y2="7.7966" layer="94"/>
<rectangle x1="-39.6582" y1="7.7966" x2="-39.4271" y2="7.8144" layer="94"/>
<rectangle x1="-37.0623" y1="7.7966" x2="-36.7779" y2="7.8144" layer="94"/>
<rectangle x1="-36.5112" y1="7.7966" x2="-36.28" y2="7.8144" layer="94"/>
<rectangle x1="-39.6582" y1="7.8144" x2="-39.4271" y2="7.8321" layer="94"/>
<rectangle x1="-37.0623" y1="7.8144" x2="-36.7779" y2="7.8321" layer="94"/>
<rectangle x1="-36.5112" y1="7.8144" x2="-36.28" y2="7.8321" layer="94"/>
<rectangle x1="-39.6582" y1="7.8321" x2="-39.4271" y2="7.8499" layer="94"/>
<rectangle x1="-37.0801" y1="7.8321" x2="-36.7779" y2="7.8499" layer="94"/>
<rectangle x1="-36.5112" y1="7.8321" x2="-36.28" y2="7.8499" layer="94"/>
<rectangle x1="-39.6582" y1="7.8499" x2="-39.4271" y2="7.8677" layer="94"/>
<rectangle x1="-37.0801" y1="7.8499" x2="-36.7779" y2="7.8677" layer="94"/>
<rectangle x1="-36.5112" y1="7.8499" x2="-36.28" y2="7.8677" layer="94"/>
<rectangle x1="-39.6582" y1="7.8677" x2="-39.4271" y2="7.8855" layer="94"/>
<rectangle x1="-37.0979" y1="7.8677" x2="-36.7779" y2="7.8855" layer="94"/>
<rectangle x1="-36.5112" y1="7.8677" x2="-36.28" y2="7.8855" layer="94"/>
<rectangle x1="-39.6582" y1="7.8855" x2="-39.4271" y2="7.9033" layer="94"/>
<rectangle x1="-37.0979" y1="7.8855" x2="-36.7779" y2="7.9033" layer="94"/>
<rectangle x1="-36.5112" y1="7.8855" x2="-36.28" y2="7.9033" layer="94"/>
<rectangle x1="-39.6582" y1="7.9033" x2="-39.4271" y2="7.921" layer="94"/>
<rectangle x1="-37.0979" y1="7.9033" x2="-36.7779" y2="7.921" layer="94"/>
<rectangle x1="-36.5112" y1="7.9033" x2="-36.28" y2="7.921" layer="94"/>
<rectangle x1="-39.6582" y1="7.921" x2="-39.4271" y2="7.9388" layer="94"/>
<rectangle x1="-37.1157" y1="7.921" x2="-36.7779" y2="7.9388" layer="94"/>
<rectangle x1="-36.5112" y1="7.921" x2="-36.28" y2="7.9388" layer="94"/>
<rectangle x1="-39.6582" y1="7.9388" x2="-39.4271" y2="7.9566" layer="94"/>
<rectangle x1="-37.1157" y1="7.9388" x2="-36.7779" y2="7.9566" layer="94"/>
<rectangle x1="-36.5112" y1="7.9388" x2="-36.28" y2="7.9566" layer="94"/>
<rectangle x1="-39.6582" y1="7.9566" x2="-39.4271" y2="7.9744" layer="94"/>
<rectangle x1="-37.1335" y1="7.9566" x2="-36.7779" y2="7.9744" layer="94"/>
<rectangle x1="-36.5112" y1="7.9566" x2="-36.28" y2="7.9744" layer="94"/>
<rectangle x1="-39.6582" y1="7.9744" x2="-39.4271" y2="7.9922" layer="94"/>
<rectangle x1="-37.1335" y1="7.9744" x2="-36.7779" y2="7.9922" layer="94"/>
<rectangle x1="-36.5112" y1="7.9744" x2="-36.28" y2="7.9922" layer="94"/>
<rectangle x1="-39.6582" y1="7.9922" x2="-39.4271" y2="8.0099" layer="94"/>
<rectangle x1="-37.1512" y1="7.9922" x2="-36.7779" y2="8.0099" layer="94"/>
<rectangle x1="-36.5112" y1="7.9922" x2="-36.28" y2="8.0099" layer="94"/>
<rectangle x1="-39.6582" y1="8.0099" x2="-39.4271" y2="8.0277" layer="94"/>
<rectangle x1="-37.1512" y1="8.0099" x2="-36.7779" y2="8.0277" layer="94"/>
<rectangle x1="-36.5112" y1="8.0099" x2="-36.28" y2="8.0277" layer="94"/>
<rectangle x1="-39.6582" y1="8.0277" x2="-39.4271" y2="8.0455" layer="94"/>
<rectangle x1="-37.169" y1="8.0277" x2="-36.7779" y2="8.0455" layer="94"/>
<rectangle x1="-36.5112" y1="8.0277" x2="-36.28" y2="8.0455" layer="94"/>
<rectangle x1="-39.6582" y1="8.0455" x2="-39.4271" y2="8.0633" layer="94"/>
<rectangle x1="-37.169" y1="8.0455" x2="-36.7779" y2="8.0633" layer="94"/>
<rectangle x1="-36.5112" y1="8.0455" x2="-36.28" y2="8.0633" layer="94"/>
<rectangle x1="-39.6582" y1="8.0633" x2="-39.4271" y2="8.0811" layer="94"/>
<rectangle x1="-37.169" y1="8.0633" x2="-36.7779" y2="8.0811" layer="94"/>
<rectangle x1="-36.5112" y1="8.0633" x2="-36.28" y2="8.0811" layer="94"/>
<rectangle x1="-39.6582" y1="8.0811" x2="-39.4271" y2="8.0988" layer="94"/>
<rectangle x1="-37.1868" y1="8.0811" x2="-36.7779" y2="8.0988" layer="94"/>
<rectangle x1="-36.5112" y1="8.0811" x2="-36.28" y2="8.0988" layer="94"/>
<rectangle x1="-39.6582" y1="8.0988" x2="-39.4271" y2="8.1166" layer="94"/>
<rectangle x1="-37.1868" y1="8.0988" x2="-36.7779" y2="8.1166" layer="94"/>
<rectangle x1="-36.5112" y1="8.0988" x2="-36.28" y2="8.1166" layer="94"/>
<rectangle x1="-39.6582" y1="8.1166" x2="-39.4271" y2="8.1344" layer="94"/>
<rectangle x1="-37.2046" y1="8.1166" x2="-36.7779" y2="8.1344" layer="94"/>
<rectangle x1="-36.5112" y1="8.1166" x2="-36.28" y2="8.1344" layer="94"/>
<rectangle x1="-39.6582" y1="8.1344" x2="-39.4271" y2="8.1522" layer="94"/>
<rectangle x1="-37.2046" y1="8.1344" x2="-36.7779" y2="8.1522" layer="94"/>
<rectangle x1="-36.5112" y1="8.1344" x2="-36.28" y2="8.1522" layer="94"/>
<rectangle x1="-39.6582" y1="8.1522" x2="-39.4271" y2="8.17" layer="94"/>
<rectangle x1="-37.2224" y1="8.1522" x2="-36.7779" y2="8.17" layer="94"/>
<rectangle x1="-36.5112" y1="8.1522" x2="-36.28" y2="8.17" layer="94"/>
<rectangle x1="-39.6582" y1="8.17" x2="-39.4271" y2="8.1877" layer="94"/>
<rectangle x1="-37.2224" y1="8.17" x2="-36.7779" y2="8.1877" layer="94"/>
<rectangle x1="-36.5112" y1="8.17" x2="-36.28" y2="8.1877" layer="94"/>
<rectangle x1="-39.6582" y1="8.1877" x2="-39.4271" y2="8.2055" layer="94"/>
<rectangle x1="-37.2401" y1="8.1877" x2="-36.7779" y2="8.2055" layer="94"/>
<rectangle x1="-36.5112" y1="8.1877" x2="-36.28" y2="8.2055" layer="94"/>
<rectangle x1="-39.6582" y1="8.2055" x2="-39.4271" y2="8.2233" layer="94"/>
<rectangle x1="-37.2401" y1="8.2055" x2="-36.7779" y2="8.2233" layer="94"/>
<rectangle x1="-36.5112" y1="8.2055" x2="-36.28" y2="8.2233" layer="94"/>
<rectangle x1="-39.6582" y1="8.2233" x2="-39.4271" y2="8.2411" layer="94"/>
<rectangle x1="-37.2579" y1="8.2233" x2="-36.7779" y2="8.2411" layer="94"/>
<rectangle x1="-36.5112" y1="8.2233" x2="-36.28" y2="8.2411" layer="94"/>
<rectangle x1="-39.6582" y1="8.2411" x2="-39.4271" y2="8.2589" layer="94"/>
<rectangle x1="-37.2579" y1="8.2411" x2="-36.7779" y2="8.2589" layer="94"/>
<rectangle x1="-36.5112" y1="8.2411" x2="-36.28" y2="8.2589" layer="94"/>
<rectangle x1="-39.6582" y1="8.2589" x2="-39.4271" y2="8.2766" layer="94"/>
<rectangle x1="-37.2757" y1="8.2589" x2="-36.7779" y2="8.2766" layer="94"/>
<rectangle x1="-36.5112" y1="8.2589" x2="-36.28" y2="8.2766" layer="94"/>
<rectangle x1="-39.6582" y1="8.2766" x2="-39.4271" y2="8.2944" layer="94"/>
<rectangle x1="-37.2757" y1="8.2766" x2="-36.7779" y2="8.2944" layer="94"/>
<rectangle x1="-36.5112" y1="8.2766" x2="-36.28" y2="8.2944" layer="94"/>
<rectangle x1="-39.6582" y1="8.2944" x2="-39.4271" y2="8.3122" layer="94"/>
<rectangle x1="-37.2935" y1="8.2944" x2="-36.7779" y2="8.3122" layer="94"/>
<rectangle x1="-36.5112" y1="8.2944" x2="-36.28" y2="8.3122" layer="94"/>
<rectangle x1="-39.6582" y1="8.3122" x2="-39.4271" y2="8.33" layer="94"/>
<rectangle x1="-37.3113" y1="8.3122" x2="-36.7779" y2="8.33" layer="94"/>
<rectangle x1="-36.5112" y1="8.3122" x2="-36.28" y2="8.33" layer="94"/>
<rectangle x1="-39.6582" y1="8.33" x2="-39.4271" y2="8.3478" layer="94"/>
<rectangle x1="-37.3113" y1="8.33" x2="-36.7779" y2="8.3478" layer="94"/>
<rectangle x1="-36.5112" y1="8.33" x2="-36.28" y2="8.3478" layer="94"/>
<rectangle x1="-39.6582" y1="8.3478" x2="-39.4271" y2="8.3655" layer="94"/>
<rectangle x1="-37.329" y1="8.3478" x2="-36.7779" y2="8.3655" layer="94"/>
<rectangle x1="-36.5112" y1="8.3478" x2="-36.28" y2="8.3655" layer="94"/>
<rectangle x1="-39.6582" y1="8.3655" x2="-39.4271" y2="8.3833" layer="94"/>
<rectangle x1="-37.329" y1="8.3655" x2="-36.7779" y2="8.3833" layer="94"/>
<rectangle x1="-36.5112" y1="8.3655" x2="-36.28" y2="8.3833" layer="94"/>
<rectangle x1="-39.6582" y1="8.3833" x2="-39.4271" y2="8.4011" layer="94"/>
<rectangle x1="-37.3468" y1="8.3833" x2="-36.7779" y2="8.4011" layer="94"/>
<rectangle x1="-36.5112" y1="8.3833" x2="-36.28" y2="8.4011" layer="94"/>
<rectangle x1="-39.6582" y1="8.4011" x2="-39.4271" y2="8.4189" layer="94"/>
<rectangle x1="-37.3468" y1="8.4011" x2="-36.7779" y2="8.4189" layer="94"/>
<rectangle x1="-36.5112" y1="8.4011" x2="-36.28" y2="8.4189" layer="94"/>
<rectangle x1="-39.6582" y1="8.4189" x2="-39.4271" y2="8.4367" layer="94"/>
<rectangle x1="-37.3646" y1="8.4189" x2="-36.7779" y2="8.4367" layer="94"/>
<rectangle x1="-36.5112" y1="8.4189" x2="-36.28" y2="8.4367" layer="94"/>
<rectangle x1="-39.6582" y1="8.4367" x2="-39.4271" y2="8.4544" layer="94"/>
<rectangle x1="-37.3646" y1="8.4367" x2="-36.7779" y2="8.4544" layer="94"/>
<rectangle x1="-36.5112" y1="8.4367" x2="-36.28" y2="8.4544" layer="94"/>
<rectangle x1="-39.6582" y1="8.4544" x2="-39.4271" y2="8.4722" layer="94"/>
<rectangle x1="-37.3824" y1="8.4544" x2="-36.7779" y2="8.4722" layer="94"/>
<rectangle x1="-36.5112" y1="8.4544" x2="-36.28" y2="8.4722" layer="94"/>
<rectangle x1="-39.6582" y1="8.4722" x2="-39.4271" y2="8.49" layer="94"/>
<rectangle x1="-37.4002" y1="8.4722" x2="-36.7779" y2="8.49" layer="94"/>
<rectangle x1="-36.5112" y1="8.4722" x2="-36.28" y2="8.49" layer="94"/>
<rectangle x1="-39.6582" y1="8.49" x2="-39.4271" y2="8.5078" layer="94"/>
<rectangle x1="-37.4002" y1="8.49" x2="-36.7779" y2="8.5078" layer="94"/>
<rectangle x1="-36.5112" y1="8.49" x2="-36.28" y2="8.5078" layer="94"/>
<rectangle x1="-39.6582" y1="8.5078" x2="-39.4271" y2="8.5256" layer="94"/>
<rectangle x1="-37.4179" y1="8.5078" x2="-36.7779" y2="8.5256" layer="94"/>
<rectangle x1="-36.5112" y1="8.5078" x2="-36.28" y2="8.5256" layer="94"/>
<rectangle x1="-39.6582" y1="8.5256" x2="-39.4271" y2="8.5433" layer="94"/>
<rectangle x1="-37.4179" y1="8.5256" x2="-36.7779" y2="8.5433" layer="94"/>
<rectangle x1="-36.5112" y1="8.5256" x2="-36.28" y2="8.5433" layer="94"/>
<rectangle x1="-39.6582" y1="8.5433" x2="-39.4271" y2="8.5611" layer="94"/>
<rectangle x1="-37.4357" y1="8.5433" x2="-36.7779" y2="8.5611" layer="94"/>
<rectangle x1="-36.5112" y1="8.5433" x2="-36.28" y2="8.5611" layer="94"/>
<rectangle x1="-39.6582" y1="8.5611" x2="-39.4271" y2="8.5789" layer="94"/>
<rectangle x1="-37.4535" y1="8.5611" x2="-36.7779" y2="8.5789" layer="94"/>
<rectangle x1="-36.5112" y1="8.5611" x2="-36.28" y2="8.5789" layer="94"/>
<rectangle x1="-39.6582" y1="8.5789" x2="-39.4271" y2="8.5967" layer="94"/>
<rectangle x1="-37.4535" y1="8.5789" x2="-36.7779" y2="8.5967" layer="94"/>
<rectangle x1="-36.5112" y1="8.5789" x2="-36.28" y2="8.5967" layer="94"/>
<rectangle x1="-39.6582" y1="8.5967" x2="-39.4271" y2="8.6145" layer="94"/>
<rectangle x1="-37.4713" y1="8.5967" x2="-36.7779" y2="8.6145" layer="94"/>
<rectangle x1="-36.5112" y1="8.5967" x2="-36.28" y2="8.6145" layer="94"/>
<rectangle x1="-39.6582" y1="8.6145" x2="-39.4271" y2="8.6322" layer="94"/>
<rectangle x1="-37.4891" y1="8.6145" x2="-36.7779" y2="8.6322" layer="94"/>
<rectangle x1="-36.5112" y1="8.6145" x2="-36.28" y2="8.6322" layer="94"/>
<rectangle x1="-39.6582" y1="8.6322" x2="-39.4271" y2="8.65" layer="94"/>
<rectangle x1="-37.4891" y1="8.6322" x2="-36.7779" y2="8.65" layer="94"/>
<rectangle x1="-36.5112" y1="8.6322" x2="-36.28" y2="8.65" layer="94"/>
<rectangle x1="-39.6582" y1="8.65" x2="-39.4271" y2="8.6678" layer="94"/>
<rectangle x1="-37.5068" y1="8.65" x2="-36.7779" y2="8.6678" layer="94"/>
<rectangle x1="-36.5112" y1="8.65" x2="-36.28" y2="8.6678" layer="94"/>
<rectangle x1="-39.6582" y1="8.6678" x2="-39.4271" y2="8.6856" layer="94"/>
<rectangle x1="-37.5246" y1="8.6678" x2="-36.7779" y2="8.6856" layer="94"/>
<rectangle x1="-36.5112" y1="8.6678" x2="-36.28" y2="8.6856" layer="94"/>
<rectangle x1="-39.6582" y1="8.6856" x2="-39.4271" y2="8.7034" layer="94"/>
<rectangle x1="-37.5246" y1="8.6856" x2="-36.7779" y2="8.7034" layer="94"/>
<rectangle x1="-36.5112" y1="8.6856" x2="-36.28" y2="8.7034" layer="94"/>
<rectangle x1="-39.6582" y1="8.7034" x2="-39.4271" y2="8.7211" layer="94"/>
<rectangle x1="-37.5424" y1="8.7034" x2="-36.7779" y2="8.7211" layer="94"/>
<rectangle x1="-36.5112" y1="8.7034" x2="-36.28" y2="8.7211" layer="94"/>
<rectangle x1="-39.6582" y1="8.7211" x2="-39.4271" y2="8.7389" layer="94"/>
<rectangle x1="-37.5602" y1="8.7211" x2="-36.7779" y2="8.7389" layer="94"/>
<rectangle x1="-36.5112" y1="8.7211" x2="-36.28" y2="8.7389" layer="94"/>
<rectangle x1="-39.6582" y1="8.7389" x2="-39.4271" y2="8.7567" layer="94"/>
<rectangle x1="-37.5602" y1="8.7389" x2="-36.7779" y2="8.7567" layer="94"/>
<rectangle x1="-36.5112" y1="8.7389" x2="-36.28" y2="8.7567" layer="94"/>
<rectangle x1="-39.6582" y1="8.7567" x2="-39.4271" y2="8.7745" layer="94"/>
<rectangle x1="-37.578" y1="8.7567" x2="-36.7779" y2="8.7745" layer="94"/>
<rectangle x1="-36.5112" y1="8.7567" x2="-36.28" y2="8.7745" layer="94"/>
<rectangle x1="-39.6582" y1="8.7745" x2="-39.4271" y2="8.7923" layer="94"/>
<rectangle x1="-37.5957" y1="8.7745" x2="-36.7779" y2="8.7923" layer="94"/>
<rectangle x1="-36.5112" y1="8.7745" x2="-36.28" y2="8.7923" layer="94"/>
<rectangle x1="-39.6582" y1="8.7923" x2="-39.4271" y2="8.81" layer="94"/>
<rectangle x1="-37.5957" y1="8.7923" x2="-36.7779" y2="8.81" layer="94"/>
<rectangle x1="-36.5112" y1="8.7923" x2="-36.28" y2="8.81" layer="94"/>
<rectangle x1="-39.6582" y1="8.81" x2="-39.4271" y2="8.8278" layer="94"/>
<rectangle x1="-37.6135" y1="8.81" x2="-36.7779" y2="8.8278" layer="94"/>
<rectangle x1="-36.5112" y1="8.81" x2="-36.28" y2="8.8278" layer="94"/>
<rectangle x1="-39.6582" y1="8.8278" x2="-39.4271" y2="8.8456" layer="94"/>
<rectangle x1="-37.6313" y1="8.8278" x2="-36.7779" y2="8.8456" layer="94"/>
<rectangle x1="-36.5112" y1="8.8278" x2="-36.28" y2="8.8456" layer="94"/>
<rectangle x1="-39.6582" y1="8.8456" x2="-39.4271" y2="8.8634" layer="94"/>
<rectangle x1="-37.6491" y1="8.8456" x2="-36.7779" y2="8.8634" layer="94"/>
<rectangle x1="-36.5112" y1="8.8456" x2="-36.28" y2="8.8634" layer="94"/>
<rectangle x1="-39.6582" y1="8.8634" x2="-39.4271" y2="8.8812" layer="94"/>
<rectangle x1="-37.6491" y1="8.8634" x2="-36.7779" y2="8.8812" layer="94"/>
<rectangle x1="-36.5112" y1="8.8634" x2="-36.28" y2="8.8812" layer="94"/>
<rectangle x1="-39.6582" y1="8.8812" x2="-39.4271" y2="8.8989" layer="94"/>
<rectangle x1="-37.6669" y1="8.8812" x2="-36.7779" y2="8.8989" layer="94"/>
<rectangle x1="-36.5112" y1="8.8812" x2="-36.28" y2="8.8989" layer="94"/>
<rectangle x1="-39.6582" y1="8.8989" x2="-39.4271" y2="8.9167" layer="94"/>
<rectangle x1="-37.6846" y1="8.8989" x2="-36.7779" y2="8.9167" layer="94"/>
<rectangle x1="-36.5112" y1="8.8989" x2="-36.28" y2="8.9167" layer="94"/>
<rectangle x1="-39.6582" y1="8.9167" x2="-39.4271" y2="8.9345" layer="94"/>
<rectangle x1="-37.7024" y1="8.9167" x2="-36.7779" y2="8.9345" layer="94"/>
<rectangle x1="-36.5112" y1="8.9167" x2="-36.28" y2="8.9345" layer="94"/>
<rectangle x1="-39.6582" y1="8.9345" x2="-39.4271" y2="8.9523" layer="94"/>
<rectangle x1="-37.7202" y1="8.9345" x2="-36.7779" y2="8.9523" layer="94"/>
<rectangle x1="-36.5112" y1="8.9345" x2="-36.28" y2="8.9523" layer="94"/>
<rectangle x1="-39.6582" y1="8.9523" x2="-39.4271" y2="8.9701" layer="94"/>
<rectangle x1="-37.7202" y1="8.9523" x2="-36.7779" y2="8.9701" layer="94"/>
<rectangle x1="-36.5112" y1="8.9523" x2="-36.28" y2="8.9701" layer="94"/>
<rectangle x1="-39.6582" y1="8.9701" x2="-39.4271" y2="8.9878" layer="94"/>
<rectangle x1="-37.738" y1="8.9701" x2="-36.7779" y2="8.9878" layer="94"/>
<rectangle x1="-36.5112" y1="8.9701" x2="-36.28" y2="8.9878" layer="94"/>
<rectangle x1="-39.6582" y1="8.9878" x2="-39.4271" y2="9.0056" layer="94"/>
<rectangle x1="-37.7558" y1="8.9878" x2="-36.7779" y2="9.0056" layer="94"/>
<rectangle x1="-36.5112" y1="8.9878" x2="-36.28" y2="9.0056" layer="94"/>
<rectangle x1="-39.6582" y1="9.0056" x2="-39.4271" y2="9.0234" layer="94"/>
<rectangle x1="-37.7735" y1="9.0056" x2="-36.7779" y2="9.0234" layer="94"/>
<rectangle x1="-36.5112" y1="9.0056" x2="-36.28" y2="9.0234" layer="94"/>
<rectangle x1="-39.6582" y1="9.0234" x2="-39.4271" y2="9.0412" layer="94"/>
<rectangle x1="-37.7913" y1="9.0234" x2="-36.7779" y2="9.0412" layer="94"/>
<rectangle x1="-36.5112" y1="9.0234" x2="-36.28" y2="9.0412" layer="94"/>
<rectangle x1="-39.6582" y1="9.0412" x2="-39.4271" y2="9.059" layer="94"/>
<rectangle x1="-37.7913" y1="9.0412" x2="-36.7779" y2="9.059" layer="94"/>
<rectangle x1="-36.5112" y1="9.0412" x2="-36.28" y2="9.059" layer="94"/>
<rectangle x1="-39.6582" y1="9.059" x2="-39.4271" y2="9.0767" layer="94"/>
<rectangle x1="-37.8091" y1="9.059" x2="-36.7779" y2="9.0767" layer="94"/>
<rectangle x1="-36.5112" y1="9.059" x2="-36.28" y2="9.0767" layer="94"/>
<rectangle x1="-39.6582" y1="9.0767" x2="-39.4271" y2="9.0945" layer="94"/>
<rectangle x1="-37.8269" y1="9.0767" x2="-36.7779" y2="9.0945" layer="94"/>
<rectangle x1="-36.5112" y1="9.0767" x2="-36.28" y2="9.0945" layer="94"/>
<rectangle x1="-39.6582" y1="9.0945" x2="-39.4271" y2="9.1123" layer="94"/>
<rectangle x1="-37.8447" y1="9.0945" x2="-36.7779" y2="9.1123" layer="94"/>
<rectangle x1="-36.5112" y1="9.0945" x2="-36.28" y2="9.1123" layer="94"/>
<rectangle x1="-39.6582" y1="9.1123" x2="-39.4271" y2="9.1301" layer="94"/>
<rectangle x1="-37.8624" y1="9.1123" x2="-36.7779" y2="9.1301" layer="94"/>
<rectangle x1="-36.5112" y1="9.1123" x2="-36.28" y2="9.1301" layer="94"/>
<rectangle x1="-39.6582" y1="9.1301" x2="-39.4271" y2="9.1479" layer="94"/>
<rectangle x1="-37.8802" y1="9.1301" x2="-36.7779" y2="9.1479" layer="94"/>
<rectangle x1="-36.5112" y1="9.1301" x2="-36.28" y2="9.1479" layer="94"/>
<rectangle x1="-39.6582" y1="9.1479" x2="-39.4271" y2="9.1656" layer="94"/>
<rectangle x1="-37.898" y1="9.1479" x2="-36.7779" y2="9.1656" layer="94"/>
<rectangle x1="-36.5112" y1="9.1479" x2="-36.28" y2="9.1656" layer="94"/>
<rectangle x1="-39.6582" y1="9.1656" x2="-39.4271" y2="9.1834" layer="94"/>
<rectangle x1="-37.898" y1="9.1656" x2="-36.7779" y2="9.1834" layer="94"/>
<rectangle x1="-36.5112" y1="9.1656" x2="-36.28" y2="9.1834" layer="94"/>
<rectangle x1="-39.6582" y1="9.1834" x2="-39.4271" y2="9.2012" layer="94"/>
<rectangle x1="-37.9158" y1="9.1834" x2="-36.7779" y2="9.2012" layer="94"/>
<rectangle x1="-36.5112" y1="9.1834" x2="-36.28" y2="9.2012" layer="94"/>
<rectangle x1="-39.6582" y1="9.2012" x2="-39.4271" y2="9.219" layer="94"/>
<rectangle x1="-37.9336" y1="9.2012" x2="-36.7779" y2="9.219" layer="94"/>
<rectangle x1="-36.5112" y1="9.2012" x2="-36.28" y2="9.219" layer="94"/>
<rectangle x1="-39.6582" y1="9.219" x2="-39.4271" y2="9.2368" layer="94"/>
<rectangle x1="-37.9513" y1="9.219" x2="-36.7779" y2="9.2368" layer="94"/>
<rectangle x1="-36.5112" y1="9.219" x2="-36.28" y2="9.2368" layer="94"/>
<rectangle x1="-39.6582" y1="9.2368" x2="-39.4271" y2="9.2545" layer="94"/>
<rectangle x1="-37.9691" y1="9.2368" x2="-36.7779" y2="9.2545" layer="94"/>
<rectangle x1="-36.5112" y1="9.2368" x2="-36.28" y2="9.2545" layer="94"/>
<rectangle x1="-39.6582" y1="9.2545" x2="-39.4271" y2="9.2723" layer="94"/>
<rectangle x1="-37.9869" y1="9.2545" x2="-36.7779" y2="9.2723" layer="94"/>
<rectangle x1="-36.5112" y1="9.2545" x2="-36.28" y2="9.2723" layer="94"/>
<rectangle x1="-39.6582" y1="9.2723" x2="-39.4271" y2="9.2901" layer="94"/>
<rectangle x1="-38.0047" y1="9.2723" x2="-36.7779" y2="9.2901" layer="94"/>
<rectangle x1="-36.5112" y1="9.2723" x2="-36.28" y2="9.2901" layer="94"/>
<rectangle x1="-39.6582" y1="9.2901" x2="-39.4271" y2="9.3079" layer="94"/>
<rectangle x1="-38.0225" y1="9.2901" x2="-36.7779" y2="9.3079" layer="94"/>
<rectangle x1="-36.5112" y1="9.2901" x2="-36.28" y2="9.3079" layer="94"/>
<rectangle x1="-39.6582" y1="9.3079" x2="-39.4271" y2="9.3257" layer="94"/>
<rectangle x1="-38.0402" y1="9.3079" x2="-36.7779" y2="9.3257" layer="94"/>
<rectangle x1="-36.5112" y1="9.3079" x2="-36.28" y2="9.3257" layer="94"/>
<rectangle x1="-39.6582" y1="9.3257" x2="-39.4271" y2="9.3434" layer="94"/>
<rectangle x1="-38.058" y1="9.3257" x2="-36.7779" y2="9.3434" layer="94"/>
<rectangle x1="-36.5112" y1="9.3257" x2="-36.28" y2="9.3434" layer="94"/>
<rectangle x1="-39.6582" y1="9.3434" x2="-39.4271" y2="9.3612" layer="94"/>
<rectangle x1="-38.0758" y1="9.3434" x2="-36.7779" y2="9.3612" layer="94"/>
<rectangle x1="-36.5112" y1="9.3434" x2="-36.28" y2="9.3612" layer="94"/>
<rectangle x1="-39.6582" y1="9.3612" x2="-39.4271" y2="9.379" layer="94"/>
<rectangle x1="-38.0936" y1="9.3612" x2="-36.7779" y2="9.379" layer="94"/>
<rectangle x1="-36.5112" y1="9.3612" x2="-36.28" y2="9.379" layer="94"/>
<rectangle x1="-39.6582" y1="9.379" x2="-39.4271" y2="9.3968" layer="94"/>
<rectangle x1="-38.1114" y1="9.379" x2="-36.7779" y2="9.3968" layer="94"/>
<rectangle x1="-36.5112" y1="9.379" x2="-36.28" y2="9.3968" layer="94"/>
<rectangle x1="-39.6582" y1="9.3968" x2="-39.4271" y2="9.4146" layer="94"/>
<rectangle x1="-38.1291" y1="9.3968" x2="-36.7779" y2="9.4146" layer="94"/>
<rectangle x1="-36.5112" y1="9.3968" x2="-36.28" y2="9.4146" layer="94"/>
<rectangle x1="-39.6582" y1="9.4146" x2="-39.4271" y2="9.4323" layer="94"/>
<rectangle x1="-38.1469" y1="9.4146" x2="-36.7779" y2="9.4323" layer="94"/>
<rectangle x1="-36.5112" y1="9.4146" x2="-36.28" y2="9.4323" layer="94"/>
<rectangle x1="-39.6582" y1="9.4323" x2="-39.4271" y2="9.4501" layer="94"/>
<rectangle x1="-38.1647" y1="9.4323" x2="-36.7779" y2="9.4501" layer="94"/>
<rectangle x1="-36.5112" y1="9.4323" x2="-36.28" y2="9.4501" layer="94"/>
<rectangle x1="-39.6582" y1="9.4501" x2="-39.4271" y2="9.4679" layer="94"/>
<rectangle x1="-38.1825" y1="9.4501" x2="-36.7779" y2="9.4679" layer="94"/>
<rectangle x1="-36.5112" y1="9.4501" x2="-36.28" y2="9.4679" layer="94"/>
<rectangle x1="-39.6582" y1="9.4679" x2="-39.4271" y2="9.4857" layer="94"/>
<rectangle x1="-38.2003" y1="9.4679" x2="-36.7779" y2="9.4857" layer="94"/>
<rectangle x1="-36.5112" y1="9.4679" x2="-36.28" y2="9.4857" layer="94"/>
<rectangle x1="-39.6582" y1="9.4857" x2="-39.4271" y2="9.5035" layer="94"/>
<rectangle x1="-38.218" y1="9.4857" x2="-36.7779" y2="9.5035" layer="94"/>
<rectangle x1="-36.5112" y1="9.4857" x2="-36.28" y2="9.5035" layer="94"/>
<rectangle x1="-39.6582" y1="9.5035" x2="-39.4271" y2="9.5212" layer="94"/>
<rectangle x1="-38.2536" y1="9.5035" x2="-36.7779" y2="9.5212" layer="94"/>
<rectangle x1="-36.5112" y1="9.5035" x2="-36.28" y2="9.5212" layer="94"/>
<rectangle x1="-39.6582" y1="9.5212" x2="-39.4271" y2="9.539" layer="94"/>
<rectangle x1="-38.2714" y1="9.5212" x2="-36.7779" y2="9.539" layer="94"/>
<rectangle x1="-36.5112" y1="9.5212" x2="-36.28" y2="9.539" layer="94"/>
<rectangle x1="-39.6582" y1="9.539" x2="-39.4271" y2="9.5568" layer="94"/>
<rectangle x1="-38.2892" y1="9.539" x2="-36.7779" y2="9.5568" layer="94"/>
<rectangle x1="-36.5112" y1="9.539" x2="-36.28" y2="9.5568" layer="94"/>
<rectangle x1="-39.6582" y1="9.5568" x2="-39.4271" y2="9.5746" layer="94"/>
<rectangle x1="-38.3069" y1="9.5568" x2="-36.7779" y2="9.5746" layer="94"/>
<rectangle x1="-36.5112" y1="9.5568" x2="-36.28" y2="9.5746" layer="94"/>
<rectangle x1="-39.6582" y1="9.5746" x2="-39.4271" y2="9.5924" layer="94"/>
<rectangle x1="-38.3247" y1="9.5746" x2="-36.7779" y2="9.5924" layer="94"/>
<rectangle x1="-36.5112" y1="9.5746" x2="-36.28" y2="9.5924" layer="94"/>
<rectangle x1="-39.6582" y1="9.5924" x2="-39.4271" y2="9.6101" layer="94"/>
<rectangle x1="-38.3425" y1="9.5924" x2="-36.7779" y2="9.6101" layer="94"/>
<rectangle x1="-36.5112" y1="9.5924" x2="-36.28" y2="9.6101" layer="94"/>
<rectangle x1="-39.6582" y1="9.6101" x2="-39.4271" y2="9.6279" layer="94"/>
<rectangle x1="-38.3603" y1="9.6101" x2="-36.7779" y2="9.6279" layer="94"/>
<rectangle x1="-36.5112" y1="9.6101" x2="-36.28" y2="9.6279" layer="94"/>
<rectangle x1="-39.6582" y1="9.6279" x2="-39.4271" y2="9.6457" layer="94"/>
<rectangle x1="-38.3958" y1="9.6279" x2="-36.7779" y2="9.6457" layer="94"/>
<rectangle x1="-36.5112" y1="9.6279" x2="-36.28" y2="9.6457" layer="94"/>
<rectangle x1="-39.6582" y1="9.6457" x2="-39.4271" y2="9.6635" layer="94"/>
<rectangle x1="-38.4136" y1="9.6457" x2="-36.7779" y2="9.6635" layer="94"/>
<rectangle x1="-36.5112" y1="9.6457" x2="-36.28" y2="9.6635" layer="94"/>
<rectangle x1="-39.6582" y1="9.6635" x2="-39.4271" y2="9.6813" layer="94"/>
<rectangle x1="-38.4314" y1="9.6635" x2="-36.7779" y2="9.6813" layer="94"/>
<rectangle x1="-36.5112" y1="9.6635" x2="-36.28" y2="9.6813" layer="94"/>
<rectangle x1="-39.6582" y1="9.6813" x2="-39.4271" y2="9.699" layer="94"/>
<rectangle x1="-38.4492" y1="9.6813" x2="-36.7779" y2="9.699" layer="94"/>
<rectangle x1="-36.5112" y1="9.6813" x2="-36.28" y2="9.699" layer="94"/>
<rectangle x1="-39.6582" y1="9.699" x2="-39.4271" y2="9.7168" layer="94"/>
<rectangle x1="-38.4847" y1="9.699" x2="-36.7779" y2="9.7168" layer="94"/>
<rectangle x1="-36.5112" y1="9.699" x2="-36.28" y2="9.7168" layer="94"/>
<rectangle x1="-39.6582" y1="9.7168" x2="-39.4271" y2="9.7346" layer="94"/>
<rectangle x1="-38.5025" y1="9.7168" x2="-38.1291" y2="9.7346" layer="94"/>
<rectangle x1="-37.6846" y1="9.7168" x2="-36.7779" y2="9.7346" layer="94"/>
<rectangle x1="-36.5112" y1="9.7168" x2="-36.28" y2="9.7346" layer="94"/>
<rectangle x1="-39.6582" y1="9.7346" x2="-39.4271" y2="9.7524" layer="94"/>
<rectangle x1="-38.5203" y1="9.7346" x2="-38.3425" y2="9.7524" layer="94"/>
<rectangle x1="-37.4535" y1="9.7346" x2="-36.7779" y2="9.7524" layer="94"/>
<rectangle x1="-36.5112" y1="9.7346" x2="-36.28" y2="9.7524" layer="94"/>
<rectangle x1="-39.6582" y1="9.7524" x2="-39.4271" y2="9.7702" layer="94"/>
<rectangle x1="-38.5559" y1="9.7524" x2="-38.4847" y2="9.7702" layer="94"/>
<rectangle x1="-37.3113" y1="9.7524" x2="-36.7779" y2="9.7702" layer="94"/>
<rectangle x1="-36.5112" y1="9.7524" x2="-36.28" y2="9.7702" layer="94"/>
<rectangle x1="-39.6582" y1="9.7702" x2="-39.4271" y2="9.7879" layer="94"/>
<rectangle x1="-37.2046" y1="9.7702" x2="-36.7779" y2="9.7879" layer="94"/>
<rectangle x1="-36.5112" y1="9.7702" x2="-36.28" y2="9.7879" layer="94"/>
<rectangle x1="-39.6582" y1="9.7879" x2="-39.4271" y2="9.8057" layer="94"/>
<rectangle x1="-37.0979" y1="9.7879" x2="-36.7779" y2="9.8057" layer="94"/>
<rectangle x1="-36.5112" y1="9.7879" x2="-36.28" y2="9.8057" layer="94"/>
<rectangle x1="-39.6582" y1="9.8057" x2="-39.4271" y2="9.8235" layer="94"/>
<rectangle x1="-37.0268" y1="9.8057" x2="-36.7779" y2="9.8235" layer="94"/>
<rectangle x1="-36.5112" y1="9.8057" x2="-36.28" y2="9.8235" layer="94"/>
<rectangle x1="-39.6582" y1="9.8235" x2="-39.4271" y2="9.8413" layer="94"/>
<rectangle x1="-36.9379" y1="9.8235" x2="-36.7779" y2="9.8413" layer="94"/>
<rectangle x1="-36.5112" y1="9.8235" x2="-36.28" y2="9.8413" layer="94"/>
<rectangle x1="-39.6582" y1="9.8413" x2="-39.4271" y2="9.8591" layer="94"/>
<rectangle x1="-36.8845" y1="9.8413" x2="-36.7779" y2="9.8591" layer="94"/>
<rectangle x1="-36.5112" y1="9.8413" x2="-36.28" y2="9.8591" layer="94"/>
<rectangle x1="-39.6582" y1="9.8591" x2="-39.4271" y2="9.8768" layer="94"/>
<rectangle x1="-36.8134" y1="9.8591" x2="-36.7779" y2="9.8768" layer="94"/>
<rectangle x1="-36.5112" y1="9.8591" x2="-36.28" y2="9.8768" layer="94"/>
<rectangle x1="-39.6582" y1="9.8768" x2="-39.4271" y2="9.8946" layer="94"/>
<rectangle x1="-36.5112" y1="9.8768" x2="-36.28" y2="9.8946" layer="94"/>
<rectangle x1="-39.6582" y1="9.8946" x2="-39.4271" y2="9.9124" layer="94"/>
<rectangle x1="-36.5112" y1="9.8946" x2="-36.28" y2="9.9124" layer="94"/>
<rectangle x1="-39.6582" y1="9.9124" x2="-39.4271" y2="9.9302" layer="94"/>
<rectangle x1="-36.5112" y1="9.9124" x2="-36.28" y2="9.9302" layer="94"/>
<rectangle x1="-39.6582" y1="9.9302" x2="-39.4271" y2="9.948" layer="94"/>
<rectangle x1="-36.5112" y1="9.9302" x2="-36.28" y2="9.948" layer="94"/>
<rectangle x1="-39.6582" y1="9.948" x2="-39.4271" y2="9.9657" layer="94"/>
<rectangle x1="-36.5112" y1="9.948" x2="-36.28" y2="9.9657" layer="94"/>
<rectangle x1="-39.6582" y1="9.9657" x2="-39.4271" y2="9.9835" layer="94"/>
<rectangle x1="-36.5112" y1="9.9657" x2="-36.28" y2="9.9835" layer="94"/>
<rectangle x1="-39.6582" y1="9.9835" x2="-39.4271" y2="10.0013" layer="94"/>
<rectangle x1="-36.5112" y1="9.9835" x2="-36.28" y2="10.0013" layer="94"/>
<rectangle x1="-39.6582" y1="10.0013" x2="-39.4271" y2="10.0191" layer="94"/>
<rectangle x1="-36.5112" y1="10.0013" x2="-36.28" y2="10.0191" layer="94"/>
<rectangle x1="-39.6582" y1="10.0191" x2="-39.4271" y2="10.0369" layer="94"/>
<rectangle x1="-36.5112" y1="10.0191" x2="-36.28" y2="10.0369" layer="94"/>
<rectangle x1="-39.6582" y1="10.0369" x2="-39.4271" y2="10.0546" layer="94"/>
<rectangle x1="-38.3958" y1="10.0369" x2="-37.5424" y2="10.0546" layer="94"/>
<rectangle x1="-36.5112" y1="10.0369" x2="-36.28" y2="10.0546" layer="94"/>
<rectangle x1="-39.6582" y1="10.0546" x2="-39.4271" y2="10.0724" layer="94"/>
<rectangle x1="-38.6092" y1="10.0546" x2="-37.329" y2="10.0724" layer="94"/>
<rectangle x1="-36.5112" y1="10.0546" x2="-36.28" y2="10.0724" layer="94"/>
<rectangle x1="-39.6582" y1="10.0724" x2="-39.4271" y2="10.0902" layer="94"/>
<rectangle x1="-38.7692" y1="10.0724" x2="-37.169" y2="10.0902" layer="94"/>
<rectangle x1="-36.5112" y1="10.0724" x2="-36.28" y2="10.0902" layer="94"/>
<rectangle x1="-39.6582" y1="10.0902" x2="-39.4271" y2="10.108" layer="94"/>
<rectangle x1="-38.8937" y1="10.0902" x2="-37.0446" y2="10.108" layer="94"/>
<rectangle x1="-36.5112" y1="10.0902" x2="-36.28" y2="10.108" layer="94"/>
<rectangle x1="-39.6582" y1="10.108" x2="-39.4271" y2="10.1258" layer="94"/>
<rectangle x1="-39.0004" y1="10.108" x2="-36.9379" y2="10.1258" layer="94"/>
<rectangle x1="-36.5112" y1="10.108" x2="-36.28" y2="10.1258" layer="94"/>
<rectangle x1="-39.6582" y1="10.1258" x2="-39.4271" y2="10.1435" layer="94"/>
<rectangle x1="-39.0893" y1="10.1258" x2="-36.849" y2="10.1435" layer="94"/>
<rectangle x1="-36.5112" y1="10.1258" x2="-36.28" y2="10.1435" layer="94"/>
<rectangle x1="-39.6582" y1="10.1435" x2="-39.4271" y2="10.1613" layer="94"/>
<rectangle x1="-39.1604" y1="10.1435" x2="-36.7601" y2="10.1613" layer="94"/>
<rectangle x1="-36.5112" y1="10.1435" x2="-36.28" y2="10.1613" layer="94"/>
<rectangle x1="-39.6582" y1="10.1613" x2="-39.4271" y2="10.1791" layer="94"/>
<rectangle x1="-39.2493" y1="10.1613" x2="-36.689" y2="10.1791" layer="94"/>
<rectangle x1="-36.5112" y1="10.1613" x2="-36.28" y2="10.1791" layer="94"/>
<rectangle x1="-39.6582" y1="10.1791" x2="-39.4271" y2="10.1969" layer="94"/>
<rectangle x1="-39.3204" y1="10.1791" x2="-36.6178" y2="10.1969" layer="94"/>
<rectangle x1="-36.5112" y1="10.1791" x2="-36.28" y2="10.1969" layer="94"/>
<rectangle x1="-39.6582" y1="10.1969" x2="-39.4271" y2="10.2147" layer="94"/>
<rectangle x1="-39.3737" y1="10.1969" x2="-36.5645" y2="10.2147" layer="94"/>
<rectangle x1="-36.5112" y1="10.1969" x2="-36.28" y2="10.2147" layer="94"/>
<rectangle x1="-39.6582" y1="10.2147" x2="-36.28" y2="10.2324" layer="94"/>
<rectangle x1="-39.6582" y1="10.2324" x2="-36.28" y2="10.2502" layer="94"/>
<rectangle x1="-39.6582" y1="10.2502" x2="-36.28" y2="10.268" layer="94"/>
<rectangle x1="-39.6582" y1="10.268" x2="-38.0758" y2="10.2858" layer="94"/>
<rectangle x1="-37.8624" y1="10.268" x2="-36.28" y2="10.2858" layer="94"/>
<rectangle x1="-39.6582" y1="10.2858" x2="-38.4492" y2="10.3036" layer="94"/>
<rectangle x1="-37.4891" y1="10.2858" x2="-36.28" y2="10.3036" layer="94"/>
<rectangle x1="-39.6582" y1="10.3036" x2="-38.627" y2="10.3213" layer="94"/>
<rectangle x1="-37.3113" y1="10.3036" x2="-36.28" y2="10.3213" layer="94"/>
<rectangle x1="-39.6582" y1="10.3213" x2="-38.7692" y2="10.3391" layer="94"/>
<rectangle x1="-37.169" y1="10.3213" x2="-36.28" y2="10.3391" layer="94"/>
<rectangle x1="-39.6582" y1="10.3391" x2="-38.8937" y2="10.3569" layer="94"/>
<rectangle x1="-37.0446" y1="10.3391" x2="-36.28" y2="10.3569" layer="94"/>
<rectangle x1="-39.6582" y1="10.3569" x2="-39.0004" y2="10.3747" layer="94"/>
<rectangle x1="-36.9379" y1="10.3569" x2="-36.28" y2="10.3747" layer="94"/>
<rectangle x1="-39.6582" y1="10.3747" x2="-39.0893" y2="10.3925" layer="94"/>
<rectangle x1="-36.849" y1="10.3747" x2="-36.28" y2="10.3925" layer="94"/>
<rectangle x1="-39.6582" y1="10.3925" x2="-39.1604" y2="10.4102" layer="94"/>
<rectangle x1="-36.7779" y1="10.3925" x2="-36.28" y2="10.4102" layer="94"/>
<rectangle x1="-39.6582" y1="10.4102" x2="-39.2315" y2="10.428" layer="94"/>
<rectangle x1="-36.7067" y1="10.4102" x2="-36.28" y2="10.428" layer="94"/>
<rectangle x1="-39.6582" y1="10.428" x2="-39.3026" y2="10.4458" layer="94"/>
<rectangle x1="-36.6356" y1="10.428" x2="-36.28" y2="10.4458" layer="94"/>
<rectangle x1="-39.6582" y1="10.4458" x2="-39.3737" y2="10.4636" layer="94"/>
<rectangle x1="-36.5645" y1="10.4458" x2="-36.28" y2="10.4636" layer="94"/>
<rectangle x1="-39.6582" y1="10.4636" x2="-39.4271" y2="10.4814" layer="94"/>
<rectangle x1="-36.5112" y1="10.4636" x2="-36.28" y2="10.4814" layer="94"/>
<rectangle x1="-39.6582" y1="10.4814" x2="-39.4804" y2="10.4991" layer="94"/>
<rectangle x1="-36.4578" y1="10.4814" x2="-36.28" y2="10.4991" layer="94"/>
<rectangle x1="-39.6582" y1="10.4991" x2="-39.5338" y2="10.5169" layer="94"/>
<rectangle x1="-36.4045" y1="10.4991" x2="-36.28" y2="10.5169" layer="94"/>
<rectangle x1="-39.6582" y1="10.5169" x2="-39.5871" y2="10.5347" layer="94"/>
<rectangle x1="-36.3511" y1="10.5169" x2="-36.28" y2="10.5347" layer="94"/>
<rectangle x1="-39.6582" y1="10.5347" x2="-39.6404" y2="10.5525" layer="94"/>
<rectangle x1="-36.2978" y1="10.5347" x2="-36.28" y2="10.5525" layer="94"/>
<rectangle x1="-39.516" y1="10.8903" x2="-38.2003" y2="10.9081" layer="94"/>
<rectangle x1="-37.738" y1="10.8903" x2="-36.4223" y2="10.9081" layer="94"/>
<rectangle x1="-39.5338" y1="10.9081" x2="-38.1647" y2="10.9259" layer="94"/>
<rectangle x1="-37.7735" y1="10.9081" x2="-36.4045" y2="10.9259" layer="94"/>
<rectangle x1="-39.5515" y1="10.9259" x2="-38.1469" y2="10.9436" layer="94"/>
<rectangle x1="-37.7913" y1="10.9259" x2="-36.3867" y2="10.9436" layer="94"/>
<rectangle x1="-39.5693" y1="10.9436" x2="-38.1291" y2="10.9614" layer="94"/>
<rectangle x1="-37.8091" y1="10.9436" x2="-36.3689" y2="10.9614" layer="94"/>
<rectangle x1="-39.5871" y1="10.9614" x2="-38.1291" y2="10.9792" layer="94"/>
<rectangle x1="-37.8091" y1="10.9614" x2="-36.3511" y2="10.9792" layer="94"/>
<rectangle x1="-39.5871" y1="10.9792" x2="-38.1114" y2="10.997" layer="94"/>
<rectangle x1="-37.8269" y1="10.9792" x2="-36.3511" y2="10.997" layer="94"/>
<rectangle x1="-39.5871" y1="10.997" x2="-38.1114" y2="11.0148" layer="94"/>
<rectangle x1="-37.8269" y1="10.997" x2="-36.3511" y2="11.0148" layer="94"/>
<rectangle x1="-39.5871" y1="11.0148" x2="-38.1114" y2="11.0325" layer="94"/>
<rectangle x1="-37.8269" y1="11.0148" x2="-36.3511" y2="11.0325" layer="94"/>
<rectangle x1="-39.5871" y1="11.0325" x2="-39.4271" y2="11.0503" layer="94"/>
<rectangle x1="-38.2714" y1="11.0325" x2="-38.1114" y2="11.0503" layer="94"/>
<rectangle x1="-37.8269" y1="11.0325" x2="-37.6669" y2="11.0503" layer="94"/>
<rectangle x1="-36.5112" y1="11.0325" x2="-36.3511" y2="11.0503" layer="94"/>
<rectangle x1="-39.5871" y1="11.0503" x2="-39.4449" y2="11.0681" layer="94"/>
<rectangle x1="-38.2714" y1="11.0503" x2="-38.1114" y2="11.0681" layer="94"/>
<rectangle x1="-37.8269" y1="11.0503" x2="-37.6669" y2="11.0681" layer="94"/>
<rectangle x1="-36.4934" y1="11.0503" x2="-36.3511" y2="11.0681" layer="94"/>
<rectangle x1="-39.5871" y1="11.0681" x2="-39.4449" y2="11.0859" layer="94"/>
<rectangle x1="-38.2714" y1="11.0681" x2="-38.1114" y2="11.0859" layer="94"/>
<rectangle x1="-37.8269" y1="11.0681" x2="-37.6669" y2="11.0859" layer="94"/>
<rectangle x1="-36.4934" y1="11.0681" x2="-36.3511" y2="11.0859" layer="94"/>
<rectangle x1="-39.5871" y1="11.0859" x2="-39.4449" y2="11.1037" layer="94"/>
<rectangle x1="-38.2714" y1="11.0859" x2="-38.1114" y2="11.1037" layer="94"/>
<rectangle x1="-37.8269" y1="11.0859" x2="-37.6669" y2="11.1037" layer="94"/>
<rectangle x1="-36.4934" y1="11.0859" x2="-36.3511" y2="11.1037" layer="94"/>
<rectangle x1="-39.5871" y1="11.1037" x2="-39.4449" y2="11.1214" layer="94"/>
<rectangle x1="-38.2714" y1="11.1037" x2="-38.1114" y2="11.1214" layer="94"/>
<rectangle x1="-37.8269" y1="11.1037" x2="-37.6669" y2="11.1214" layer="94"/>
<rectangle x1="-36.4934" y1="11.1037" x2="-36.3511" y2="11.1214" layer="94"/>
<rectangle x1="-39.5871" y1="11.1214" x2="-39.4449" y2="11.1392" layer="94"/>
<rectangle x1="-38.2714" y1="11.1214" x2="-38.1114" y2="11.1392" layer="94"/>
<rectangle x1="-37.8269" y1="11.1214" x2="-37.6669" y2="11.1392" layer="94"/>
<rectangle x1="-36.4934" y1="11.1214" x2="-36.3511" y2="11.1392" layer="94"/>
<rectangle x1="-39.5871" y1="11.1392" x2="-39.4449" y2="11.157" layer="94"/>
<rectangle x1="-38.2714" y1="11.1392" x2="-38.1114" y2="11.157" layer="94"/>
<rectangle x1="-37.8269" y1="11.1392" x2="-37.6669" y2="11.157" layer="94"/>
<rectangle x1="-36.4934" y1="11.1392" x2="-36.3511" y2="11.157" layer="94"/>
<rectangle x1="-39.5871" y1="11.157" x2="-39.4449" y2="11.1748" layer="94"/>
<rectangle x1="-38.2714" y1="11.157" x2="-38.1114" y2="11.1748" layer="94"/>
<rectangle x1="-37.8269" y1="11.157" x2="-37.6669" y2="11.1748" layer="94"/>
<rectangle x1="-36.4934" y1="11.157" x2="-36.3511" y2="11.1748" layer="94"/>
<rectangle x1="-39.5871" y1="11.1748" x2="-39.4449" y2="11.1926" layer="94"/>
<rectangle x1="-38.2714" y1="11.1748" x2="-38.1114" y2="11.1926" layer="94"/>
<rectangle x1="-37.8269" y1="11.1748" x2="-37.6669" y2="11.1926" layer="94"/>
<rectangle x1="-36.4934" y1="11.1748" x2="-36.3511" y2="11.1926" layer="94"/>
<rectangle x1="-39.5871" y1="11.1926" x2="-39.4449" y2="11.2103" layer="94"/>
<rectangle x1="-38.2714" y1="11.1926" x2="-38.1114" y2="11.2103" layer="94"/>
<rectangle x1="-37.8269" y1="11.1926" x2="-37.6669" y2="11.2103" layer="94"/>
<rectangle x1="-36.4934" y1="11.1926" x2="-36.3511" y2="11.2103" layer="94"/>
<rectangle x1="-39.5871" y1="11.2103" x2="-39.4449" y2="11.2281" layer="94"/>
<rectangle x1="-38.2714" y1="11.2103" x2="-38.1114" y2="11.2281" layer="94"/>
<rectangle x1="-37.8269" y1="11.2103" x2="-37.6669" y2="11.2281" layer="94"/>
<rectangle x1="-36.4934" y1="11.2103" x2="-36.3511" y2="11.2281" layer="94"/>
<rectangle x1="-39.5871" y1="11.2281" x2="-39.4449" y2="11.2459" layer="94"/>
<rectangle x1="-38.2714" y1="11.2281" x2="-38.1114" y2="11.2459" layer="94"/>
<rectangle x1="-37.8269" y1="11.2281" x2="-37.6669" y2="11.2459" layer="94"/>
<rectangle x1="-36.4934" y1="11.2281" x2="-36.3511" y2="11.2459" layer="94"/>
<rectangle x1="-39.5871" y1="11.2459" x2="-39.4449" y2="11.2637" layer="94"/>
<rectangle x1="-38.2714" y1="11.2459" x2="-38.1114" y2="11.2637" layer="94"/>
<rectangle x1="-37.8269" y1="11.2459" x2="-37.6669" y2="11.2637" layer="94"/>
<rectangle x1="-36.4934" y1="11.2459" x2="-36.3511" y2="11.2637" layer="94"/>
<rectangle x1="-39.5871" y1="11.2637" x2="-39.4449" y2="11.2815" layer="94"/>
<rectangle x1="-38.2714" y1="11.2637" x2="-38.1114" y2="11.2815" layer="94"/>
<rectangle x1="-37.8269" y1="11.2637" x2="-37.6669" y2="11.2815" layer="94"/>
<rectangle x1="-36.4934" y1="11.2637" x2="-36.3511" y2="11.2815" layer="94"/>
<rectangle x1="-39.5871" y1="11.2815" x2="-39.4449" y2="11.2992" layer="94"/>
<rectangle x1="-38.2714" y1="11.2815" x2="-38.1114" y2="11.2992" layer="94"/>
<rectangle x1="-37.8269" y1="11.2815" x2="-37.6669" y2="11.2992" layer="94"/>
<rectangle x1="-36.4934" y1="11.2815" x2="-36.3511" y2="11.2992" layer="94"/>
<rectangle x1="-39.5871" y1="11.2992" x2="-39.4449" y2="11.317" layer="94"/>
<rectangle x1="-38.2714" y1="11.2992" x2="-38.1114" y2="11.317" layer="94"/>
<rectangle x1="-37.8269" y1="11.2992" x2="-37.6669" y2="11.317" layer="94"/>
<rectangle x1="-36.4934" y1="11.2992" x2="-36.3511" y2="11.317" layer="94"/>
<rectangle x1="-39.5871" y1="11.317" x2="-39.4449" y2="11.3348" layer="94"/>
<rectangle x1="-38.2714" y1="11.317" x2="-38.1114" y2="11.3348" layer="94"/>
<rectangle x1="-37.8269" y1="11.317" x2="-37.6669" y2="11.3348" layer="94"/>
<rectangle x1="-36.4934" y1="11.317" x2="-36.3511" y2="11.3348" layer="94"/>
<rectangle x1="-39.5871" y1="11.3348" x2="-39.4449" y2="11.3526" layer="94"/>
<rectangle x1="-38.2714" y1="11.3348" x2="-38.1114" y2="11.3526" layer="94"/>
<rectangle x1="-37.8269" y1="11.3348" x2="-37.6669" y2="11.3526" layer="94"/>
<rectangle x1="-36.4934" y1="11.3348" x2="-36.3511" y2="11.3526" layer="94"/>
<rectangle x1="-39.5871" y1="11.3526" x2="-39.4449" y2="11.3704" layer="94"/>
<rectangle x1="-38.2714" y1="11.3526" x2="-38.1114" y2="11.3704" layer="94"/>
<rectangle x1="-37.8269" y1="11.3526" x2="-37.6669" y2="11.3704" layer="94"/>
<rectangle x1="-36.4934" y1="11.3526" x2="-36.3511" y2="11.3704" layer="94"/>
<rectangle x1="-39.5871" y1="11.3704" x2="-39.4449" y2="11.3881" layer="94"/>
<rectangle x1="-38.2714" y1="11.3704" x2="-38.1114" y2="11.3881" layer="94"/>
<rectangle x1="-37.8269" y1="11.3704" x2="-37.6669" y2="11.3881" layer="94"/>
<rectangle x1="-36.4934" y1="11.3704" x2="-36.3511" y2="11.3881" layer="94"/>
<rectangle x1="-39.5871" y1="11.3881" x2="-39.4449" y2="11.4059" layer="94"/>
<rectangle x1="-38.2714" y1="11.3881" x2="-38.1114" y2="11.4059" layer="94"/>
<rectangle x1="-37.8269" y1="11.3881" x2="-37.6669" y2="11.4059" layer="94"/>
<rectangle x1="-36.4934" y1="11.3881" x2="-36.3511" y2="11.4059" layer="94"/>
<rectangle x1="-39.5871" y1="11.4059" x2="-39.4449" y2="11.4237" layer="94"/>
<rectangle x1="-38.2714" y1="11.4059" x2="-38.1114" y2="11.4237" layer="94"/>
<rectangle x1="-37.8269" y1="11.4059" x2="-37.6669" y2="11.4237" layer="94"/>
<rectangle x1="-36.4934" y1="11.4059" x2="-36.3511" y2="11.4237" layer="94"/>
<rectangle x1="-39.5871" y1="11.4237" x2="-39.4449" y2="11.4415" layer="94"/>
<rectangle x1="-38.2714" y1="11.4237" x2="-37.6669" y2="11.4415" layer="94"/>
<rectangle x1="-36.4934" y1="11.4237" x2="-36.3511" y2="11.4415" layer="94"/>
<rectangle x1="-39.5871" y1="11.4415" x2="-39.4449" y2="11.4593" layer="94"/>
<rectangle x1="-38.2714" y1="11.4415" x2="-37.6669" y2="11.4593" layer="94"/>
<rectangle x1="-36.4934" y1="11.4415" x2="-36.3511" y2="11.4593" layer="94"/>
<rectangle x1="-39.5871" y1="11.4593" x2="-39.4449" y2="11.477" layer="94"/>
<rectangle x1="-38.2714" y1="11.4593" x2="-37.6669" y2="11.477" layer="94"/>
<rectangle x1="-36.4934" y1="11.4593" x2="-36.3511" y2="11.477" layer="94"/>
<rectangle x1="-39.5871" y1="11.477" x2="-39.4449" y2="11.4948" layer="94"/>
<rectangle x1="-38.2714" y1="11.477" x2="-37.6669" y2="11.4948" layer="94"/>
<rectangle x1="-36.4934" y1="11.477" x2="-36.3511" y2="11.4948" layer="94"/>
<rectangle x1="-39.5871" y1="11.4948" x2="-39.4449" y2="11.5126" layer="94"/>
<rectangle x1="-38.2714" y1="11.4948" x2="-37.6669" y2="11.5126" layer="94"/>
<rectangle x1="-36.4934" y1="11.4948" x2="-36.3511" y2="11.5126" layer="94"/>
<rectangle x1="-39.5871" y1="11.5126" x2="-39.4449" y2="11.5304" layer="94"/>
<rectangle x1="-38.5559" y1="11.5126" x2="-38.467" y2="11.5304" layer="94"/>
<rectangle x1="-38.2714" y1="11.5126" x2="-37.6669" y2="11.5304" layer="94"/>
<rectangle x1="-36.7779" y1="11.5126" x2="-36.689" y2="11.5304" layer="94"/>
<rectangle x1="-36.4934" y1="11.5126" x2="-36.3511" y2="11.5304" layer="94"/>
<rectangle x1="-39.5871" y1="11.5304" x2="-39.4449" y2="11.5482" layer="94"/>
<rectangle x1="-38.5914" y1="11.5304" x2="-38.4136" y2="11.5482" layer="94"/>
<rectangle x1="-38.2714" y1="11.5304" x2="-37.6669" y2="11.5482" layer="94"/>
<rectangle x1="-36.8134" y1="11.5304" x2="-36.6356" y2="11.5482" layer="94"/>
<rectangle x1="-36.4934" y1="11.5304" x2="-36.3511" y2="11.5482" layer="94"/>
<rectangle x1="-39.5871" y1="11.5482" x2="-39.4449" y2="11.5659" layer="94"/>
<rectangle x1="-38.627" y1="11.5482" x2="-38.3958" y2="11.5659" layer="94"/>
<rectangle x1="-38.2714" y1="11.5482" x2="-37.6669" y2="11.5659" layer="94"/>
<rectangle x1="-36.849" y1="11.5482" x2="-36.6178" y2="11.5659" layer="94"/>
<rectangle x1="-36.4934" y1="11.5482" x2="-36.3511" y2="11.5659" layer="94"/>
<rectangle x1="-39.5871" y1="11.5659" x2="-39.4449" y2="11.5837" layer="94"/>
<rectangle x1="-38.6448" y1="11.5659" x2="-38.3781" y2="11.5837" layer="94"/>
<rectangle x1="-38.2714" y1="11.5659" x2="-37.6669" y2="11.5837" layer="94"/>
<rectangle x1="-36.8668" y1="11.5659" x2="-36.6001" y2="11.5837" layer="94"/>
<rectangle x1="-36.4934" y1="11.5659" x2="-36.3511" y2="11.5837" layer="94"/>
<rectangle x1="-39.5871" y1="11.5837" x2="-39.4449" y2="11.6015" layer="94"/>
<rectangle x1="-38.6448" y1="11.5837" x2="-38.3603" y2="11.6015" layer="94"/>
<rectangle x1="-38.2714" y1="11.5837" x2="-37.6669" y2="11.6015" layer="94"/>
<rectangle x1="-36.8668" y1="11.5837" x2="-36.5823" y2="11.6015" layer="94"/>
<rectangle x1="-36.4934" y1="11.5837" x2="-36.3511" y2="11.6015" layer="94"/>
<rectangle x1="-39.5871" y1="11.6015" x2="-39.4449" y2="11.6193" layer="94"/>
<rectangle x1="-38.6625" y1="11.6015" x2="-38.3603" y2="11.6193" layer="94"/>
<rectangle x1="-38.2714" y1="11.6015" x2="-37.6669" y2="11.6193" layer="94"/>
<rectangle x1="-36.8845" y1="11.6015" x2="-36.5823" y2="11.6193" layer="94"/>
<rectangle x1="-36.4934" y1="11.6015" x2="-36.3511" y2="11.6193" layer="94"/>
<rectangle x1="-39.5871" y1="11.6193" x2="-39.4449" y2="11.6371" layer="94"/>
<rectangle x1="-38.6625" y1="11.6193" x2="-38.3425" y2="11.6371" layer="94"/>
<rectangle x1="-38.2714" y1="11.6193" x2="-38.1114" y2="11.6371" layer="94"/>
<rectangle x1="-37.8269" y1="11.6193" x2="-37.6669" y2="11.6371" layer="94"/>
<rectangle x1="-36.9023" y1="11.6193" x2="-36.5645" y2="11.6371" layer="94"/>
<rectangle x1="-36.4934" y1="11.6193" x2="-36.3511" y2="11.6371" layer="94"/>
<rectangle x1="-39.5871" y1="11.6371" x2="-39.4449" y2="11.6548" layer="94"/>
<rectangle x1="-38.6803" y1="11.6371" x2="-38.3425" y2="11.6548" layer="94"/>
<rectangle x1="-38.2714" y1="11.6371" x2="-38.1114" y2="11.6548" layer="94"/>
<rectangle x1="-37.8269" y1="11.6371" x2="-37.6669" y2="11.6548" layer="94"/>
<rectangle x1="-36.9023" y1="11.6371" x2="-36.5645" y2="11.6548" layer="94"/>
<rectangle x1="-36.4934" y1="11.6371" x2="-36.3511" y2="11.6548" layer="94"/>
<rectangle x1="-39.5871" y1="11.6548" x2="-39.4449" y2="11.6726" layer="94"/>
<rectangle x1="-38.6803" y1="11.6548" x2="-38.3425" y2="11.6726" layer="94"/>
<rectangle x1="-38.2714" y1="11.6548" x2="-38.1114" y2="11.6726" layer="94"/>
<rectangle x1="-37.8269" y1="11.6548" x2="-37.6669" y2="11.6726" layer="94"/>
<rectangle x1="-36.9023" y1="11.6548" x2="-36.5645" y2="11.6726" layer="94"/>
<rectangle x1="-36.4934" y1="11.6548" x2="-36.3511" y2="11.6726" layer="94"/>
<rectangle x1="-39.5871" y1="11.6726" x2="-39.4449" y2="11.6904" layer="94"/>
<rectangle x1="-38.6803" y1="11.6726" x2="-38.3425" y2="11.6904" layer="94"/>
<rectangle x1="-38.2714" y1="11.6726" x2="-38.1114" y2="11.6904" layer="94"/>
<rectangle x1="-37.8269" y1="11.6726" x2="-37.6669" y2="11.6904" layer="94"/>
<rectangle x1="-36.9023" y1="11.6726" x2="-36.5645" y2="11.6904" layer="94"/>
<rectangle x1="-36.4934" y1="11.6726" x2="-36.3511" y2="11.6904" layer="94"/>
<rectangle x1="-39.5871" y1="11.6904" x2="-39.4449" y2="11.7082" layer="94"/>
<rectangle x1="-38.6803" y1="11.6904" x2="-38.3425" y2="11.7082" layer="94"/>
<rectangle x1="-38.2714" y1="11.6904" x2="-38.1114" y2="11.7082" layer="94"/>
<rectangle x1="-37.8269" y1="11.6904" x2="-37.6669" y2="11.7082" layer="94"/>
<rectangle x1="-36.9023" y1="11.6904" x2="-36.5645" y2="11.7082" layer="94"/>
<rectangle x1="-36.4934" y1="11.6904" x2="-36.3511" y2="11.7082" layer="94"/>
<rectangle x1="-39.5871" y1="11.7082" x2="-39.4449" y2="11.726" layer="94"/>
<rectangle x1="-38.6803" y1="11.7082" x2="-38.3425" y2="11.726" layer="94"/>
<rectangle x1="-38.2714" y1="11.7082" x2="-38.1114" y2="11.726" layer="94"/>
<rectangle x1="-37.8269" y1="11.7082" x2="-37.6669" y2="11.726" layer="94"/>
<rectangle x1="-36.9023" y1="11.7082" x2="-36.5645" y2="11.726" layer="94"/>
<rectangle x1="-36.4934" y1="11.7082" x2="-36.3511" y2="11.726" layer="94"/>
<rectangle x1="-39.5871" y1="11.726" x2="-39.4449" y2="11.7437" layer="94"/>
<rectangle x1="-38.6803" y1="11.726" x2="-38.3425" y2="11.7437" layer="94"/>
<rectangle x1="-38.2714" y1="11.726" x2="-38.1114" y2="11.7437" layer="94"/>
<rectangle x1="-37.8269" y1="11.726" x2="-37.6669" y2="11.7437" layer="94"/>
<rectangle x1="-36.9023" y1="11.726" x2="-36.5645" y2="11.7437" layer="94"/>
<rectangle x1="-36.4934" y1="11.726" x2="-36.3511" y2="11.7437" layer="94"/>
<rectangle x1="-39.5871" y1="11.7437" x2="-39.4449" y2="11.7615" layer="94"/>
<rectangle x1="-38.6625" y1="11.7437" x2="-38.3425" y2="11.7615" layer="94"/>
<rectangle x1="-38.2714" y1="11.7437" x2="-38.1114" y2="11.7615" layer="94"/>
<rectangle x1="-37.8269" y1="11.7437" x2="-37.6669" y2="11.7615" layer="94"/>
<rectangle x1="-36.9023" y1="11.7437" x2="-36.5645" y2="11.7615" layer="94"/>
<rectangle x1="-36.4934" y1="11.7437" x2="-36.3511" y2="11.7615" layer="94"/>
<rectangle x1="-39.5871" y1="11.7615" x2="-39.4449" y2="11.7793" layer="94"/>
<rectangle x1="-38.6625" y1="11.7615" x2="-38.3425" y2="11.7793" layer="94"/>
<rectangle x1="-38.2714" y1="11.7615" x2="-38.1114" y2="11.7793" layer="94"/>
<rectangle x1="-37.8269" y1="11.7615" x2="-37.6669" y2="11.7793" layer="94"/>
<rectangle x1="-36.8845" y1="11.7615" x2="-36.5823" y2="11.7793" layer="94"/>
<rectangle x1="-36.4934" y1="11.7615" x2="-36.3511" y2="11.7793" layer="94"/>
<rectangle x1="-39.5871" y1="11.7793" x2="-39.4449" y2="11.7971" layer="94"/>
<rectangle x1="-38.6448" y1="11.7793" x2="-38.3603" y2="11.7971" layer="94"/>
<rectangle x1="-38.2714" y1="11.7793" x2="-38.1114" y2="11.7971" layer="94"/>
<rectangle x1="-37.8269" y1="11.7793" x2="-37.6669" y2="11.7971" layer="94"/>
<rectangle x1="-36.8668" y1="11.7793" x2="-36.5823" y2="11.7971" layer="94"/>
<rectangle x1="-36.4934" y1="11.7793" x2="-36.3511" y2="11.7971" layer="94"/>
<rectangle x1="-39.5871" y1="11.7971" x2="-39.4449" y2="11.8149" layer="94"/>
<rectangle x1="-38.6448" y1="11.7971" x2="-38.3781" y2="11.8149" layer="94"/>
<rectangle x1="-38.2714" y1="11.7971" x2="-38.1114" y2="11.8149" layer="94"/>
<rectangle x1="-37.8269" y1="11.7971" x2="-37.6669" y2="11.8149" layer="94"/>
<rectangle x1="-36.8668" y1="11.7971" x2="-36.6001" y2="11.8149" layer="94"/>
<rectangle x1="-36.4934" y1="11.7971" x2="-36.3511" y2="11.8149" layer="94"/>
<rectangle x1="-39.5871" y1="11.8149" x2="-39.4449" y2="11.8326" layer="94"/>
<rectangle x1="-38.627" y1="11.8149" x2="-38.3958" y2="11.8326" layer="94"/>
<rectangle x1="-38.2714" y1="11.8149" x2="-38.1114" y2="11.8326" layer="94"/>
<rectangle x1="-37.8269" y1="11.8149" x2="-37.6669" y2="11.8326" layer="94"/>
<rectangle x1="-36.849" y1="11.8149" x2="-36.6178" y2="11.8326" layer="94"/>
<rectangle x1="-36.4934" y1="11.8149" x2="-36.3511" y2="11.8326" layer="94"/>
<rectangle x1="-39.5871" y1="11.8326" x2="-39.4449" y2="11.8504" layer="94"/>
<rectangle x1="-38.5914" y1="11.8326" x2="-38.4136" y2="11.8504" layer="94"/>
<rectangle x1="-38.2714" y1="11.8326" x2="-38.1114" y2="11.8504" layer="94"/>
<rectangle x1="-37.8269" y1="11.8326" x2="-37.6669" y2="11.8504" layer="94"/>
<rectangle x1="-36.8134" y1="11.8326" x2="-36.6356" y2="11.8504" layer="94"/>
<rectangle x1="-36.4934" y1="11.8326" x2="-36.3511" y2="11.8504" layer="94"/>
<rectangle x1="-39.5871" y1="11.8504" x2="-39.4449" y2="11.8682" layer="94"/>
<rectangle x1="-38.5559" y1="11.8504" x2="-38.467" y2="11.8682" layer="94"/>
<rectangle x1="-38.2714" y1="11.8504" x2="-38.1114" y2="11.8682" layer="94"/>
<rectangle x1="-37.8269" y1="11.8504" x2="-37.6669" y2="11.8682" layer="94"/>
<rectangle x1="-36.7779" y1="11.8504" x2="-36.689" y2="11.8682" layer="94"/>
<rectangle x1="-36.4934" y1="11.8504" x2="-36.3511" y2="11.8682" layer="94"/>
<rectangle x1="-39.5871" y1="11.8682" x2="-39.4449" y2="11.886" layer="94"/>
<rectangle x1="-38.2714" y1="11.8682" x2="-38.1114" y2="11.886" layer="94"/>
<rectangle x1="-37.8269" y1="11.8682" x2="-37.6669" y2="11.886" layer="94"/>
<rectangle x1="-36.4934" y1="11.8682" x2="-36.3511" y2="11.886" layer="94"/>
<rectangle x1="-39.5871" y1="11.886" x2="-39.4449" y2="11.9038" layer="94"/>
<rectangle x1="-38.2714" y1="11.886" x2="-38.1114" y2="11.9038" layer="94"/>
<rectangle x1="-37.8269" y1="11.886" x2="-37.6669" y2="11.9038" layer="94"/>
<rectangle x1="-36.4934" y1="11.886" x2="-36.3511" y2="11.9038" layer="94"/>
<rectangle x1="-39.5871" y1="11.9038" x2="-39.4449" y2="11.9215" layer="94"/>
<rectangle x1="-38.2714" y1="11.9038" x2="-38.1114" y2="11.9215" layer="94"/>
<rectangle x1="-37.8269" y1="11.9038" x2="-37.6669" y2="11.9215" layer="94"/>
<rectangle x1="-36.4934" y1="11.9038" x2="-36.3511" y2="11.9215" layer="94"/>
<rectangle x1="-39.5871" y1="11.9215" x2="-39.4449" y2="11.9393" layer="94"/>
<rectangle x1="-38.2714" y1="11.9215" x2="-38.1114" y2="11.9393" layer="94"/>
<rectangle x1="-37.8269" y1="11.9215" x2="-37.6669" y2="11.9393" layer="94"/>
<rectangle x1="-36.4934" y1="11.9215" x2="-36.3511" y2="11.9393" layer="94"/>
<rectangle x1="-39.5871" y1="11.9393" x2="-39.4449" y2="11.9571" layer="94"/>
<rectangle x1="-38.2714" y1="11.9393" x2="-38.1114" y2="11.9571" layer="94"/>
<rectangle x1="-37.8269" y1="11.9393" x2="-37.6669" y2="11.9571" layer="94"/>
<rectangle x1="-36.4934" y1="11.9393" x2="-36.3511" y2="11.9571" layer="94"/>
<rectangle x1="-39.5871" y1="11.9571" x2="-39.4449" y2="11.9749" layer="94"/>
<rectangle x1="-38.2714" y1="11.9571" x2="-38.1114" y2="11.9749" layer="94"/>
<rectangle x1="-37.8269" y1="11.9571" x2="-37.6669" y2="11.9749" layer="94"/>
<rectangle x1="-36.4934" y1="11.9571" x2="-36.3511" y2="11.9749" layer="94"/>
<rectangle x1="-39.5871" y1="11.9749" x2="-39.4449" y2="11.9927" layer="94"/>
<rectangle x1="-38.2714" y1="11.9749" x2="-38.1114" y2="11.9927" layer="94"/>
<rectangle x1="-37.8269" y1="11.9749" x2="-37.6669" y2="11.9927" layer="94"/>
<rectangle x1="-36.4934" y1="11.9749" x2="-36.3511" y2="11.9927" layer="94"/>
<rectangle x1="-39.5871" y1="11.9927" x2="-39.4449" y2="12.0104" layer="94"/>
<rectangle x1="-38.2714" y1="11.9927" x2="-38.1114" y2="12.0104" layer="94"/>
<rectangle x1="-37.8269" y1="11.9927" x2="-37.6669" y2="12.0104" layer="94"/>
<rectangle x1="-36.4934" y1="11.9927" x2="-36.3511" y2="12.0104" layer="94"/>
<rectangle x1="-39.5871" y1="12.0104" x2="-38.1114" y2="12.0282" layer="94"/>
<rectangle x1="-37.8269" y1="12.0104" x2="-36.3511" y2="12.0282" layer="94"/>
<rectangle x1="-39.5871" y1="12.0282" x2="-38.1114" y2="12.046" layer="94"/>
<rectangle x1="-37.8269" y1="12.0282" x2="-36.3511" y2="12.046" layer="94"/>
<rectangle x1="-39.5871" y1="12.046" x2="-38.1291" y2="12.0638" layer="94"/>
<rectangle x1="-37.8091" y1="12.046" x2="-36.3511" y2="12.0638" layer="94"/>
<rectangle x1="-39.5693" y1="12.0638" x2="-38.1291" y2="12.0816" layer="94"/>
<rectangle x1="-37.8091" y1="12.0638" x2="-36.3689" y2="12.0816" layer="94"/>
<rectangle x1="-39.5693" y1="12.0816" x2="-38.1469" y2="12.0993" layer="94"/>
<rectangle x1="-37.7913" y1="12.0816" x2="-36.3689" y2="12.0993" layer="94"/>
<rectangle x1="-39.5515" y1="12.0993" x2="-38.1469" y2="12.1171" layer="94"/>
<rectangle x1="-37.7913" y1="12.0993" x2="-36.3867" y2="12.1171" layer="94"/>
<rectangle x1="-39.5338" y1="12.1171" x2="-38.1825" y2="12.1349" layer="94"/>
<rectangle x1="-37.7558" y1="12.1171" x2="-36.4045" y2="12.1349" layer="94"/>
<rectangle x1="-39.4982" y1="12.1349" x2="-38.218" y2="12.1527" layer="94"/>
<rectangle x1="-37.7202" y1="12.1349" x2="-36.4578" y2="12.1527" layer="94"/>
<rectangle x1="-37.8091" y1="12.4194" x2="-37.2579" y2="12.4372" layer="94"/>
<rectangle x1="-38.0402" y1="12.4372" x2="-37.0623" y2="12.4549" layer="94"/>
<rectangle x1="-38.218" y1="12.4549" x2="-36.9201" y2="12.4727" layer="94"/>
<rectangle x1="-38.3425" y1="12.4727" x2="-36.8312" y2="12.4905" layer="94"/>
<rectangle x1="-38.467" y1="12.4905" x2="-36.7423" y2="12.5083" layer="94"/>
<rectangle x1="-38.5736" y1="12.5083" x2="-36.6712" y2="12.5261" layer="94"/>
<rectangle x1="-38.6803" y1="12.5261" x2="-36.6001" y2="12.5438" layer="94"/>
<rectangle x1="-38.7692" y1="12.5438" x2="-36.5467" y2="12.5616" layer="94"/>
<rectangle x1="-38.8581" y1="12.5616" x2="-36.4934" y2="12.5794" layer="94"/>
<rectangle x1="-38.947" y1="12.5794" x2="-36.7779" y2="12.5972" layer="94"/>
<rectangle x1="-39.0359" y1="12.5972" x2="-36.9379" y2="12.615" layer="94"/>
<rectangle x1="-39.107" y1="12.615" x2="-37.0268" y2="12.6327" layer="94"/>
<rectangle x1="-39.1782" y1="12.6327" x2="-37.1157" y2="12.6505" layer="94"/>
<rectangle x1="-39.2671" y1="12.6505" x2="-37.169" y2="12.6683" layer="94"/>
<rectangle x1="-39.3382" y1="12.6683" x2="-37.2224" y2="12.6861" layer="94"/>
<rectangle x1="-39.3915" y1="12.6861" x2="-37.2757" y2="12.7039" layer="94"/>
<rectangle x1="-39.4626" y1="12.7039" x2="-37.3113" y2="12.7216" layer="94"/>
<rectangle x1="-39.4804" y1="12.7216" x2="-37.3468" y2="12.7394" layer="94"/>
<rectangle x1="-39.4804" y1="12.7394" x2="-37.3646" y2="12.7572" layer="94"/>
<rectangle x1="-39.4804" y1="12.7572" x2="-37.4002" y2="12.775" layer="94"/>
<rectangle x1="-39.4804" y1="12.775" x2="-37.4179" y2="12.7928" layer="94"/>
<rectangle x1="-39.4626" y1="12.7928" x2="-37.4357" y2="12.8105" layer="94"/>
<rectangle x1="-39.4626" y1="12.8105" x2="-37.4713" y2="12.8283" layer="94"/>
<rectangle x1="-39.4626" y1="12.8283" x2="-37.4891" y2="12.8461" layer="94"/>
<rectangle x1="-39.4626" y1="12.8461" x2="-37.4891" y2="12.8639" layer="94"/>
<rectangle x1="-36.4578" y1="12.8461" x2="-36.4223" y2="12.8639" layer="94"/>
<rectangle x1="-39.4449" y1="12.8639" x2="-37.5068" y2="12.8817" layer="94"/>
<rectangle x1="-36.4934" y1="12.8639" x2="-36.44" y2="12.8817" layer="94"/>
<rectangle x1="-39.4449" y1="12.8817" x2="-37.5246" y2="12.8994" layer="94"/>
<rectangle x1="-36.5289" y1="12.8817" x2="-36.44" y2="12.8994" layer="94"/>
<rectangle x1="-39.4449" y1="12.8994" x2="-37.5424" y2="12.9172" layer="94"/>
<rectangle x1="-36.5645" y1="12.8994" x2="-36.44" y2="12.9172" layer="94"/>
<rectangle x1="-39.4271" y1="12.9172" x2="-37.5424" y2="12.935" layer="94"/>
<rectangle x1="-36.6178" y1="12.9172" x2="-36.4578" y2="12.935" layer="94"/>
<rectangle x1="-39.4271" y1="12.935" x2="-37.5602" y2="12.9528" layer="94"/>
<rectangle x1="-36.6534" y1="12.935" x2="-36.4578" y2="12.9528" layer="94"/>
<rectangle x1="-39.4271" y1="12.9528" x2="-37.5602" y2="12.9706" layer="94"/>
<rectangle x1="-36.689" y1="12.9528" x2="-36.4578" y2="12.9706" layer="94"/>
<rectangle x1="-39.4093" y1="12.9706" x2="-37.578" y2="12.9883" layer="94"/>
<rectangle x1="-36.7245" y1="12.9706" x2="-36.4756" y2="12.9883" layer="94"/>
<rectangle x1="-39.4093" y1="12.9883" x2="-37.578" y2="13.0061" layer="94"/>
<rectangle x1="-36.7601" y1="12.9883" x2="-36.4756" y2="13.0061" layer="94"/>
<rectangle x1="-39.4093" y1="13.0061" x2="-37.5957" y2="13.0239" layer="94"/>
<rectangle x1="-36.7956" y1="13.0061" x2="-36.4756" y2="13.0239" layer="94"/>
<rectangle x1="-39.3915" y1="13.0239" x2="-37.5957" y2="13.0417" layer="94"/>
<rectangle x1="-36.849" y1="13.0239" x2="-36.4934" y2="13.0417" layer="94"/>
<rectangle x1="-39.3915" y1="13.0417" x2="-37.5957" y2="13.0595" layer="94"/>
<rectangle x1="-36.8845" y1="13.0417" x2="-36.4934" y2="13.0595" layer="94"/>
<rectangle x1="-39.3915" y1="13.0595" x2="-37.5957" y2="13.0772" layer="94"/>
<rectangle x1="-36.9201" y1="13.0595" x2="-36.5112" y2="13.0772" layer="94"/>
<rectangle x1="-39.3737" y1="13.0772" x2="-37.5957" y2="13.095" layer="94"/>
<rectangle x1="-36.9734" y1="13.0772" x2="-36.5112" y2="13.095" layer="94"/>
<rectangle x1="-39.3737" y1="13.095" x2="-37.5957" y2="13.1128" layer="94"/>
<rectangle x1="-37.009" y1="13.095" x2="-36.5112" y2="13.1128" layer="94"/>
<rectangle x1="-39.356" y1="13.1128" x2="-37.5957" y2="13.1306" layer="94"/>
<rectangle x1="-37.0623" y1="13.1128" x2="-36.5289" y2="13.1306" layer="94"/>
<rectangle x1="-39.356" y1="13.1306" x2="-37.5957" y2="13.1484" layer="94"/>
<rectangle x1="-37.1157" y1="13.1306" x2="-36.5289" y2="13.1484" layer="94"/>
<rectangle x1="-39.3382" y1="13.1484" x2="-37.578" y2="13.1661" layer="94"/>
<rectangle x1="-37.169" y1="13.1484" x2="-36.5467" y2="13.1661" layer="94"/>
<rectangle x1="-39.3382" y1="13.1661" x2="-37.5602" y2="13.1839" layer="94"/>
<rectangle x1="-37.2401" y1="13.1661" x2="-36.5467" y2="13.1839" layer="94"/>
<rectangle x1="-39.3382" y1="13.1839" x2="-37.5246" y2="13.2017" layer="94"/>
<rectangle x1="-37.3113" y1="13.1839" x2="-36.5467" y2="13.2017" layer="94"/>
<rectangle x1="-39.3204" y1="13.2017" x2="-36.5645" y2="13.2195" layer="94"/>
<rectangle x1="-39.3204" y1="13.2195" x2="-36.5645" y2="13.2373" layer="94"/>
<rectangle x1="-39.3026" y1="13.2373" x2="-36.5823" y2="13.255" layer="94"/>
<rectangle x1="-39.3026" y1="13.255" x2="-36.5823" y2="13.2728" layer="94"/>
<rectangle x1="-39.2848" y1="13.2728" x2="-36.6001" y2="13.2906" layer="94"/>
<rectangle x1="-39.2848" y1="13.2906" x2="-36.6178" y2="13.3084" layer="94"/>
<rectangle x1="-39.2671" y1="13.3084" x2="-36.6178" y2="13.3262" layer="94"/>
<rectangle x1="-39.2493" y1="13.3262" x2="-36.6356" y2="13.3439" layer="94"/>
<rectangle x1="-39.2493" y1="13.3439" x2="-36.6356" y2="13.3617" layer="94"/>
<rectangle x1="-39.2315" y1="13.3617" x2="-36.6534" y2="13.3795" layer="94"/>
<rectangle x1="-39.2315" y1="13.3795" x2="-36.6534" y2="13.3973" layer="94"/>
<rectangle x1="-39.2137" y1="13.3973" x2="-36.6712" y2="13.4151" layer="94"/>
<rectangle x1="-39.1959" y1="13.4151" x2="-36.689" y2="13.4328" layer="94"/>
<rectangle x1="-39.1959" y1="13.4328" x2="-36.689" y2="13.4506" layer="94"/>
<rectangle x1="-39.1782" y1="13.4506" x2="-36.7067" y2="13.4684" layer="94"/>
<rectangle x1="-39.1604" y1="13.4684" x2="-36.7245" y2="13.4862" layer="94"/>
<rectangle x1="-39.1604" y1="13.4862" x2="-36.7245" y2="13.504" layer="94"/>
<rectangle x1="-39.1426" y1="13.504" x2="-36.7423" y2="13.5217" layer="94"/>
<rectangle x1="-39.1248" y1="13.5217" x2="-36.7601" y2="13.5395" layer="94"/>
<rectangle x1="-39.107" y1="13.5395" x2="-36.7779" y2="13.5573" layer="94"/>
<rectangle x1="-39.107" y1="13.5573" x2="-36.7779" y2="13.5751" layer="94"/>
<rectangle x1="-39.0893" y1="13.5751" x2="-36.7956" y2="13.5929" layer="94"/>
<rectangle x1="-39.0715" y1="13.5929" x2="-36.8134" y2="13.6106" layer="94"/>
<rectangle x1="-39.0537" y1="13.6106" x2="-36.8312" y2="13.6284" layer="94"/>
<rectangle x1="-39.0359" y1="13.6284" x2="-36.849" y2="13.6462" layer="94"/>
<rectangle x1="-39.0181" y1="13.6462" x2="-36.8668" y2="13.664" layer="94"/>
<rectangle x1="-39.0004" y1="13.664" x2="-36.8845" y2="13.6818" layer="94"/>
<rectangle x1="-38.9826" y1="13.6818" x2="-36.8845" y2="13.6995" layer="94"/>
<rectangle x1="-38.9648" y1="13.6995" x2="-36.9023" y2="13.7173" layer="94"/>
<rectangle x1="-38.947" y1="13.7173" x2="-36.9201" y2="13.7351" layer="94"/>
<rectangle x1="-38.9292" y1="13.7351" x2="-36.9379" y2="13.7529" layer="94"/>
<rectangle x1="-38.9115" y1="13.7529" x2="-36.9734" y2="13.7707" layer="94"/>
<rectangle x1="-38.8937" y1="13.7707" x2="-36.9912" y2="13.7884" layer="94"/>
<rectangle x1="-38.8759" y1="13.7884" x2="-37.009" y2="13.8062" layer="94"/>
<rectangle x1="-38.8581" y1="13.8062" x2="-37.0268" y2="13.824" layer="94"/>
<rectangle x1="-38.8226" y1="13.824" x2="-37.0446" y2="13.8418" layer="94"/>
<rectangle x1="-38.8048" y1="13.8418" x2="-37.0623" y2="13.8596" layer="94"/>
<rectangle x1="-38.787" y1="13.8596" x2="-37.0979" y2="13.8773" layer="94"/>
<rectangle x1="-38.7692" y1="13.8773" x2="-37.1157" y2="13.8951" layer="94"/>
<rectangle x1="-38.7337" y1="13.8951" x2="-37.1335" y2="13.9129" layer="94"/>
<rectangle x1="-38.7159" y1="13.9129" x2="-37.169" y2="13.9307" layer="94"/>
<rectangle x1="-38.6803" y1="13.9307" x2="-37.1868" y2="13.9485" layer="94"/>
<rectangle x1="-38.6625" y1="13.9485" x2="-37.2224" y2="13.9662" layer="94"/>
<rectangle x1="-38.627" y1="13.9662" x2="-37.2401" y2="13.984" layer="94"/>
<rectangle x1="-38.6092" y1="13.984" x2="-37.2757" y2="14.0018" layer="94"/>
<rectangle x1="-38.5736" y1="14.0018" x2="-37.3113" y2="14.0196" layer="94"/>
<rectangle x1="-38.5381" y1="14.0196" x2="-37.3468" y2="14.0374" layer="94"/>
<rectangle x1="-38.5025" y1="14.0374" x2="-37.3824" y2="14.0551" layer="94"/>
<rectangle x1="-38.467" y1="14.0551" x2="-37.4179" y2="14.0729" layer="94"/>
<rectangle x1="-38.4136" y1="14.0729" x2="-37.4535" y2="14.0907" layer="94"/>
<rectangle x1="-38.3603" y1="14.0907" x2="-37.5068" y2="14.1085" layer="94"/>
<rectangle x1="-38.3069" y1="14.1085" x2="-37.5602" y2="14.1263" layer="94"/>
<rectangle x1="-38.2358" y1="14.1263" x2="-37.6313" y2="14.144" layer="94"/>
<rectangle x1="-38.1469" y1="14.144" x2="-37.7202" y2="14.1618" layer="94"/>
</symbol>
<symbol name="FRAME2-A4">
<frame x1="0" y1="0" x2="248.92" y2="185.42" columns="8" rows="5" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="DOCFIELD1">
<description>&lt;b&gt;DOCUMENT FIELDS TYPE 1&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="DOCFIELD1" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="FRAME2-A4">
<description>&lt;b&gt;FRAME TYPE 2 - A4&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="FRAME2-A4" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Electromechanical">
<description>&lt;h3&gt;SparkFun Electronics' preferred foot prints&lt;/h3&gt;
In this library you'll find anything that moves- switches, relays, buttons, potentiometers. Also, anything that goes on a board but isn't electrical in nature- screws, standoffs, etc.&lt;br&gt;&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is the end user's responsibility to ensure correctness and suitablity for a given componet or application. If you enjoy using this library, please buy one of our products at www.sparkfun.com.
&lt;br&gt;&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="TACTILE-PTH">
<description>&lt;b&gt;OMRON SWITCH&lt;/b&gt;</description>
<wire x1="3.048" y1="1.016" x2="3.048" y2="2.54" width="0.2032" layer="51"/>
<wire x1="3.048" y1="2.54" x2="2.54" y2="3.048" width="0.2032" layer="51"/>
<wire x1="2.54" y1="-3.048" x2="3.048" y2="-2.54" width="0.2032" layer="51"/>
<wire x1="3.048" y1="-2.54" x2="3.048" y2="-1.016" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="3.048" x2="-3.048" y2="2.54" width="0.2032" layer="51"/>
<wire x1="-3.048" y1="2.54" x2="-3.048" y2="1.016" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="-3.048" x2="-3.048" y2="-2.54" width="0.2032" layer="51"/>
<wire x1="-3.048" y1="-2.54" x2="-3.048" y2="-1.016" width="0.2032" layer="51"/>
<wire x1="2.54" y1="-3.048" x2="2.159" y2="-3.048" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="-3.048" x2="-2.159" y2="-3.048" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="3.048" x2="-2.159" y2="3.048" width="0.2032" layer="51"/>
<wire x1="2.54" y1="3.048" x2="2.159" y2="3.048" width="0.2032" layer="51"/>
<wire x1="2.159" y1="3.048" x2="-2.159" y2="3.048" width="0.2032" layer="21"/>
<wire x1="-2.159" y1="-3.048" x2="2.159" y2="-3.048" width="0.2032" layer="21"/>
<wire x1="3.048" y1="0.998" x2="3.048" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-3.048" y1="1.028" x2="-3.048" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="1.27" x2="-2.54" y2="0.508" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="-0.508" x2="-2.54" y2="-1.27" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="0.508" x2="-2.159" y2="-0.381" width="0.2032" layer="51"/>
<circle x="0" y="0" radius="1.778" width="0.2032" layer="21"/>
<pad name="1" x="-3.2512" y="2.2606" drill="1.016" diameter="1.8796"/>
<pad name="2" x="3.2512" y="2.2606" drill="1.016" diameter="1.8796"/>
<pad name="3" x="-3.2512" y="-2.2606" drill="1.016" diameter="1.8796"/>
<pad name="4" x="3.2512" y="-2.2606" drill="1.016" diameter="1.8796"/>
<text x="-2.54" y="3.81" size="1.27" layer="25" ratio="10">&gt;NAME</text>
</package>
<package name="TACTILE_SWITCH_SMD-2">
<wire x1="1.905" y1="1.27" x2="1.905" y2="0.445" width="0.127" layer="51"/>
<wire x1="1.905" y1="0.445" x2="2.16" y2="-0.01" width="0.127" layer="51"/>
<wire x1="1.905" y1="-0.23" x2="1.905" y2="-1.115" width="0.127" layer="51"/>
<wire x1="-2.25" y1="2.25" x2="2.25" y2="2.25" width="0.127" layer="51"/>
<wire x1="2.25" y1="2.25" x2="2.25" y2="-2.25" width="0.127" layer="51"/>
<wire x1="2.25" y1="-2.25" x2="-2.25" y2="-2.25" width="0.127" layer="51"/>
<wire x1="-2.25" y1="-2.25" x2="-2.25" y2="2.25" width="0.127" layer="51"/>
<wire x1="-2.2" y1="0.8" x2="-2.2" y2="-0.8" width="0.2032" layer="21"/>
<wire x1="1.3" y1="2.2" x2="-1.3" y2="2.2" width="0.2032" layer="21"/>
<wire x1="2.2" y1="-0.8" x2="2.2" y2="0.8" width="0.2032" layer="21"/>
<wire x1="-1.3" y1="-2.2" x2="1.3" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="2.2" y1="0.8" x2="1.8" y2="0.8" width="0.2032" layer="21"/>
<wire x1="2.2" y1="-0.8" x2="1.8" y2="-0.8" width="0.2032" layer="21"/>
<wire x1="-1.8" y1="0.8" x2="-2.2" y2="0.8" width="0.2032" layer="21"/>
<wire x1="-1.8" y1="-0.8" x2="-2.2" y2="-0.8" width="0.2032" layer="21"/>
<circle x="0" y="0" radius="1.27" width="0.2032" layer="21"/>
<smd name="1" x="2.225" y="1.75" dx="1.1" dy="0.7" layer="1" rot="R90"/>
<smd name="2" x="2.225" y="-1.75" dx="1.1" dy="0.7" layer="1" rot="R90"/>
<smd name="3" x="-2.225" y="-1.75" dx="1.1" dy="0.7" layer="1" rot="R90"/>
<smd name="4" x="-2.225" y="1.75" dx="1.1" dy="0.7" layer="1" rot="R90"/>
<text x="-0.889" y="1.778" size="0.4064" layer="25">&gt;NAME</text>
<text x="-0.889" y="-2.032" size="0.4064" layer="27">&gt;Value</text>
</package>
<package name="TACTILE-PTH-12MM">
<wire x1="5" y1="-1.3" x2="5" y2="-0.7" width="0.2032" layer="51"/>
<wire x1="5" y1="-0.7" x2="4.5" y2="-0.2" width="0.2032" layer="51"/>
<wire x1="5" y1="0.2" x2="5" y2="1" width="0.2032" layer="51"/>
<wire x1="-6" y1="4" x2="-6" y2="5" width="0.2032" layer="21"/>
<wire x1="-5" y1="6" x2="5" y2="6" width="0.2032" layer="21"/>
<wire x1="6" y1="5" x2="6" y2="4" width="0.2032" layer="21"/>
<wire x1="6" y1="1" x2="6" y2="-1" width="0.2032" layer="21"/>
<wire x1="6" y1="-4" x2="6" y2="-5" width="0.2032" layer="21"/>
<wire x1="5" y1="-6" x2="-5" y2="-6" width="0.2032" layer="21"/>
<wire x1="-6" y1="-5" x2="-6" y2="-4" width="0.2032" layer="21"/>
<wire x1="-6" y1="-1" x2="-6" y2="1" width="0.2032" layer="21"/>
<wire x1="-6" y1="5" x2="-5" y2="6" width="0.2032" layer="21" curve="-90"/>
<wire x1="5" y1="6" x2="6" y2="5" width="0.2032" layer="21" curve="-90"/>
<wire x1="6" y1="-5" x2="5" y2="-6" width="0.2032" layer="21" curve="-90"/>
<wire x1="-5" y1="-6" x2="-6" y2="-5" width="0.2032" layer="21" curve="-90"/>
<circle x="0" y="0" radius="3.5" width="0.2032" layer="21"/>
<circle x="-4.5" y="4.5" radius="0.3" width="0.7" layer="21"/>
<circle x="4.5" y="4.5" radius="0.3" width="0.7" layer="21"/>
<circle x="4.5" y="-4.5" radius="0.3" width="0.7" layer="21"/>
<circle x="-4.5" y="-4.5" radius="0.3" width="0.7" layer="21"/>
<pad name="4" x="-6.25" y="2.5" drill="1.2" diameter="2.159"/>
<pad name="2" x="-6.25" y="-2.5" drill="1.2" diameter="2.159"/>
<pad name="1" x="6.25" y="-2.5" drill="1.2" diameter="2.159"/>
<pad name="3" x="6.25" y="2.5" drill="1.2" diameter="2.159"/>
</package>
<package name="TACTILE-SWITCH-1101NE">
<description>SparkFun SKU# COM-08229</description>
<wire x1="-3" y1="1.1" x2="-3" y2="-1.1" width="0.127" layer="51"/>
<wire x1="3" y1="1.1" x2="3" y2="-1.1" width="0.127" layer="51"/>
<wire x1="-2.75" y1="1.75" x2="-3" y2="1.5" width="0.2032" layer="21" curve="90"/>
<wire x1="-2.75" y1="1.75" x2="2.75" y2="1.75" width="0.2032" layer="21"/>
<wire x1="2.75" y1="1.75" x2="3" y2="1.5" width="0.2032" layer="21" curve="-90"/>
<wire x1="3" y1="-1.5" x2="2.75" y2="-1.75" width="0.2032" layer="21" curve="-90"/>
<wire x1="2.75" y1="-1.75" x2="-2.75" y2="-1.75" width="0.2032" layer="21"/>
<wire x1="-3" y1="-1.5" x2="-2.75" y2="-1.75" width="0.2032" layer="21" curve="90"/>
<wire x1="-3" y1="-1.5" x2="-3" y2="-1.1" width="0.2032" layer="21"/>
<wire x1="-3" y1="1.1" x2="-3" y2="1.5" width="0.2032" layer="21"/>
<wire x1="3" y1="1.1" x2="3" y2="1.5" width="0.2032" layer="21"/>
<wire x1="3" y1="-1.5" x2="3" y2="-1.1" width="0.2032" layer="21"/>
<wire x1="-1.5" y1="0.75" x2="1.5" y2="0.75" width="0.2032" layer="21"/>
<wire x1="1.5" y1="-0.75" x2="-1.5" y2="-0.75" width="0.2032" layer="21"/>
<wire x1="-1.5" y1="-0.75" x2="-1.5" y2="0.75" width="0.2032" layer="21"/>
<wire x1="1.5" y1="-0.75" x2="1.5" y2="0.75" width="0.2032" layer="21"/>
<wire x1="-2" y1="0" x2="-1" y2="0" width="0.127" layer="51"/>
<wire x1="-1" y1="0" x2="0.1" y2="0.5" width="0.127" layer="51"/>
<wire x1="0.3" y1="0" x2="2" y2="0" width="0.127" layer="51"/>
<smd name="1" x="-3.15" y="0" dx="2.3" dy="1.6" layer="1" rot="R180"/>
<smd name="2" x="3.15" y="0" dx="2.3" dy="1.6" layer="1" rot="R180"/>
<text x="-3" y="2" size="0.762" layer="25">&gt;NAME</text>
<text x="-3" y="-2.7" size="0.762" layer="27">&gt;VALUE</text>
</package>
<package name="REED_SWITCH_PTH">
<wire x1="-6.985" y1="-0.635" x2="6.985" y2="-0.635" width="0.127" layer="21"/>
<wire x1="-6.985" y1="-0.635" x2="-6.985" y2="0" width="0.127" layer="21"/>
<wire x1="-6.985" y1="0" x2="-6.985" y2="0.635" width="0.127" layer="21"/>
<wire x1="-6.985" y1="0.635" x2="6.985" y2="0.635" width="0.127" layer="21"/>
<wire x1="6.985" y1="0.635" x2="6.985" y2="0" width="0.127" layer="21"/>
<wire x1="6.985" y1="0" x2="6.985" y2="-0.635" width="0.127" layer="21"/>
<wire x1="-6.985" y1="0" x2="-7.62" y2="0" width="0.127" layer="21"/>
<wire x1="6.985" y1="0" x2="7.62" y2="0" width="0.127" layer="21"/>
<pad name="P$1" x="-8.89" y="0" drill="1.016" diameter="1.8796"/>
<pad name="P$2" x="8.89" y="0" drill="1.016" diameter="1.8796"/>
</package>
<package name="TACTILE_SWITCH_TALL">
<wire x1="-3" y1="-3" x2="3" y2="-3" width="0.254" layer="21"/>
<wire x1="3" y1="-3" x2="3" y2="3" width="0.254" layer="21"/>
<wire x1="3" y1="3" x2="-3" y2="3" width="0.254" layer="21"/>
<wire x1="-3" y1="3" x2="-3" y2="-3" width="0.254" layer="21"/>
<circle x="0" y="0" radius="1.75" width="0.254" layer="21"/>
<smd name="A1" x="-3.975" y="-2.25" dx="1.3" dy="1.55" layer="1" rot="R90"/>
<smd name="A2" x="3.975" y="-2.25" dx="1.3" dy="1.55" layer="1" rot="R90"/>
<smd name="B1" x="-3.975" y="2.25" dx="1.3" dy="1.55" layer="1" rot="R90"/>
<smd name="B2" x="3.975" y="2.25" dx="1.3" dy="1.55" layer="1" rot="R90"/>
</package>
<package name="REED_SWITCH_PLASTIC">
<wire x1="-7.5" y1="-1.65" x2="7.5" y2="-1.65" width="0.127" layer="21"/>
<wire x1="-7.5" y1="-1.65" x2="-7.5" y2="0" width="0.127" layer="21"/>
<wire x1="-7.5" y1="0" x2="-7.5" y2="1.65" width="0.127" layer="21"/>
<wire x1="-7.5" y1="1.65" x2="7.5" y2="1.65" width="0.127" layer="21"/>
<wire x1="7.5" y1="1.65" x2="7.5" y2="0" width="0.127" layer="21"/>
<wire x1="7.5" y1="0" x2="7.5" y2="-1.65" width="0.127" layer="21"/>
<wire x1="-7.5" y1="0" x2="-7.72" y2="0" width="0.127" layer="21"/>
<wire x1="7.5" y1="0" x2="7.72" y2="0" width="0.127" layer="21"/>
<pad name="P$1" x="-8.89" y="0" drill="1.016" diameter="1.8796"/>
<pad name="P$2" x="8.89" y="0" drill="1.016" diameter="1.8796"/>
</package>
<package name="TACTILE-PTH-SIDEEZ">
<wire x1="1.5" y1="-3.8" x2="-1.5" y2="-3.8" width="0.2032" layer="51"/>
<wire x1="-3.65" y1="-2" x2="-3.65" y2="3.5" width="0.2032" layer="51"/>
<wire x1="-3.65" y1="3.5" x2="-3" y2="3.5" width="0.2032" layer="51"/>
<wire x1="3" y1="3.5" x2="3.65" y2="3.5" width="0.2032" layer="51"/>
<wire x1="3.65" y1="3.5" x2="3.65" y2="-2" width="0.2032" layer="51"/>
<wire x1="-3" y1="2" x2="3" y2="2" width="0.2032" layer="51"/>
<wire x1="-3" y1="2" x2="-3" y2="3.5" width="0.2032" layer="51"/>
<wire x1="3" y1="2" x2="3" y2="3.5" width="0.2032" layer="51"/>
<wire x1="-3.65" y1="-2" x2="-1.5" y2="-2" width="0.2032" layer="51"/>
<wire x1="-1.5" y1="-2" x2="1.5" y2="-2" width="0.2032" layer="51"/>
<wire x1="1.5" y1="-2" x2="3.65" y2="-2" width="0.2032" layer="51"/>
<wire x1="1.5" y1="-2" x2="1.5" y2="-3.8" width="0.2032" layer="51"/>
<wire x1="-1.5" y1="-2" x2="-1.5" y2="-3.8" width="0.2032" layer="51"/>
<wire x1="-3.65" y1="1" x2="-3.65" y2="-2" width="0.2032" layer="21"/>
<wire x1="-3.65" y1="-2" x2="3.65" y2="-2" width="0.2032" layer="21"/>
<wire x1="3.65" y1="-2" x2="3.65" y2="1" width="0.2032" layer="21"/>
<wire x1="2" y1="2" x2="-2" y2="2" width="0.2032" layer="21"/>
<pad name="ANCHOR1" x="-3.5" y="2.5" drill="1.2" diameter="2.2" stop="no"/>
<pad name="ANCHOR2" x="3.5" y="2.5" drill="1.2" diameter="2.2" stop="no"/>
<pad name="1" x="-2.5" y="0" drill="0.8" diameter="1.7" stop="no"/>
<pad name="2" x="2.5" y="0" drill="0.8" diameter="1.7" stop="no"/>
<text x="-2.54" y="3.81" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<circle x="2.5" y="0" radius="0.4445" width="0" layer="29"/>
<circle x="-2.5" y="0" radius="0.4445" width="0" layer="29"/>
<circle x="-3.5" y="2.5" radius="0.635" width="0" layer="29"/>
<circle x="3.5" y="2.5" radius="0.635" width="0" layer="29"/>
<circle x="-3.5" y="2.5" radius="1.143" width="0" layer="30"/>
<circle x="2.5" y="0" radius="0.889" width="0" layer="30"/>
<circle x="-2.5" y="0" radius="0.889" width="0" layer="30"/>
<circle x="3.5" y="2.5" radius="1.143" width="0" layer="30"/>
</package>
<package name="TACTILE_SWITCH_SMD-3">
<wire x1="-2.04" y1="-0.44" x2="-2.04" y2="0.47" width="0.2032" layer="21"/>
<wire x1="-1.04" y1="1.14" x2="1.04" y2="1.14" width="0.2032" layer="21"/>
<circle x="0" y="0" radius="0.8" width="0.15" layer="21"/>
<smd name="1" x="-1.8" y="1.1" dx="0.8" dy="1" layer="1" rot="R90"/>
<smd name="2" x="1.8" y="1.1" dx="0.8" dy="1" layer="1" rot="R90"/>
<smd name="3" x="-1.8" y="-1.1" dx="0.8" dy="1" layer="1" rot="R90"/>
<smd name="4" x="1.8" y="-1.1" dx="0.8" dy="1" layer="1" rot="R90"/>
<wire x1="2.06" y1="-0.44" x2="2.06" y2="0.47" width="0.2032" layer="21"/>
<wire x1="-1.04" y1="-1.16" x2="1.04" y2="-1.16" width="0.2032" layer="21"/>
</package>
<package name="TACTILE-SMD-12MM">
<wire x1="5" y1="-1.3" x2="5" y2="-0.7" width="0.2032" layer="51"/>
<wire x1="5" y1="-0.7" x2="4.5" y2="-0.2" width="0.2032" layer="51"/>
<wire x1="5" y1="0.2" x2="5" y2="1" width="0.2032" layer="51"/>
<wire x1="-6" y1="4" x2="-6" y2="5" width="0.2032" layer="21"/>
<wire x1="-5" y1="6" x2="5" y2="6" width="0.2032" layer="21"/>
<wire x1="6" y1="5" x2="6" y2="4" width="0.2032" layer="21"/>
<wire x1="6" y1="1" x2="6" y2="-1" width="0.2032" layer="21"/>
<wire x1="6" y1="-4" x2="6" y2="-5" width="0.2032" layer="21"/>
<wire x1="5" y1="-6" x2="-5" y2="-6" width="0.2032" layer="21"/>
<wire x1="-6" y1="-5" x2="-6" y2="-4" width="0.2032" layer="21"/>
<wire x1="-6" y1="-1" x2="-6" y2="1" width="0.2032" layer="21"/>
<circle x="0" y="0" radius="3.5" width="0.2032" layer="21"/>
<circle x="-4.5" y="4.5" radius="0.3" width="0.7" layer="21"/>
<circle x="4.5" y="4.5" radius="0.3" width="0.7" layer="21"/>
<circle x="4.5" y="-4.5" radius="0.3" width="0.7" layer="21"/>
<circle x="-4.5" y="-4.5" radius="0.3" width="0.7" layer="21"/>
<smd name="4" x="-6.975" y="2.5" dx="1.6" dy="1.55" layer="1"/>
<smd name="2" x="-6.975" y="-2.5" dx="1.6" dy="1.55" layer="1"/>
<smd name="1" x="6.975" y="-2.5" dx="1.6" dy="1.55" layer="1"/>
<smd name="3" x="6.975" y="2.5" dx="1.6" dy="1.55" layer="1"/>
<wire x1="-6" y1="-5" x2="-5" y2="-6" width="0.2032" layer="21"/>
<wire x1="6" y1="-5" x2="5" y2="-6" width="0.2032" layer="21"/>
<wire x1="6" y1="5" x2="5" y2="6" width="0.2032" layer="21"/>
<wire x1="-5" y1="6" x2="-6" y2="5" width="0.2032" layer="21"/>
</package>
<package name="TACTILE-PTH-EZ">
<wire x1="3.048" y1="1.016" x2="3.048" y2="2.54" width="0.2032" layer="51"/>
<wire x1="3.048" y1="2.54" x2="2.54" y2="3.048" width="0.2032" layer="51"/>
<wire x1="2.54" y1="-3.048" x2="3.048" y2="-2.54" width="0.2032" layer="51"/>
<wire x1="3.048" y1="-2.54" x2="3.048" y2="-1.016" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="3.048" x2="-3.048" y2="2.54" width="0.2032" layer="51"/>
<wire x1="-3.048" y1="2.54" x2="-3.048" y2="1.016" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="-3.048" x2="-3.048" y2="-2.54" width="0.2032" layer="51"/>
<wire x1="-3.048" y1="-2.54" x2="-3.048" y2="-1.016" width="0.2032" layer="51"/>
<wire x1="2.54" y1="-3.048" x2="2.159" y2="-3.048" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="-3.048" x2="-2.159" y2="-3.048" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="3.048" x2="-2.159" y2="3.048" width="0.2032" layer="51"/>
<wire x1="2.54" y1="3.048" x2="2.159" y2="3.048" width="0.2032" layer="51"/>
<wire x1="2.159" y1="3.048" x2="-2.159" y2="3.048" width="0.2032" layer="21"/>
<wire x1="-2.159" y1="-3.048" x2="2.159" y2="-3.048" width="0.2032" layer="21"/>
<wire x1="3.048" y1="0.998" x2="3.048" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-3.048" y1="1.028" x2="-3.048" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="1.27" x2="-2.54" y2="0.508" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="-0.508" x2="-2.54" y2="-1.27" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="0.508" x2="-2.159" y2="-0.381" width="0.2032" layer="51"/>
<circle x="0" y="0" radius="1.778" width="0.2032" layer="21"/>
<pad name="1" x="-3.2512" y="2.2606" drill="1.016" diameter="1.8796" stop="no"/>
<pad name="2" x="3.2512" y="2.2606" drill="1.016" diameter="1.8796" stop="no"/>
<pad name="3" x="-3.2512" y="-2.2606" drill="1.016" diameter="1.8796" stop="no"/>
<pad name="4" x="3.2512" y="-2.2606" drill="1.016" diameter="1.8796" stop="no"/>
<text x="-2.54" y="3.81" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<polygon width="0.127" layer="30">
<vertex x="-3.2664" y="3.142"/>
<vertex x="-3.2589" y="3.1445" curve="89.986886"/>
<vertex x="-4.1326" y="2.286"/>
<vertex x="-4.1351" y="2.2657" curve="90.00652"/>
<vertex x="-3.2563" y="1.392"/>
<vertex x="-3.2487" y="1.3869" curve="90.006616"/>
<vertex x="-2.3826" y="2.2403"/>
<vertex x="-2.3775" y="2.2683" curve="89.98711"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="-3.2462" y="2.7026"/>
<vertex x="-3.2589" y="2.7051" curve="90.026544"/>
<vertex x="-3.6881" y="2.2733"/>
<vertex x="-3.6881" y="2.2632" curve="89.974074"/>
<vertex x="-3.2562" y="1.8213"/>
<vertex x="-3.2259" y="1.8186" curve="90.051271"/>
<vertex x="-2.8093" y="2.2658"/>
<vertex x="-2.8093" y="2.2606" curve="90.012964"/>
</polygon>
<polygon width="0.127" layer="30">
<vertex x="3.2411" y="3.1395"/>
<vertex x="3.2486" y="3.142" curve="89.986886"/>
<vertex x="2.3749" y="2.2835"/>
<vertex x="2.3724" y="2.2632" curve="90.00652"/>
<vertex x="3.2512" y="1.3895"/>
<vertex x="3.2588" y="1.3844" curve="90.006616"/>
<vertex x="4.1249" y="2.2378"/>
<vertex x="4.13" y="2.2658" curve="89.98711"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="3.2613" y="2.7001"/>
<vertex x="3.2486" y="2.7026" curve="90.026544"/>
<vertex x="2.8194" y="2.2708"/>
<vertex x="2.8194" y="2.2607" curve="89.974074"/>
<vertex x="3.2513" y="1.8188"/>
<vertex x="3.2816" y="1.8161" curve="90.051271"/>
<vertex x="3.6982" y="2.2633"/>
<vertex x="3.6982" y="2.2581" curve="90.012964"/>
</polygon>
<polygon width="0.127" layer="30">
<vertex x="-3.2613" y="-1.3868"/>
<vertex x="-3.2538" y="-1.3843" curve="89.986886"/>
<vertex x="-4.1275" y="-2.2428"/>
<vertex x="-4.13" y="-2.2631" curve="90.00652"/>
<vertex x="-3.2512" y="-3.1368"/>
<vertex x="-3.2436" y="-3.1419" curve="90.006616"/>
<vertex x="-2.3775" y="-2.2885"/>
<vertex x="-2.3724" y="-2.2605" curve="89.98711"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="-3.2411" y="-1.8262"/>
<vertex x="-3.2538" y="-1.8237" curve="90.026544"/>
<vertex x="-3.683" y="-2.2555"/>
<vertex x="-3.683" y="-2.2656" curve="89.974074"/>
<vertex x="-3.2511" y="-2.7075"/>
<vertex x="-3.2208" y="-2.7102" curve="90.051271"/>
<vertex x="-2.8042" y="-2.263"/>
<vertex x="-2.8042" y="-2.2682" curve="90.012964"/>
</polygon>
<polygon width="0.127" layer="30">
<vertex x="3.2411" y="-1.3843"/>
<vertex x="3.2486" y="-1.3818" curve="89.986886"/>
<vertex x="2.3749" y="-2.2403"/>
<vertex x="2.3724" y="-2.2606" curve="90.00652"/>
<vertex x="3.2512" y="-3.1343"/>
<vertex x="3.2588" y="-3.1394" curve="90.006616"/>
<vertex x="4.1249" y="-2.286"/>
<vertex x="4.13" y="-2.258" curve="89.98711"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="3.2613" y="-1.8237"/>
<vertex x="3.2486" y="-1.8212" curve="90.026544"/>
<vertex x="2.8194" y="-2.253"/>
<vertex x="2.8194" y="-2.2631" curve="89.974074"/>
<vertex x="3.2513" y="-2.705"/>
<vertex x="3.2816" y="-2.7077" curve="90.051271"/>
<vertex x="3.6982" y="-2.2605"/>
<vertex x="3.6982" y="-2.2657" curve="90.012964"/>
</polygon>
</package>
<package name="TACTILE-SWITCH-SMD">
<wire x1="-1.54" y1="-2.54" x2="-2.54" y2="-1.54" width="0.2032" layer="51"/>
<wire x1="-2.54" y1="-1.24" x2="-2.54" y2="1.27" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="1.54" x2="-1.54" y2="2.54" width="0.2032" layer="51"/>
<wire x1="-1.54" y1="2.54" x2="1.54" y2="2.54" width="0.2032" layer="21"/>
<wire x1="1.54" y1="2.54" x2="2.54" y2="1.54" width="0.2032" layer="51"/>
<wire x1="2.54" y1="1.24" x2="2.54" y2="-1.24" width="0.2032" layer="21"/>
<wire x1="2.54" y1="-1.54" x2="1.54" y2="-2.54" width="0.2032" layer="51"/>
<wire x1="1.54" y1="-2.54" x2="-1.54" y2="-2.54" width="0.2032" layer="21"/>
<wire x1="1.905" y1="1.27" x2="1.905" y2="0.445" width="0.127" layer="51"/>
<wire x1="1.905" y1="0.445" x2="2.16" y2="-0.01" width="0.127" layer="51"/>
<wire x1="1.905" y1="-0.23" x2="1.905" y2="-1.115" width="0.127" layer="51"/>
<circle x="0" y="0" radius="1.27" width="0.2032" layer="21"/>
<smd name="1" x="-2.794" y="1.905" dx="0.762" dy="1.524" layer="1" rot="R90"/>
<smd name="2" x="2.794" y="1.905" dx="0.762" dy="1.524" layer="1" rot="R90"/>
<smd name="3" x="-2.794" y="-1.905" dx="0.762" dy="1.524" layer="1" rot="R90"/>
<smd name="4" x="2.794" y="-1.905" dx="0.762" dy="1.524" layer="1" rot="R90"/>
<text x="-0.889" y="1.778" size="0.4064" layer="25">&gt;NAME</text>
<text x="-0.889" y="-2.032" size="0.4064" layer="27">&gt;Value</text>
</package>
<package name="TACTILE_SWITCH-SMD-RIGHT-ANGLE">
<hole x="0" y="0.9" drill="0.7"/>
<hole x="0" y="-0.9" drill="0.7"/>
<smd name="1" x="-1.95" y="0" dx="2" dy="1.1" layer="1" rot="R90"/>
<smd name="2" x="1.95" y="0" dx="2" dy="1.1" layer="1" rot="R90"/>
<wire x1="-2" y1="1.2" x2="-2" y2="1.5" width="0.127" layer="21"/>
<wire x1="-2" y1="1.5" x2="2" y2="1.5" width="0.127" layer="21"/>
<wire x1="2" y1="1.5" x2="2" y2="1.2" width="0.127" layer="21"/>
<wire x1="-2" y1="-1.2" x2="-2" y2="-1.5" width="0.127" layer="21"/>
<wire x1="-2" y1="-1.5" x2="-0.7" y2="-1.5" width="0.127" layer="21"/>
<wire x1="-0.7" y1="-1.5" x2="0.7" y2="-1.5" width="0.127" layer="21"/>
<wire x1="0.7" y1="-1.5" x2="2" y2="-1.5" width="0.127" layer="21"/>
<wire x1="2" y1="-1.5" x2="2" y2="-1.2" width="0.127" layer="21"/>
<wire x1="-0.7" y1="-2.1" x2="0.7" y2="-2.1" width="0.127" layer="21"/>
<wire x1="0.7" y1="-2.1" x2="0.7" y2="-1.5" width="0.127" layer="21"/>
<wire x1="-0.7" y1="-2.1" x2="-0.7" y2="-1.5" width="0.127" layer="21"/>
<text x="-2" y="1.7" size="0.8128" layer="25" ratio="15">&gt;NAME</text>
</package>
<package name="SWITCH-SPDT">
<wire x1="2.175" y1="5.815" x2="-2.175" y2="5.815" width="0.2032" layer="21"/>
<wire x1="-2.175" y1="5.815" x2="-2.175" y2="-5.815" width="0.2032" layer="21"/>
<wire x1="-2.175" y1="-5.815" x2="2.175" y2="-5.815" width="0.2032" layer="21"/>
<wire x1="2.175" y1="-5.815" x2="2.175" y2="5.815" width="0.2032" layer="21"/>
<pad name="1" x="0" y="2.54" drill="1.016" diameter="1.8796"/>
<pad name="2" x="0" y="0" drill="1.016" diameter="1.8796"/>
<pad name="3" x="0" y="-2.54" drill="1.016" diameter="1.8796"/>
<text x="-3.81" y="7.62" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-9.525" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="AYZ0202">
<description>&lt;b&gt;DPDT Slide Switch SMD&lt;/b&gt;
www.SparkFun.com SKU : Comp-SMDS</description>
<wire x1="-3.6" y1="1.75" x2="-3.6" y2="-1.75" width="0.2032" layer="21"/>
<wire x1="-3.6" y1="-1.75" x2="3.6" y2="-1.75" width="0.2032" layer="21"/>
<wire x1="3.6" y1="-1.75" x2="3.6" y2="1.75" width="0.2032" layer="21"/>
<wire x1="3.6" y1="1.75" x2="-3.6" y2="1.75" width="0.2032" layer="21"/>
<smd name="3" x="2.5" y="2.825" dx="1" dy="1.15" layer="1"/>
<smd name="2" x="0" y="2.825" dx="1" dy="1.15" layer="1"/>
<smd name="1" x="-2.5" y="2.825" dx="1" dy="1.15" layer="1"/>
<smd name="6" x="2.5" y="-2.825" dx="1" dy="1.15" layer="1"/>
<smd name="5" x="0" y="-2.825" dx="1" dy="1.15" layer="1"/>
<smd name="4" x="-2.5" y="-2.825" dx="1" dy="1.15" layer="1"/>
<text x="-2.54" y="1.143" size="0.4064" layer="25">&gt;Name</text>
<text x="0.508" y="1.143" size="0.4064" layer="27">&gt;Value</text>
<hole x="1.5" y="0" drill="0.85"/>
<hole x="-1.5" y="0" drill="0.85"/>
</package>
<package name="SWITCHE-DPDT">
<wire x1="8" y1="3.25" x2="-8" y2="3.25" width="0.127" layer="51"/>
<wire x1="-8" y1="3.25" x2="-8" y2="-3.25" width="0.127" layer="51"/>
<wire x1="-8" y1="-3.25" x2="8" y2="-3.25" width="0.127" layer="51"/>
<wire x1="8" y1="-3.25" x2="8" y2="3.25" width="0.127" layer="51"/>
<wire x1="-6" y1="3.25" x2="6" y2="3.25" width="0.2032" layer="21"/>
<wire x1="8" y1="1" x2="8" y2="-1" width="0.2032" layer="21"/>
<wire x1="6" y1="-3.25" x2="-6" y2="-3.25" width="0.2032" layer="21"/>
<wire x1="-8" y1="-1" x2="-8" y2="1" width="0.2032" layer="21"/>
<pad name="P$1" x="-7.5" y="3" drill="1.5" diameter="2.54"/>
<pad name="P$2" x="-7.5" y="-3" drill="1.5" diameter="2.54"/>
<pad name="P$3" x="7.5" y="3" drill="1.5" diameter="2.54"/>
<pad name="P$4" x="7.5" y="-3" drill="1.5" diameter="2.54"/>
<pad name="1" x="-4" y="1.25" drill="0.7" diameter="1.6764"/>
<pad name="2" x="0" y="1.25" drill="0.7" diameter="1.6764"/>
<pad name="3" x="4" y="1.25" drill="0.7" diameter="1.6764"/>
<pad name="4" x="-4" y="-1.25" drill="0.7" diameter="1.6764"/>
<pad name="5" x="0" y="-1.25" drill="0.7" diameter="1.6764"/>
<pad name="6" x="4" y="-1.25" drill="0.7" diameter="1.6764"/>
</package>
<package name="R_SW_TH">
<wire x1="-1.651" y1="19.2532" x2="-1.651" y2="-1.3716" width="0.2032" layer="21"/>
<wire x1="-1.651" y1="-1.3716" x2="-1.651" y2="-2.2352" width="0.2032" layer="21"/>
<wire x1="-1.651" y1="19.2532" x2="13.589" y2="19.2532" width="0.2032" layer="21"/>
<wire x1="13.589" y1="19.2532" x2="13.589" y2="-2.2352" width="0.2032" layer="21"/>
<wire x1="13.589" y1="-2.2352" x2="-1.651" y2="-2.2352" width="0.2032" layer="21"/>
<pad name="P$1" x="0" y="0" drill="1.6002"/>
<pad name="P$2" x="0" y="16.9926" drill="1.6002"/>
<pad name="P$3" x="12.0904" y="15.494" drill="1.6002"/>
<pad name="P$4" x="12.0904" y="8.4582" drill="1.6002"/>
</package>
<package name="SWITCH-SPDT-SMD">
<wire x1="-4.5" y1="1.75" x2="-4.5" y2="-1.75" width="0.127" layer="51"/>
<wire x1="-4.5" y1="-1.75" x2="4.5" y2="-1.75" width="0.127" layer="51"/>
<wire x1="4.5" y1="-1.75" x2="4.5" y2="1.75" width="0.127" layer="51"/>
<wire x1="4.5" y1="1.75" x2="2" y2="1.75" width="0.127" layer="51"/>
<wire x1="2" y1="1.75" x2="0.5" y2="1.75" width="0.127" layer="51"/>
<wire x1="0.5" y1="1.75" x2="-4.5" y2="1.75" width="0.127" layer="51"/>
<wire x1="0.5" y1="1.75" x2="0.5" y2="3.75" width="0.127" layer="51"/>
<wire x1="0.5" y1="3.75" x2="2" y2="3.75" width="0.127" layer="51"/>
<wire x1="2" y1="3.75" x2="2" y2="1.75" width="0.127" layer="51"/>
<wire x1="-4" y1="-1.75" x2="-4.5" y2="-1.75" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="-1.75" x2="-4.5" y2="1.75" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="1.75" x2="4.5" y2="1.75" width="0.2032" layer="21"/>
<wire x1="4.5" y1="1.75" x2="4.5" y2="-1.75" width="0.2032" layer="21"/>
<wire x1="4.5" y1="-1.75" x2="4" y2="-1.75" width="0.2032" layer="21"/>
<smd name="1" x="-2.5" y="-2.75" dx="1.2" dy="2.5" layer="1" rot="R180"/>
<smd name="2" x="0" y="-2.75" dx="1.2" dy="2.5" layer="1" rot="R180"/>
<smd name="3" x="2.5" y="-2.75" dx="1.2" dy="2.5" layer="1" rot="R180"/>
<text x="-1.27" y="0.635" size="0.6096" layer="25">&gt;Name</text>
<text x="-1.27" y="-1.27" size="0.6096" layer="27">&gt;Value</text>
<hole x="-3.55" y="0" drill="0.9"/>
<hole x="3.55" y="0" drill="0.9"/>
</package>
<package name="SWITCH-SPDT_LOCK.007S">
<wire x1="2.175" y1="5.815" x2="-2.175" y2="5.815" width="0.2032" layer="21"/>
<wire x1="-2.175" y1="5.815" x2="-2.175" y2="-5.815" width="0.2032" layer="21"/>
<wire x1="-2.175" y1="-5.815" x2="2.175" y2="-5.815" width="0.2032" layer="21"/>
<wire x1="2.175" y1="-5.815" x2="2.175" y2="5.815" width="0.2032" layer="21"/>
<pad name="1" x="0" y="2.7178" drill="1.016" diameter="1.8796"/>
<pad name="2" x="0" y="0" drill="1.016" diameter="1.8796"/>
<pad name="3" x="0" y="-2.7178" drill="1.016" diameter="1.8796"/>
<text x="-3.81" y="7.62" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-9.525" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-0.2286" y1="-0.3048" x2="0.2286" y2="0.3048" layer="51"/>
<rectangle x1="-0.2286" y1="2.2352" x2="0.2286" y2="2.8448" layer="51"/>
<rectangle x1="-0.2286" y1="-2.8448" x2="0.2286" y2="-2.2352" layer="51"/>
</package>
<package name="SWITCH-SPDT_KIT">
<description>&lt;h3&gt;SWITCH-SPDT_KIT&lt;/h3&gt;
Through-hole SPDT Switch&lt;br&gt;
&lt;br&gt;
&lt;b&gt;Warning:&lt;/b&gt; This is the KIT version of this package. This package has a smaller diameter top stop mask, which doesn't cover the diameter of the pad. This means only the bottom side of the pads' copper will be exposed. You'll only be able to solder to the bottom side.</description>
<wire x1="2.175" y1="5.815" x2="-2.175" y2="5.815" width="0.2032" layer="21"/>
<wire x1="-2.175" y1="5.815" x2="-2.175" y2="-5.815" width="0.2032" layer="21"/>
<wire x1="-2.175" y1="-5.815" x2="2.175" y2="-5.815" width="0.2032" layer="21"/>
<wire x1="2.175" y1="-5.815" x2="2.175" y2="5.815" width="0.2032" layer="21"/>
<pad name="1" x="0" y="2.7178" drill="1.016" diameter="1.8796" stop="no"/>
<pad name="2" x="0" y="0" drill="1.016" diameter="1.8796" stop="no"/>
<pad name="3" x="0" y="-2.7178" drill="1.016" diameter="1.8796" stop="no"/>
<text x="-3.81" y="7.62" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-9.525" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-0.2286" y1="-0.3048" x2="0.2286" y2="0.3048" layer="51"/>
<rectangle x1="-0.2286" y1="2.2352" x2="0.2286" y2="2.8448" layer="51"/>
<rectangle x1="-0.2286" y1="-2.8448" x2="0.2286" y2="-2.2352" layer="51"/>
<polygon width="0.127" layer="30">
<vertex x="-0.0178" y="1.8414" curve="-90.039946"/>
<vertex x="-0.8787" y="2.6975" curve="-90"/>
<vertex x="-0.0026" y="3.5916" curve="-90.006409"/>
<vertex x="0.8738" y="2.6975" curve="-90.03214"/>
</polygon>
<polygon width="0.127" layer="30">
<vertex x="-0.0051" y="-3.5967" curve="-90.006558"/>
<vertex x="-0.8788" y="-2.7431" curve="-90.037923"/>
<vertex x="0.0128" y="-1.8363" curve="-90.006318"/>
<vertex x="0.8814" y="-2.7432" curve="-90.038792"/>
</polygon>
<polygon width="0.127" layer="30">
<vertex x="-0.0102" y="-0.8738" curve="-90.019852"/>
<vertex x="-0.8762" y="-0.0203" curve="-90.019119"/>
<vertex x="0.0153" y="0.8789" curve="-90"/>
<vertex x="0.8739" y="-0.0077" curve="-90.038897"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="0" y="2.2758" curve="-90.012891"/>
<vertex x="-0.4445" y="2.7" curve="-90"/>
<vertex x="0" y="3.1673" curve="-90"/>
<vertex x="0.4419" y="2.7102" curve="-90.012967"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="0.0026" y="-3.1648" curve="-90.012891"/>
<vertex x="-0.4419" y="-2.7406" curve="-90"/>
<vertex x="0.0026" y="-2.2733" curve="-90"/>
<vertex x="0.4445" y="-2.7304" curve="-90.012967"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="0.0102" y="-0.4471" curve="-90.012891"/>
<vertex x="-0.4343" y="-0.0229" curve="-90"/>
<vertex x="0.0102" y="0.4444" curve="-90"/>
<vertex x="0.4521" y="-0.0127" curve="-90.012967"/>
</polygon>
</package>
<package name="SWITCH-SPST-SMD-A">
<wire x1="-3.35" y1="1.3" x2="-3.35" y2="-1.3" width="0.127" layer="51"/>
<wire x1="-3.35" y1="-1.3" x2="3.35" y2="-1.3" width="0.127" layer="51"/>
<wire x1="3.35" y1="-1.3" x2="3.35" y2="1.3" width="0.127" layer="51"/>
<wire x1="3.35" y1="1.3" x2="-0.1" y2="1.3" width="0.127" layer="51"/>
<wire x1="-0.1" y1="1.3" x2="-1.4" y2="1.3" width="0.127" layer="51"/>
<wire x1="-1.4" y1="1.3" x2="-3.35" y2="1.3" width="0.127" layer="51"/>
<wire x1="-0.1" y1="1.3" x2="-0.1" y2="2.8" width="0.127" layer="51"/>
<wire x1="-0.1" y1="2.8" x2="-1.4" y2="2.8" width="0.127" layer="51"/>
<wire x1="-1.4" y1="2.8" x2="-1.4" y2="1.3" width="0.127" layer="51"/>
<wire x1="-3.35" y1="0.3" x2="-3.35" y2="-0.3" width="0.2032" layer="21"/>
<wire x1="3.35" y1="0.3" x2="3.35" y2="-0.3" width="0.2032" layer="21"/>
<wire x1="2.7" y1="1.3" x2="-2.7" y2="1.3" width="0.2032" layer="21"/>
<wire x1="1.5" y1="-1.3" x2="0" y2="-1.3" width="0.2032" layer="21"/>
<smd name="1" x="-2.25" y="-1.75" dx="0.7" dy="1.5" layer="1" rot="R180"/>
<smd name="2" x="-0.75" y="-1.75" dx="0.7" dy="1.5" layer="1" rot="R180"/>
<smd name="3" x="2.25" y="-1.75" dx="0.7" dy="1.5" layer="1" rot="R180"/>
<smd name="GND1" x="-3.65" y="1" dx="1" dy="0.6" layer="1"/>
<smd name="GND2" x="-3.65" y="-1.1" dx="1" dy="0.8" layer="1"/>
<smd name="GND3" x="3.65" y="1" dx="1" dy="0.6" layer="1"/>
<smd name="GND4" x="3.65" y="-1.1" dx="1" dy="0.8" layer="1"/>
<text x="-1.27" y="0.635" size="0.6096" layer="25">&gt;Name</text>
<text x="-1.27" y="-1.27" size="0.6096" layer="27">&gt;Value</text>
<hole x="-1.5" y="0" drill="0.9"/>
<hole x="1.5" y="0" drill="0.9"/>
</package>
<package name="MSS22">
<description>MSS22 DPDT Switch</description>
<wire x1="-0.25" y1="-0.5" x2="5.55" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-0.25" y1="13.5" x2="5.55" y2="13.5" width="0.127" layer="51"/>
<wire x1="-0.25" y1="13.5" x2="-0.25" y2="-0.5" width="0.127" layer="51"/>
<wire x1="5.55" y1="13.5" x2="5.55" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-0.3" y1="11.7" x2="-0.3" y2="1.3" width="0.127" layer="21"/>
<wire x1="5.6" y1="11.7" x2="5.6" y2="1.3" width="0.127" layer="21"/>
<wire x1="1.2" y1="-0.6" x2="4.1" y2="-0.6" width="0.127" layer="21"/>
<wire x1="1.2" y1="13.6" x2="4.1" y2="13.6" width="0.127" layer="21"/>
<pad name="NC@3" x="0" y="0" drill="1.35"/>
<pad name="NC@1" x="0" y="13" drill="1.35"/>
<pad name="NC@2" x="5.3" y="13" drill="1.35"/>
<pad name="NC@4" x="5.3" y="0" drill="1.35"/>
<pad name="6" x="2.65" y="1.5" drill="0.95"/>
<pad name="5" x="2.65" y="3.5" drill="0.95"/>
<pad name="4" x="2.65" y="5.5" drill="0.95"/>
<pad name="3" x="2.65" y="7.5" drill="0.95"/>
<pad name="2" x="2.65" y="9.5" drill="0.95"/>
<pad name="1" x="2.65" y="11.5" drill="0.95"/>
<text x="1" y="2" size="0.4064" layer="25" rot="R90">&gt;NAME</text>
<text x="5" y="2" size="0.4064" layer="27" rot="R90">&gt;VALUE</text>
</package>
<package name="MS">
<description>Series S Slide Switch</description>
<wire x1="-6.731" y1="11.4935" x2="6.731" y2="11.4935" width="0.2032" layer="51"/>
<wire x1="6.731" y1="11.4935" x2="6.731" y2="-11.4935" width="0.2032" layer="51"/>
<wire x1="6.731" y1="-11.4935" x2="-6.731" y2="-11.4935" width="0.2032" layer="51"/>
<wire x1="-6.731" y1="-11.4935" x2="-6.731" y2="11.4935" width="0.2032" layer="51"/>
<wire x1="-6.858" y1="11.684" x2="-6.858" y2="-11.684" width="0.2032" layer="21"/>
<wire x1="-6.858" y1="-11.684" x2="6.858" y2="-11.684" width="0.2032" layer="21"/>
<wire x1="6.858" y1="-11.684" x2="6.858" y2="11.684" width="0.2032" layer="21"/>
<wire x1="6.858" y1="11.684" x2="-6.858" y2="11.684" width="0.2032" layer="21"/>
<pad name="5" x="3.81" y="0" drill="2.8" diameter="3.302"/>
<pad name="2" x="-3.81" y="0" drill="2.8" diameter="3.302"/>
<pad name="3" x="-3.81" y="6.35" drill="2.8" diameter="3.302"/>
<pad name="1" x="-3.81" y="-6.35" drill="2.8" diameter="3.302"/>
<pad name="4" x="3.81" y="-6.35" drill="2.8" diameter="3.302"/>
<pad name="6" x="3.81" y="6.35" drill="2.8" diameter="3.302"/>
</package>
<package name="ES">
<description>Right Angle Slide Switch. Works with part#ES21MABE</description>
<wire x1="5.0165" y1="12.1094" x2="5.0165" y2="2.3686" width="0.2032" layer="51"/>
<wire x1="5.0165" y1="2.3686" x2="-5.0165" y2="2.3686" width="0.2032" layer="51"/>
<wire x1="-5.0165" y1="2.3686" x2="-5.0165" y2="12.1094" width="0.2032" layer="51"/>
<wire x1="-5.0165" y1="12.1094" x2="5.0165" y2="12.1094" width="0.2032" layer="51"/>
<wire x1="-4.318" y1="2.286" x2="-5.08" y2="2.286" width="0.2032" layer="21"/>
<wire x1="-5.08" y1="2.286" x2="-5.08" y2="3.302" width="0.2032" layer="21"/>
<wire x1="4.318" y1="2.286" x2="5.08" y2="2.286" width="0.2032" layer="21"/>
<wire x1="5.08" y1="2.286" x2="5.08" y2="3.302" width="0.2032" layer="21"/>
<wire x1="5.08" y1="10.668" x2="5.08" y2="11.938" width="0.2032" layer="21"/>
<wire x1="-5.08" y1="10.668" x2="-5.08" y2="11.938" width="0.2032" layer="21"/>
<pad name="5" x="0" y="0" drill="0.8636" diameter="1.27"/>
<pad name="6" x="-2.54" y="0" drill="0.8636" diameter="1.27"/>
<pad name="4" x="2.54" y="0" drill="0.8636" diameter="1.27"/>
<pad name="1" x="2.54" y="2.54" drill="0.8636" diameter="1.27"/>
<pad name="2" x="0" y="2.54" drill="0.8636" diameter="1.27"/>
<pad name="3" x="-2.54" y="2.54" drill="0.8636" diameter="1.27"/>
<pad name="NC@1" x="-2.54" y="7.62" drill="0.9" diameter="1.6764"/>
<pad name="NC@2" x="2.54" y="7.62" drill="0.9" diameter="1.6764"/>
<text x="-3.81" y="12.7" size="0.4064" layer="25">&gt;NAME</text>
<text x="-3.81" y="10.16" size="0.4064" layer="27">&gt;VALUE</text>
</package>
<package name="OS">
<wire x1="4.3053" y1="3.4036" x2="4.3053" y2="-3.4036" width="0.2032" layer="51"/>
<wire x1="4.3053" y1="-3.4036" x2="-4.3053" y2="-3.4036" width="0.2032" layer="51"/>
<wire x1="-4.3053" y1="-3.4036" x2="-4.3053" y2="3.4036" width="0.2032" layer="51"/>
<wire x1="-4.3053" y1="3.4036" x2="4.3053" y2="3.4036" width="0.2032" layer="51"/>
<wire x1="-4.445" y1="3.175" x2="-4.445" y2="-3.81" width="0.2032" layer="21"/>
<wire x1="-4.445" y1="-3.81" x2="4.445" y2="-3.81" width="0.2032" layer="21"/>
<wire x1="4.445" y1="-3.81" x2="4.445" y2="3.175" width="0.2032" layer="21"/>
<pad name="2" x="0" y="1.2446" drill="0.7874" diameter="1.27"/>
<pad name="5" x="0" y="-1.2446" drill="0.7874" diameter="1.27"/>
<pad name="6" x="2.0066" y="-1.2446" drill="0.7874" diameter="1.27"/>
<pad name="4" x="-2.0066" y="-1.2446" drill="0.7874" diameter="1.27"/>
<pad name="1" x="-2.0066" y="1.2446" drill="0.7874" diameter="1.27"/>
<pad name="3" x="2.0066" y="1.2446" drill="0.7874" diameter="1.27"/>
<text x="-3.81" y="3.81" size="0.4064" layer="25">&gt;NAME</text>
<text x="-1.905" y="-4.445" size="0.4064" layer="27" rot="R180">&gt;VALUE</text>
</package>
<package name="EG2211">
<wire x1="-3" y1="-5" x2="3" y2="-5" width="0.127" layer="51"/>
<wire x1="3" y1="-5" x2="3" y2="5" width="0.127" layer="51"/>
<wire x1="3" y1="5" x2="-3" y2="5" width="0.127" layer="51"/>
<wire x1="-3" y1="5" x2="-3" y2="2.5" width="0.127" layer="51"/>
<wire x1="-3" y1="2.5" x2="-3" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-3" y1="-1" x2="-3" y2="-2" width="0.127" layer="51"/>
<wire x1="-3" y1="-2" x2="-3" y2="-2.5" width="0.127" layer="51"/>
<wire x1="-3" y1="-2.5" x2="-3" y2="-5" width="0.127" layer="51"/>
<wire x1="-3" y1="2.5" x2="-7" y2="2.5" width="0.127" layer="51"/>
<wire x1="-7" y1="2.5" x2="-7" y2="2" width="0.127" layer="51"/>
<wire x1="-7" y1="0" x2="-7" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-7" y1="-0.5" x2="-6.5" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-6.5" y1="-0.5" x2="-5.5" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-5.5" y1="-0.5" x2="-4.5" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-4.5" y1="-0.5" x2="-3" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-7" y1="2" x2="-6.5" y2="1.5" width="0.127" layer="51"/>
<wire x1="-6.5" y1="1.5" x2="-7" y2="1" width="0.127" layer="51"/>
<wire x1="-7" y1="1" x2="-6.5" y2="0.5" width="0.127" layer="51"/>
<wire x1="-6.5" y1="0.5" x2="-7" y2="0" width="0.127" layer="51"/>
<wire x1="-3" y1="-2.5" x2="-3.5" y2="-2.5" width="0.127" layer="51" style="dashdot"/>
<wire x1="-3.5" y1="-2.5" x2="-4.5" y2="-2.5" width="0.127" layer="51" style="dashdot"/>
<wire x1="-4.5" y1="-2.5" x2="-5.5" y2="-2.5" width="0.127" layer="51" style="dashdot"/>
<wire x1="-5.5" y1="-2.5" x2="-6.5" y2="-2.5" width="0.127" layer="51" style="dashdot"/>
<wire x1="-6.5" y1="-2.5" x2="-7" y2="-2.5" width="0.127" layer="51" style="dashdot"/>
<wire x1="-7" y1="-2.5" x2="-7" y2="-2" width="0.127" layer="51"/>
<wire x1="-7" y1="-2" x2="-7" y2="-1" width="0.127" layer="51"/>
<wire x1="-7" y1="-1" x2="-7" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-3" y1="-0.5" x2="-3" y2="-1" width="0.127" layer="51"/>
<wire x1="-3" y1="-1" x2="-3" y2="-1.5" width="0.127" layer="51"/>
<wire x1="-3" y1="-1" x2="-4.5" y2="-2.5" width="0.127" layer="51"/>
<wire x1="-3" y1="-0.5" x2="-3.5" y2="-0.5" width="0.127" layer="51"/>
<wire x1="-3.5" y1="-0.5" x2="-5.5" y2="-2.5" width="0.127" layer="51"/>
<wire x1="-4.5" y1="-0.5" x2="-6.5" y2="-2.5" width="0.127" layer="51"/>
<wire x1="-5.5" y1="-0.5" x2="-7" y2="-2" width="0.127" layer="51"/>
<wire x1="-6.5" y1="-0.5" x2="-7" y2="-1" width="0.127" layer="51"/>
<wire x1="-3" y1="-2" x2="-3.5" y2="-2.5" width="0.127" layer="51"/>
<pad name="P1-TB" x="-1" y="-2" drill="0.8"/>
<pad name="P2-TB" x="1" y="-2" drill="0.8"/>
<pad name="P1" x="-1" y="0" drill="0.8"/>
<pad name="P2" x="1" y="0" drill="0.8"/>
<pad name="P1-TA" x="-1" y="2" drill="0.8"/>
<pad name="P2-TA" x="1" y="2" drill="0.8"/>
<pad name="P$7" x="0" y="4.6" drill="1.2"/>
<pad name="P$8" x="0" y="-4.6" drill="1.2"/>
</package>
<package name="GPI-152-3013">
<description>Switch with part number: GPI-152-3013</description>
<wire x1="12.7" y1="5.13" x2="-12.7" y2="5.13" width="0.127" layer="51"/>
<wire x1="-12.7" y1="5.13" x2="-12.7" y2="-5.13" width="0.127" layer="51"/>
<wire x1="-12.7" y1="-5.13" x2="12.7" y2="-5.13" width="0.127" layer="51"/>
<wire x1="12.7" y1="-5.13" x2="12.7" y2="5.13" width="0.127" layer="51"/>
<wire x1="-9.5" y1="5.4" x2="9.5" y2="5.4" width="0.3048" layer="21"/>
<wire x1="-9.4" y1="-5.4" x2="9.6" y2="-5.4" width="0.3048" layer="21"/>
<wire x1="-13" y1="5" x2="-13" y2="-5" width="0.3048" layer="21"/>
<wire x1="13" y1="5" x2="13" y2="-5" width="0.3048" layer="21"/>
<wire x1="2" y1="5.2" x2="1.9" y2="5.2" width="0.127" layer="51"/>
<wire x1="1.9" y1="5.2" x2="1.9" y2="9.1" width="0.127" layer="51"/>
<wire x1="1.9" y1="9.1" x2="7" y2="9.1" width="0.127" layer="51"/>
<wire x1="7" y1="9.1" x2="7" y2="5.2" width="0.127" layer="51"/>
<circle x="10.31" y="0" radius="0.9518" width="0.127" layer="51"/>
<circle x="-10.69" y="0" radius="0.9518" width="0.127" layer="51"/>
<pad name="C@1" x="0" y="3.455" drill="1.65"/>
<pad name="C@2" x="0" y="-3.455" drill="1.65"/>
<pad name="21" x="4.6" y="-3.455" drill="1.65"/>
<pad name="11" x="-4.6" y="-3.455" drill="1.65"/>
<pad name="10" x="-4.6" y="3.455" drill="1.65"/>
<pad name="20" x="4.6" y="3.455" drill="1.65"/>
<pad name="P$7" x="11.43" y="4.85" drill="1.65"/>
<pad name="P$8" x="-11.43" y="4.85" drill="1.65"/>
<pad name="P$9" x="-11.43" y="-4.85" drill="1.65"/>
<pad name="P$10" x="11.43" y="-4.85" drill="1.65"/>
</package>
<package name="STOMP-SWITCH-DPDT">
<description>Stomp switch

COM-11151

Footprint unproven</description>
<wire x1="-8.6" y1="6.1" x2="-8.6" y2="-6.1" width="0.127" layer="21"/>
<wire x1="-8.6" y1="-6.1" x2="8.6" y2="-6.1" width="0.127" layer="21"/>
<wire x1="8.6" y1="-6.1" x2="8.6" y2="6.1" width="0.127" layer="21"/>
<wire x1="8.6" y1="6.1" x2="-8.6" y2="6.1" width="0.127" layer="21"/>
<pad name="P$1" x="-7.1" y="4.375" drill="1.5"/>
<pad name="P$2" x="0" y="4.375" drill="1.5"/>
<pad name="P$3" x="7.1" y="4.375" drill="1.5"/>
<pad name="P$4" x="-7.1" y="-4.375" drill="1.5"/>
<pad name="P$5" x="0" y="-4.375" drill="1.5"/>
<pad name="P$6" x="7.1" y="-4.375" drill="1.5"/>
<circle x="0" y="0" radius="6" width="0.127" layer="21"/>
<wire x1="-8.65" y1="7.25" x2="8.65" y2="7.25" width="0.127" layer="51"/>
<wire x1="8.65" y1="7.25" x2="8.65" y2="-7.25" width="0.127" layer="51"/>
<wire x1="8.65" y1="-7.25" x2="-8.65" y2="-7.25" width="0.127" layer="51"/>
<wire x1="-8.65" y1="-7.25" x2="-8.65" y2="7.25" width="0.127" layer="51"/>
<text x="-3.81" y="0.635" size="1.27" layer="25">&gt;NAME</text>
<text x="-3.81" y="-1.905" size="1.27" layer="27">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="SWITCH-MOMENTARY-2">
<wire x1="1.905" y1="0" x2="2.54" y2="0" width="0.254" layer="94"/>
<wire x1="-2.54" y1="0" x2="1.905" y2="1.27" width="0.254" layer="94"/>
<circle x="-2.54" y="0" radius="0.127" width="0.4064" layer="94"/>
<circle x="2.54" y="0" radius="0.127" width="0.4064" layer="94"/>
<text x="-2.54" y="2.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="2"/>
<pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="TOGGLE">
<wire x1="0" y1="0" x2="2.54" y2="1.27" width="0.254" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="3.175" y2="-2.54" width="0.127" layer="94"/>
<wire x1="2.54" y1="2.54" x2="3.175" y2="2.54" width="0.1524" layer="94"/>
<circle x="2.54" y="2.54" radius="0.3592" width="0.2032" layer="94"/>
<circle x="2.54" y="-2.54" radius="0.3592" width="0.2032" layer="94"/>
<circle x="0" y="0" radius="0.3592" width="0.2032" layer="94"/>
<text x="-1.905" y="-6.35" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="3.81" size="1.778" layer="96">&gt;VALUE</text>
<pin name="P" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
<pin name="S" x="5.08" y="-2.54" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="O" x="5.08" y="2.54" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="AYZ0202">
<wire x1="1.27" y1="5.08" x2="-2.286" y2="2.794" width="0.254" layer="94"/>
<wire x1="-2.286" y1="-4.826" x2="1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="2.54" y1="6.35" x2="2.54" y2="7.62" width="0.254" layer="94"/>
<wire x1="2.54" y1="7.62" x2="-2.54" y2="7.62" width="0.254" layer="94"/>
<wire x1="-2.54" y1="7.62" x2="-2.54" y2="3.81" width="0.254" layer="94"/>
<wire x1="-2.54" y1="1.27" x2="-2.54" y2="-3.81" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-6.35" x2="-2.54" y2="-10.16" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-10.16" x2="2.54" y2="-10.16" width="0.254" layer="94"/>
<wire x1="2.54" y1="-10.16" x2="2.54" y2="-8.89" width="0.254" layer="94"/>
<wire x1="2.54" y1="-6.35" x2="2.54" y2="-3.81" width="0.254" layer="94"/>
<wire x1="2.54" y1="3.81" x2="2.54" y2="1.27" width="0.254" layer="94"/>
<wire x1="0" y1="4.064" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="1.27" x2="0" y2="-1.27" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="-3.302" width="0.254" layer="94"/>
<circle x="-2.54" y="2.54" radius="0.3592" width="0.254" layer="94"/>
<circle x="2.54" y="5.08" radius="0.3592" width="0.254" layer="94"/>
<circle x="2.54" y="0" radius="0.3592" width="0.254" layer="94"/>
<circle x="2.54" y="-2.54" radius="0.3592" width="0.254" layer="94"/>
<circle x="2.54" y="-7.62" radius="0.3592" width="0.254" layer="94"/>
<circle x="-2.54" y="-5.08" radius="0.3592" width="0.254" layer="94"/>
<text x="-2.54" y="8.128" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-12.7" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="5.08" y="5.08" visible="off" length="short" rot="R180"/>
<pin name="2" x="-5.08" y="2.54" visible="off" length="short"/>
<pin name="3" x="5.08" y="0" visible="off" length="short" rot="R180"/>
<pin name="4" x="5.08" y="-2.54" visible="off" length="short" rot="R180"/>
<pin name="5" x="-5.08" y="-5.08" visible="off" length="short"/>
<pin name="6" x="5.08" y="-7.62" visible="off" length="short" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SWITCH-MOMENTARY-2" prefix="S">
<description>Various NO switches- pushbuttons, reed, etc</description>
<gates>
<gate name="G$1" symbol="SWITCH-MOMENTARY-2" x="0" y="0"/>
</gates>
<devices>
<device name="PTH" package="TACTILE-PTH">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value=" SWCH-08441" constant="no"/>
</technology>
</technologies>
</device>
<device name="SMD-2" package="TACTILE_SWITCH_SMD-2">
<connects>
<connect gate="G$1" pin="1" pad="2"/>
<connect gate="G$1" pin="2" pad="1"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-09213"/>
</technology>
</technologies>
</device>
<device name="12MM" package="TACTILE-PTH-12MM">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-09185" constant="no"/>
</technology>
</technologies>
</device>
<device name="SMD-1101NE" package="TACTILE-SWITCH-1101NE">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-00815" constant="no"/>
</technology>
</technologies>
</device>
<device name="PTH_REED" package="REED_SWITCH_PTH">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-4" package="TACTILE_SWITCH_TALL">
<connects>
<connect gate="G$1" pin="1" pad="A2"/>
<connect gate="G$1" pin="2" pad="B2"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-11966" constant="no"/>
</technology>
</technologies>
</device>
<device name="PTH_REED2" package="REED_SWITCH_PLASTIC">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-10467" constant="no"/>
</technology>
</technologies>
</device>
<device name="SIDE_EZ" package="TACTILE-PTH-SIDEEZ">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-3" package="TACTILE_SWITCH_SMD-3">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-12MM" package="TACTILE-SMD-12MM">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH_EZ" package="TACTILE-PTH-EZ">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD" package="TACTILE-SWITCH-SMD">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-08247" constant="no"/>
</technology>
</technologies>
</device>
<device name="SMD-REDUNDANT" package="TACTILE-SWITCH-SMD">
<connects>
<connect gate="G$1" pin="1" pad="1 2"/>
<connect gate="G$1" pin="2" pad="3 4"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-08247" constant="no"/>
</technology>
</technologies>
</device>
<device name="TACTILE-SWITCH-SMD-RIGHT-ANGLE" package="TACTILE_SWITCH-SMD-RIGHT-ANGLE">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="COMP-12265" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SWITCH-SPDT" prefix="S" uservalue="yes">
<description>&lt;b&gt;SPDT Switch&lt;/b&gt;&lt;br&gt;
Simple slide switch, Spark Fun Electronics SKU : COM-00102&lt;br&gt;
DPDT SMT slide switch, AYZ0202, SWCH-08179</description>
<gates>
<gate name="1" symbol="TOGGLE" x="-2.54" y="0"/>
</gates>
<devices>
<device name="PTH" package="SWITCH-SPDT">
<connects>
<connect gate="1" pin="O" pad="1"/>
<connect gate="1" pin="P" pad="2"/>
<connect gate="1" pin="S" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-08261"/>
</technology>
</technologies>
</device>
<device name="SMD" package="AYZ0202">
<connects>
<connect gate="1" pin="O" pad="1"/>
<connect gate="1" pin="P" pad="2"/>
<connect gate="1" pin="S" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-08179" constant="no"/>
<attribute name="SF_ID" value="COM-00597" constant="no"/>
</technology>
</technologies>
</device>
<device name="PTH2" package="SWITCHE-DPDT">
<connects>
<connect gate="1" pin="O" pad="1"/>
<connect gate="1" pin="P" pad="2"/>
<connect gate="1" pin="S" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH3" package="R_SW_TH">
<connects>
<connect gate="1" pin="O" pad="P$1"/>
<connect gate="1" pin="P" pad="P$2"/>
<connect gate="1" pin="S" pad="P$3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD2" package="SWITCH-SPDT-SMD">
<connects>
<connect gate="1" pin="O" pad="1"/>
<connect gate="1" pin="P" pad="2"/>
<connect gate="1" pin="S" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH_LOCK" package="SWITCH-SPDT_LOCK.007S">
<connects>
<connect gate="1" pin="O" pad="1"/>
<connect gate="1" pin="P" pad="2"/>
<connect gate="1" pin="S" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="KIT" package="SWITCH-SPDT_KIT">
<connects>
<connect gate="1" pin="O" pad="1"/>
<connect gate="1" pin="P" pad="2"/>
<connect gate="1" pin="S" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-08261" constant="no"/>
</technology>
</technologies>
</device>
<device name="-SMD-A" package="SWITCH-SPST-SMD-A">
<connects>
<connect gate="1" pin="O" pad="1"/>
<connect gate="1" pin="P" pad="2"/>
<connect gate="1" pin="S" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SWITCH-DPDT" prefix="S" uservalue="yes">
<description>DPDT Version of the COM-00597</description>
<gates>
<gate name="G$1" symbol="AYZ0202" x="0" y="0"/>
</gates>
<devices>
<device name="AYZ0202" package="AYZ0202">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="SWCH-08179"/>
<attribute name="VALUE" value="DPDT" constant="no"/>
</technology>
</technologies>
</device>
<device name="PTH" package="MSS22">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH1" package="MS">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="ES" package="ES">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="OS" package="OS">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="EG2211" package="EG2211">
<connects>
<connect gate="G$1" pin="1" pad="P1-TA"/>
<connect gate="G$1" pin="2" pad="P1"/>
<connect gate="G$1" pin="3" pad="P1-TB"/>
<connect gate="G$1" pin="4" pad="P2-TA"/>
<connect gate="G$1" pin="5" pad="P2"/>
<connect gate="G$1" pin="6" pad="P2-TB"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="GPI" package="GPI-152-3013">
<connects>
<connect gate="G$1" pin="1" pad="10"/>
<connect gate="G$1" pin="2" pad="C@1"/>
<connect gate="G$1" pin="3" pad="20"/>
<connect gate="G$1" pin="4" pad="11"/>
<connect gate="G$1" pin="5" pad="C@2"/>
<connect gate="G$1" pin="6" pad="21"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="STOMP-PTH" package="STOMP-SWITCH-DPDT">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
<connect gate="G$1" pin="3" pad="P$3"/>
<connect gate="G$1" pin="4" pad="P$4"/>
<connect gate="G$1" pin="5" pad="P$5"/>
<connect gate="G$1" pin="6" pad="P$6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-LED">
<description>&lt;h3&gt;SparkFun Electronics' preferred foot prints&lt;/h3&gt;
In this library you'll find discrete LEDs for illumination or indication, but no displays.&lt;br&gt;&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is the end user's responsibility to ensure correctness and suitablity for a given componet or application. If you enjoy using this library, please buy one of our products at www.sparkfun.com.
&lt;br&gt;&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="LED-0603">
<wire x1="0.46" y1="0.17" x2="0" y2="0.17" width="0.2032" layer="21"/>
<wire x1="-0.46" y1="0.17" x2="0" y2="0.17" width="0.2032" layer="21"/>
<wire x1="0" y1="0.17" x2="0.2338" y2="-0.14" width="0.2032" layer="21"/>
<wire x1="-0.0254" y1="0.1546" x2="-0.2184" y2="-0.14" width="0.2032" layer="21"/>
<smd name="C" x="0" y="0.877" dx="1" dy="1" layer="1" roundness="30"/>
<smd name="A" x="0" y="-0.877" dx="1" dy="1" layer="1" roundness="30"/>
<text x="-0.6985" y="-0.889" size="0.4064" layer="25" rot="R90">&gt;NAME</text>
<text x="1.0795" y="-1.016" size="0.4064" layer="27" rot="R90">&gt;VALUE</text>
</package>
<package name="LED-1206">
<wire x1="-1" y1="1" x2="-2.4" y2="1" width="0.2032" layer="21"/>
<wire x1="-2.4" y1="1" x2="-2.4" y2="-1" width="0.2032" layer="21"/>
<wire x1="-2.4" y1="-1" x2="-1" y2="-1" width="0.2032" layer="21"/>
<wire x1="1" y1="1" x2="2.4" y2="1" width="0.2032" layer="21"/>
<wire x1="2.4" y1="1" x2="2.4" y2="-1" width="0.2032" layer="21"/>
<wire x1="2.4" y1="-1" x2="1" y2="-1" width="0.2032" layer="21"/>
<wire x1="0.3" y1="0.7" x2="0.3" y2="0" width="0.2032" layer="21"/>
<wire x1="0.3" y1="0" x2="0.3" y2="-0.7" width="0.2032" layer="21"/>
<wire x1="0.3" y1="0" x2="-0.3" y2="0.6" width="0.2032" layer="21"/>
<wire x1="-0.3" y1="0.6" x2="-0.3" y2="-0.6" width="0.2032" layer="21"/>
<wire x1="-0.3" y1="-0.6" x2="0.3" y2="0" width="0.2032" layer="21"/>
<smd name="A" x="-1.5" y="0" dx="1.2" dy="1.4" layer="1"/>
<smd name="C" x="1.5" y="0" dx="1.2" dy="1.4" layer="1"/>
<text x="-0.889" y="1.397" size="0.4064" layer="25">&gt;NAME</text>
<text x="-1.016" y="-1.778" size="0.4064" layer="27">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="LED">
<wire x1="1.27" y1="0" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="-1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="-1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.27" y2="0" width="0.254" layer="94"/>
<wire x1="-2.032" y1="-0.762" x2="-3.429" y2="-2.159" width="0.1524" layer="94"/>
<wire x1="-1.905" y1="-1.905" x2="-3.302" y2="-3.302" width="0.1524" layer="94"/>
<text x="3.556" y="-4.572" size="1.778" layer="95" rot="R90">&gt;NAME</text>
<text x="5.715" y="-4.572" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="C" x="0" y="-5.08" visible="off" length="short" direction="pas" rot="R90"/>
<pin name="A" x="0" y="2.54" visible="off" length="short" direction="pas" rot="R270"/>
<polygon width="0.1524" layer="94">
<vertex x="-3.429" y="-2.159"/>
<vertex x="-3.048" y="-1.27"/>
<vertex x="-2.54" y="-1.778"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="-3.302" y="-3.302"/>
<vertex x="-2.921" y="-2.413"/>
<vertex x="-2.413" y="-2.921"/>
</polygon>
</symbol>
</symbols>
<devicesets>
<deviceset name="LED-RED" prefix="D" uservalue="yes">
<description>Assorted Red LEDs&lt;br&gt;
LilyPad 1206- DIO-09912&lt;br&gt;
1206- DIO-00809&lt;br&gt;
0603- DIO-00819&lt;br&gt;
"SMART" means more efficient and expensive</description>
<gates>
<gate name="G$1" symbol="LED" x="0" y="0"/>
</gates>
<devices>
<device name="0603" package="LED-0603">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="DIO-00819"/>
<attribute name="VALUE" value="RED" constant="no"/>
</technology>
</technologies>
</device>
<device name="1206" package="LED-1206">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="DIO-09912"/>
<attribute name="VALUE" value="RED" constant="no"/>
</technology>
</technologies>
</device>
<device name="LILYPAD" package="LED-1206">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="DIO-09912"/>
<attribute name="VALUE" value="RED" constant="no"/>
</technology>
</technologies>
</device>
<device name="0603-SMART" package="LED-0603">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="DIO-11154" constant="no"/>
<attribute name="VALUE" value="Red" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Passives">
<description>&lt;h3&gt;SparkFun Electronics' preferred foot prints&lt;/h3&gt;
In this library you'll find resistors, capacitors, inductors, test points, jumper pads, etc.&lt;br&gt;&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is the end user's responsibility to ensure correctness and suitablity for a given componet or application. If you enjoy using this library, please buy one of our products at www.sparkfun.com.
&lt;br&gt;&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="1206">
<wire x1="-2.473" y1="0.983" x2="2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-0.983" x2="-2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-0.983" x2="-2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="0.983" x2="2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-0.965" y1="0.787" x2="0.965" y2="0.787" width="0.1016" layer="51"/>
<wire x1="-0.965" y1="-0.787" x2="0.965" y2="-0.787" width="0.1016" layer="51"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="1.8" layer="1"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="1.8" layer="1"/>
<text x="-1.27" y="1.143" size="0.4064" layer="25">&gt;NAME</text>
<text x="-1.397" y="-1.524" size="0.4064" layer="27">&gt;VALUE</text>
<rectangle x1="-1.7018" y1="-0.8509" x2="-0.9517" y2="0.8491" layer="51"/>
<rectangle x1="0.9517" y1="-0.8491" x2="1.7018" y2="0.8509" layer="51"/>
<rectangle x1="-0.1999" y1="-0.4001" x2="0.1999" y2="0.4001" layer="35"/>
</package>
<package name="AXIAL-0.3">
<wire x1="-2.54" y1="0.762" x2="2.54" y2="0.762" width="0.2032" layer="21"/>
<wire x1="2.54" y1="0.762" x2="2.54" y2="0" width="0.2032" layer="21"/>
<wire x1="2.54" y1="0" x2="2.54" y2="-0.762" width="0.2032" layer="21"/>
<wire x1="2.54" y1="-0.762" x2="-2.54" y2="-0.762" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="-0.762" x2="-2.54" y2="0" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.54" y2="0.762" width="0.2032" layer="21"/>
<wire x1="2.54" y1="0" x2="2.794" y2="0" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.794" y2="0" width="0.2032" layer="21"/>
<pad name="P$1" x="-3.81" y="0" drill="0.9" diameter="1.8796"/>
<pad name="P$2" x="3.81" y="0" drill="0.9" diameter="1.8796"/>
<text x="-2.54" y="1.27" size="0.4064" layer="25" font="vector">&gt;Name</text>
<text x="-2.032" y="-0.381" size="1.016" layer="21" font="vector" ratio="15">&gt;Value</text>
</package>
<package name="R2010">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-1.662" y1="1.245" x2="1.662" y2="1.245" width="0.1524" layer="51"/>
<wire x1="-1.637" y1="-1.245" x2="1.687" y2="-1.245" width="0.1524" layer="51"/>
<wire x1="-3.473" y1="1.483" x2="3.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="1.483" x2="3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="-1.483" x2="-3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-3.473" y1="-1.483" x2="-3.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="-1.027" y1="1.245" x2="1.027" y2="1.245" width="0.1524" layer="21"/>
<wire x1="-1.002" y1="-1.245" x2="1.016" y2="-1.245" width="0.1524" layer="21"/>
<smd name="1" x="-2.2" y="0" dx="1.8" dy="2.7" layer="1"/>
<smd name="2" x="2.2" y="0" dx="1.8" dy="2.7" layer="1"/>
<text x="-2.54" y="1.5875" size="0.4064" layer="25">&gt;NAME</text>
<text x="-2.54" y="-2.032" size="0.4064" layer="27">&gt;VALUE</text>
<rectangle x1="-2.4892" y1="-1.3208" x2="-1.6393" y2="1.3292" layer="51"/>
<rectangle x1="1.651" y1="-1.3208" x2="2.5009" y2="1.3292" layer="51"/>
</package>
<package name="0805">
<wire x1="-0.3" y1="0.6" x2="0.3" y2="0.6" width="0.1524" layer="21"/>
<wire x1="-0.3" y1="-0.6" x2="0.3" y2="-0.6" width="0.1524" layer="21"/>
<smd name="1" x="-0.9" y="0" dx="0.8" dy="1.2" layer="1"/>
<smd name="2" x="0.9" y="0" dx="0.8" dy="1.2" layer="1"/>
<text x="-0.762" y="0.8255" size="0.4064" layer="25">&gt;NAME</text>
<text x="-1.016" y="-1.397" size="0.4064" layer="27">&gt;VALUE</text>
</package>
<package name="0603-RES">
<wire x1="-1.473" y1="0.983" x2="1.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.473" y1="0.983" x2="1.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.473" y1="-0.983" x2="-1.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="-0.983" x2="-1.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="-0.356" y1="0.432" x2="0.356" y2="0.432" width="0.1016" layer="51"/>
<wire x1="-0.356" y1="-0.419" x2="0.356" y2="-0.419" width="0.1016" layer="51"/>
<smd name="1" x="-0.85" y="0" dx="1.1" dy="1" layer="1"/>
<smd name="2" x="0.85" y="0" dx="1.1" dy="1" layer="1"/>
<text x="-0.889" y="0.762" size="0.4064" layer="25" font="vector">&gt;NAME</text>
<text x="-1.016" y="-1.143" size="0.4064" layer="27" font="vector">&gt;VALUE</text>
<rectangle x1="-0.8382" y1="-0.4699" x2="-0.3381" y2="0.4801" layer="51"/>
<rectangle x1="0.3302" y1="-0.4699" x2="0.8303" y2="0.4801" layer="51"/>
<rectangle x1="-0.1999" y1="-0.3" x2="0.1999" y2="0.3" layer="35"/>
<rectangle x1="-0.2286" y1="-0.381" x2="0.2286" y2="0.381" layer="21"/>
</package>
<package name="0402-RES">
<description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.245" y1="0.224" x2="0.245" y2="0.224" width="0.1524" layer="51"/>
<wire x1="0.245" y1="-0.224" x2="-0.245" y2="-0.224" width="0.1524" layer="51"/>
<wire x1="-1.473" y1="0.483" x2="1.473" y2="0.483" width="0.0508" layer="39"/>
<wire x1="1.473" y1="0.483" x2="1.473" y2="-0.483" width="0.0508" layer="39"/>
<wire x1="1.473" y1="-0.483" x2="-1.473" y2="-0.483" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="-0.483" x2="-1.473" y2="0.483" width="0.0508" layer="39"/>
<smd name="1" x="-0.65" y="0" dx="0.7" dy="0.9" layer="1"/>
<smd name="2" x="0.65" y="0" dx="0.7" dy="0.9" layer="1"/>
<text x="-0.889" y="0.6985" size="0.4064" layer="25">&gt;NAME</text>
<text x="-1.0795" y="-1.143" size="0.4064" layer="27">&gt;VALUE</text>
<rectangle x1="-0.554" y1="-0.3048" x2="-0.254" y2="0.2951" layer="51"/>
<rectangle x1="0.2588" y1="-0.3048" x2="0.5588" y2="0.2951" layer="51"/>
<rectangle x1="-0.1999" y1="-0.3" x2="0.1999" y2="0.3" layer="35"/>
<rectangle x1="-0.2032" y1="-0.3556" x2="0.2032" y2="0.3556" layer="21"/>
</package>
<package name="1/6W-RES">
<description>1/6W Thru-hole Resistor - *UNPROVEN*</description>
<wire x1="-1.55" y1="0.85" x2="-1.55" y2="-0.85" width="0.2032" layer="21"/>
<wire x1="-1.55" y1="-0.85" x2="1.55" y2="-0.85" width="0.2032" layer="21"/>
<wire x1="1.55" y1="-0.85" x2="1.55" y2="0.85" width="0.2032" layer="21"/>
<wire x1="1.55" y1="0.85" x2="-1.55" y2="0.85" width="0.2032" layer="21"/>
<pad name="1" x="-2.5" y="0" drill="0.762"/>
<pad name="2" x="2.5" y="0" drill="0.762"/>
<text x="-1.2662" y="0.9552" size="0.6096" layer="25">&gt;NAME</text>
<text x="-1.423" y="-0.4286" size="0.8128" layer="21" ratio="15">&gt;VALUE</text>
</package>
<package name="R2512">
<wire x1="-2.362" y1="1.473" x2="2.387" y2="1.473" width="0.1524" layer="51"/>
<wire x1="-2.362" y1="-1.473" x2="2.387" y2="-1.473" width="0.1524" layer="51"/>
<smd name="1" x="-2.8" y="0" dx="1.8" dy="3.2" layer="1"/>
<smd name="2" x="2.8" y="0" dx="1.8" dy="3.2" layer="1"/>
<text x="-2.54" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.54" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-3.2004" y1="-1.5494" x2="-2.3505" y2="1.5507" layer="51"/>
<rectangle x1="2.3622" y1="-1.5494" x2="3.2121" y2="1.5507" layer="51"/>
</package>
<package name="AXIAL-0.4">
<description>1/4W Resistor, 0.4" wide&lt;p&gt;

Yageo CFR series &lt;a href="http://www.yageo.com/pdf/yageo/Leaded-R_CFR_2008.pdf"&gt;http://www.yageo.com/pdf/yageo/Leaded-R_CFR_2008.pdf&lt;/a&gt;</description>
<wire x1="-3.15" y1="-1.2" x2="-3.15" y2="1.2" width="0.2032" layer="21"/>
<wire x1="-3.15" y1="1.2" x2="3.15" y2="1.2" width="0.2032" layer="21"/>
<wire x1="3.15" y1="1.2" x2="3.15" y2="-1.2" width="0.2032" layer="21"/>
<wire x1="3.15" y1="-1.2" x2="-3.15" y2="-1.2" width="0.2032" layer="21"/>
<pad name="P$1" x="-5.08" y="0" drill="0.9" diameter="1.8796"/>
<pad name="P$2" x="5.08" y="0" drill="0.9" diameter="1.8796"/>
<text x="-3.175" y="1.905" size="0.8128" layer="25" font="vector" ratio="15">&gt;Name</text>
<text x="-2.286" y="-0.381" size="0.8128" layer="21" font="vector" ratio="15">&gt;Value</text>
</package>
<package name="AXIAL-0.5">
<description>1/2W Resistor, 0.5" wide&lt;p&gt;

Yageo CFR series &lt;a href="http://www.yageo.com/pdf/yageo/Leaded-R_CFR_2008.pdf"&gt;http://www.yageo.com/pdf/yageo/Leaded-R_CFR_2008.pdf&lt;/a&gt;</description>
<wire x1="-4.5" y1="-1.65" x2="-4.5" y2="1.65" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="1.65" x2="4.5" y2="1.65" width="0.2032" layer="21"/>
<wire x1="4.5" y1="1.65" x2="4.5" y2="-1.65" width="0.2032" layer="21"/>
<wire x1="4.5" y1="-1.65" x2="-4.5" y2="-1.65" width="0.2032" layer="21"/>
<pad name="P$1" x="-6.35" y="0" drill="0.9" diameter="1.8796"/>
<pad name="P$2" x="6.35" y="0" drill="0.9" diameter="1.8796"/>
<text x="-4.445" y="2.54" size="0.8128" layer="25" font="vector" ratio="15">&gt;Name</text>
<text x="-3.429" y="-0.381" size="0.8128" layer="21" font="vector" ratio="15">&gt;Value</text>
</package>
<package name="AXIAL-0.6">
<description>1W Resistor, 0.6" wide&lt;p&gt;

Yageo CFR series &lt;a href="http://www.yageo.com/pdf/yageo/Leaded-R_CFR_2008.pdf"&gt;http://www.yageo.com/pdf/yageo/Leaded-R_CFR_2008.pdf&lt;/a&gt;</description>
<wire x1="-5.75" y1="-2.25" x2="-5.75" y2="2.25" width="0.2032" layer="21"/>
<wire x1="-5.75" y1="2.25" x2="5.75" y2="2.25" width="0.2032" layer="21"/>
<wire x1="5.75" y1="2.25" x2="5.75" y2="-2.25" width="0.2032" layer="21"/>
<wire x1="5.75" y1="-2.25" x2="-5.75" y2="-2.25" width="0.2032" layer="21"/>
<pad name="P$1" x="-7.62" y="0" drill="1.2" diameter="1.8796"/>
<pad name="P$2" x="7.62" y="0" drill="1.2" diameter="1.8796"/>
<text x="-5.715" y="3.175" size="0.8128" layer="25" font="vector" ratio="15">&gt;Name</text>
<text x="-4.064" y="-0.381" size="0.8128" layer="21" font="vector" ratio="15">&gt;Value</text>
</package>
<package name="AXIAL-0.8">
<description>2W Resistor, 0.8" wide&lt;p&gt;

Yageo CFR series &lt;a href="http://www.yageo.com/pdf/yageo/Leaded-R_CFR_2008.pdf"&gt;http://www.yageo.com/pdf/yageo/Leaded-R_CFR_2008.pdf&lt;/a&gt;</description>
<wire x1="-7.75" y1="-2.5" x2="-7.75" y2="2.5" width="0.2032" layer="21"/>
<wire x1="-7.75" y1="2.5" x2="7.75" y2="2.5" width="0.2032" layer="21"/>
<wire x1="7.75" y1="2.5" x2="7.75" y2="-2.5" width="0.2032" layer="21"/>
<wire x1="7.75" y1="-2.5" x2="-7.75" y2="-2.5" width="0.2032" layer="21"/>
<pad name="P$1" x="-10.16" y="0" drill="1.2" diameter="1.8796"/>
<pad name="P$2" x="10.16" y="0" drill="1.2" diameter="1.8796"/>
<text x="-7.62" y="3.175" size="0.8128" layer="25" font="vector" ratio="15">&gt;Name</text>
<text x="-5.969" y="-0.381" size="0.8128" layer="21" font="vector" ratio="15">&gt;Value</text>
</package>
<package name="AXIAL-0.3-KIT">
<description>&lt;h3&gt;AXIAL-0.3-KIT&lt;/h3&gt;

Commonly used for 1/4W through-hole resistors. 0.3" pitch between holes.&lt;br&gt;
&lt;br&gt;

&lt;b&gt;Warning:&lt;/b&gt; This is the KIT version of the AXIAL-0.3 package. This package has a smaller diameter top stop mask, which doesn't cover the diameter of the pad. This means only the bottom side of the pads' copper will be exposed. You'll only be able to solder to the bottom side.</description>
<wire x1="-2.54" y1="1.27" x2="2.54" y2="1.27" width="0.254" layer="21"/>
<wire x1="2.54" y1="1.27" x2="2.54" y2="0" width="0.254" layer="21"/>
<wire x1="2.54" y1="0" x2="2.54" y2="-1.27" width="0.254" layer="21"/>
<wire x1="2.54" y1="-1.27" x2="-2.54" y2="-1.27" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-1.27" x2="-2.54" y2="0" width="0.254" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.54" y2="1.27" width="0.254" layer="21"/>
<wire x1="2.54" y1="0" x2="2.794" y2="0" width="0.254" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.794" y2="0" width="0.254" layer="21"/>
<pad name="P$1" x="-3.81" y="0" drill="1.016" diameter="2.032" stop="no"/>
<pad name="P$2" x="3.81" y="0" drill="1.016" diameter="2.032" stop="no"/>
<text x="-2.54" y="1.27" size="0.4064" layer="25" font="vector">&gt;Name</text>
<text x="-2.159" y="-0.762" size="1.27" layer="21" font="vector" ratio="15">&gt;Value</text>
<polygon width="0.127" layer="30">
<vertex x="3.8201" y="-0.9449" curve="-90"/>
<vertex x="2.8652" y="-0.0152" curve="-90.011749"/>
<vertex x="3.8176" y="0.9602" curve="-90"/>
<vertex x="4.7676" y="-0.0178" curve="-90.024193"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="3.8176" y="-0.4369" curve="-90.012891"/>
<vertex x="3.3731" y="-0.0127" curve="-90"/>
<vertex x="3.8176" y="0.4546" curve="-90"/>
<vertex x="4.2595" y="-0.0025" curve="-90.012967"/>
</polygon>
<polygon width="0.127" layer="30">
<vertex x="-3.8075" y="-0.9525" curve="-90"/>
<vertex x="-4.7624" y="-0.0228" curve="-90.011749"/>
<vertex x="-3.81" y="0.9526" curve="-90"/>
<vertex x="-2.86" y="-0.0254" curve="-90.024193"/>
</polygon>
<polygon width="0.127" layer="29">
<vertex x="-3.81" y="-0.4445" curve="-90.012891"/>
<vertex x="-4.2545" y="-0.0203" curve="-90"/>
<vertex x="-3.81" y="0.447" curve="-90"/>
<vertex x="-3.3681" y="-0.0101" curve="-90.012967"/>
</polygon>
</package>
<package name="AXIAL-0.3EZ">
<description>This is the "EZ" version of the standard .3" spaced resistor package.&lt;br&gt;
It has a reduced top mask to make it harder to install upside-down.</description>
<wire x1="-2.54" y1="0.762" x2="2.54" y2="0.762" width="0.2032" layer="21"/>
<wire x1="2.54" y1="0.762" x2="2.54" y2="0" width="0.2032" layer="21"/>
<wire x1="2.54" y1="0" x2="2.54" y2="-0.762" width="0.2032" layer="21"/>
<wire x1="2.54" y1="-0.762" x2="-2.54" y2="-0.762" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="-0.762" x2="-2.54" y2="0" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.54" y2="0.762" width="0.2032" layer="21"/>
<wire x1="2.54" y1="0" x2="2.794" y2="0" width="0.2032" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.794" y2="0" width="0.2032" layer="21"/>
<pad name="P$1" x="-3.81" y="0" drill="0.9" diameter="1.8796" stop="no"/>
<pad name="P$2" x="3.81" y="0" drill="0.9" diameter="1.8796" stop="no"/>
<text x="-2.54" y="1.27" size="0.4064" layer="25" font="vector">&gt;Name</text>
<text x="-2.032" y="-0.381" size="1.016" layer="21" font="vector" ratio="15">&gt;Value</text>
<circle x="-3.81" y="0" radius="0.508" width="0" layer="29"/>
<circle x="3.81" y="0" radius="0.523634375" width="0" layer="29"/>
<circle x="-3.81" y="0" radius="1.02390625" width="0" layer="30"/>
<circle x="3.81" y="0" radius="1.04726875" width="0" layer="30"/>
</package>
</packages>
<symbols>
<symbol name="RESISTOR">
<wire x1="-2.54" y1="0" x2="-2.159" y2="1.016" width="0.1524" layer="94"/>
<wire x1="-2.159" y1="1.016" x2="-1.524" y2="-1.016" width="0.1524" layer="94"/>
<wire x1="-1.524" y1="-1.016" x2="-0.889" y2="1.016" width="0.1524" layer="94"/>
<wire x1="-0.889" y1="1.016" x2="-0.254" y2="-1.016" width="0.1524" layer="94"/>
<wire x1="-0.254" y1="-1.016" x2="0.381" y2="1.016" width="0.1524" layer="94"/>
<wire x1="0.381" y1="1.016" x2="1.016" y2="-1.016" width="0.1524" layer="94"/>
<wire x1="1.016" y1="-1.016" x2="1.651" y2="1.016" width="0.1524" layer="94"/>
<wire x1="1.651" y1="1.016" x2="2.286" y2="-1.016" width="0.1524" layer="94"/>
<wire x1="2.286" y1="-1.016" x2="2.54" y2="0" width="0.1524" layer="94"/>
<text x="-3.81" y="1.4986" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.81" y="-3.302" size="1.778" layer="96">&gt;VALUE</text>
<pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
<pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="RESISTOR" prefix="R" uservalue="yes">
<description>&lt;b&gt;Resistor&lt;/b&gt;
Basic schematic elements and footprints for 0603, 1206, and PTH resistors.</description>
<gates>
<gate name="G$1" symbol="RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="1206" package="1206">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="AXIAL-0.3" package="AXIAL-0.3">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="2010" package="R2010">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0805-RES" package="0805">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0603-RES" package="0603-RES">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0402-RES" package="0402-RES">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-1/6W" package="1/6W-RES">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="2512" package="R2512">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-1/4W" package="AXIAL-0.4">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-1/2W" package="AXIAL-0.5">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-1W" package="AXIAL-0.6">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-2W" package="AXIAL-0.8">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="KIT" package="AXIAL-0.3-KIT">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="EZ" package="AXIAL-0.3EZ">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Aesthetics">
<description>&lt;h3&gt;SparkFun Electronics' preferred foot prints&lt;/h3&gt;
In this library you'll find non-functional items- supply symbols, logos, notations, frame blocks, etc.&lt;br&gt;&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is the end user's responsibility to ensure correctness and suitablity for a given componet or application. If you enjoy using this library, please buy one of our products at www.sparkfun.com.
&lt;br&gt;&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
</packages>
<symbols>
<symbol name="3.3V">
<wire x1="0.762" y1="1.27" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="-0.762" y2="1.27" width="0.254" layer="94"/>
<text x="-1.016" y="3.556" size="1.778" layer="96">&gt;VALUE</text>
<pin name="3.3V" x="0" y="0" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="GND">
<pin name="3.3V" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
<wire x1="-2.032" y1="0" x2="2.032" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="-0.762" x2="1.27" y2="-0.762" width="0.254" layer="94"/>
<wire x1="-0.508" y1="-1.524" x2="0.508" y2="-1.524" width="0.254" layer="94"/>
<text x="2.54" y="0" size="1.778" layer="94">&gt;NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="3.3V" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="3.3V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GND2" prefix="GND">
<gates>
<gate name="G$1" symbol="GND" x="2.54" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U$1" library="GeekAmmo" deviceset="DOCFIELD1" device=""/>
<part name="U$2" library="GeekAmmo" deviceset="FRAME2-A4" device=""/>
<part name="S1" library="SparkFun-Electromechanical" deviceset="SWITCH-MOMENTARY-2" device="PTH"/>
<part name="S2" library="SparkFun-Electromechanical" deviceset="SWITCH-SPDT" device="KIT"/>
<part name="S3" library="SparkFun-Electromechanical" deviceset="SWITCH-DPDT" device="ES"/>
<part name="D1" library="SparkFun-LED" deviceset="LED-RED" device="0603" value="RED"/>
<part name="D2" library="SparkFun-LED" deviceset="LED-RED" device="0603" value="RED"/>
<part name="D3" library="SparkFun-LED" deviceset="LED-RED" device="0603" value="RED"/>
<part name="D4" library="SparkFun-LED" deviceset="LED-RED" device="0603" value="RED"/>
<part name="D5" library="SparkFun-LED" deviceset="LED-RED" device="0603" value="RED"/>
<part name="R1" library="SparkFun-Passives" deviceset="RESISTOR" device="0805-RES"/>
<part name="R2" library="SparkFun-Passives" deviceset="RESISTOR" device="0805-RES"/>
<part name="R3" library="SparkFun-Passives" deviceset="RESISTOR" device="0805-RES"/>
<part name="R4" library="SparkFun-Passives" deviceset="RESISTOR" device="0805-RES"/>
<part name="R5" library="SparkFun-Passives" deviceset="RESISTOR" device="0805-RES"/>
<part name="SUPPLY1" library="SparkFun-Aesthetics" deviceset="3.3V" device=""/>
<part name="SUPPLY2" library="SparkFun-Aesthetics" deviceset="3.3V" device=""/>
<part name="SUPPLY3" library="SparkFun-Aesthetics" deviceset="3.3V" device=""/>
<part name="SUPPLY4" library="SparkFun-Aesthetics" deviceset="3.3V" device=""/>
<part name="GND1" library="SparkFun-Aesthetics" deviceset="GND2" device=""/>
<part name="GND2" library="SparkFun-Aesthetics" deviceset="GND2" device=""/>
<part name="GND3" library="SparkFun-Aesthetics" deviceset="GND2" device=""/>
<part name="GND4" library="SparkFun-Aesthetics" deviceset="GND2" device=""/>
<part name="GND5" library="SparkFun-Aesthetics" deviceset="GND2" device=""/>
<part name="GND6" library="SparkFun-Aesthetics" deviceset="GND2" device=""/>
<part name="GND7" library="SparkFun-Aesthetics" deviceset="GND2" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="2.54" y="124.46" size="1.778" layer="91">First Circuit Demonstrates SPST</text>
<text x="7.62" y="71.12" size="1.778" layer="91">Second Circuit: SPDT</text>
<text x="147.32" y="33.02" size="1.778" layer="91">Third Circuit Double Pole Double Throw</text>
<text x="144.78" y="86.36" size="1.778" layer="91">The circuits shown here are to assist in
 the teaching of basic push buttons. The
 first switch is a simple SPST switch that
 shows that if the button is pushed, then
 the light turns on. In the Second Circuit
moving the switch causes different 
lights to turn on. The final circuit shows
two lights go on together, but from
different lines. </text>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="48.26" y="-15.24"/>
<instance part="U$2" gate="G$1" x="-10.16" y="-43.18"/>
<instance part="S1" gate="G$1" x="22.86" y="109.22" smashed="yes">
<attribute name="NAME" x="22.86" y="106.68" size="1.778" layer="95"/>
<attribute name="VALUE" x="17.78" y="111.76" size="1.778" layer="96"/>
</instance>
<instance part="S2" gate="1" x="22.86" y="55.88"/>
<instance part="S3" gate="G$1" x="165.1" y="10.16"/>
<instance part="D1" gate="G$1" x="38.1" y="101.6"/>
<instance part="D2" gate="G$1" x="53.34" y="43.18"/>
<instance part="D3" gate="G$1" x="40.64" y="43.18"/>
<instance part="D4" gate="G$1" x="144.78" y="-2.54"/>
<instance part="D5" gate="G$1" x="132.08" y="-2.54"/>
<instance part="R1" gate="G$1" x="38.1" y="91.44" rot="R90"/>
<instance part="R2" gate="G$1" x="33.02" y="58.42" rot="R180"/>
<instance part="R3" gate="G$1" x="33.02" y="53.34" rot="R180"/>
<instance part="R4" gate="G$1" x="154.94" y="12.7" rot="R180"/>
<instance part="R5" gate="G$1" x="154.94" y="5.08" rot="R180"/>
<instance part="SUPPLY1" gate="G$1" x="7.62" y="114.3"/>
<instance part="SUPPLY2" gate="G$1" x="7.62" y="58.42"/>
<instance part="SUPPLY3" gate="G$1" x="182.88" y="17.78"/>
<instance part="SUPPLY4" gate="G$1" x="170.18" y="17.78"/>
<instance part="GND1" gate="G$1" x="38.1" y="83.82"/>
<instance part="GND2" gate="G$1" x="40.64" y="33.02"/>
<instance part="GND3" gate="G$1" x="53.34" y="33.02"/>
<instance part="GND4" gate="G$1" x="187.96" y="-2.54"/>
<instance part="GND5" gate="G$1" x="170.18" y="-2.54"/>
<instance part="GND6" gate="G$1" x="144.78" y="-10.16"/>
<instance part="GND7" gate="G$1" x="132.08" y="-10.16"/>
</instances>
<busses>
</busses>
<nets>
<net name="3.3V" class="0">
<segment>
<pinref part="SUPPLY1" gate="G$1" pin="3.3V"/>
<pinref part="S1" gate="G$1" pin="1"/>
<wire x1="7.62" y1="114.3" x2="7.62" y2="109.22" width="0.1524" layer="91"/>
<wire x1="7.62" y1="109.22" x2="17.78" y2="109.22" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R1" gate="G$1" pin="1"/>
<pinref part="GND1" gate="G$1" pin="3.3V"/>
</segment>
<segment>
<pinref part="SUPPLY2" gate="G$1" pin="3.3V"/>
<pinref part="S2" gate="1" pin="P"/>
<wire x1="7.62" y1="58.42" x2="7.62" y2="55.88" width="0.1524" layer="91"/>
<wire x1="7.62" y1="55.88" x2="20.32" y2="55.88" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="D3" gate="G$1" pin="C"/>
<pinref part="GND2" gate="G$1" pin="3.3V"/>
<wire x1="40.64" y1="35.56" x2="40.64" y2="38.1" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="D2" gate="G$1" pin="C"/>
<pinref part="GND3" gate="G$1" pin="3.3V"/>
<wire x1="53.34" y1="35.56" x2="53.34" y2="38.1" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="D5" gate="G$1" pin="C"/>
<pinref part="GND7" gate="G$1" pin="3.3V"/>
</segment>
<segment>
<pinref part="D4" gate="G$1" pin="C"/>
<pinref part="GND6" gate="G$1" pin="3.3V"/>
</segment>
<segment>
<pinref part="S3" gate="G$1" pin="1"/>
<pinref part="SUPPLY4" gate="G$1" pin="3.3V"/>
<wire x1="170.18" y1="15.24" x2="170.18" y2="17.78" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="S3" gate="G$1" pin="4"/>
<pinref part="SUPPLY3" gate="G$1" pin="3.3V"/>
<wire x1="170.18" y1="7.62" x2="182.88" y2="7.62" width="0.1524" layer="91"/>
<wire x1="182.88" y1="7.62" x2="182.88" y2="17.78" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="S3" gate="G$1" pin="3"/>
<pinref part="GND4" gate="G$1" pin="3.3V"/>
<wire x1="170.18" y1="10.16" x2="187.96" y2="10.16" width="0.1524" layer="91"/>
<wire x1="187.96" y1="10.16" x2="187.96" y2="0" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="S3" gate="G$1" pin="6"/>
<pinref part="GND5" gate="G$1" pin="3.3V"/>
<wire x1="170.18" y1="2.54" x2="170.18" y2="0" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="S1" gate="G$1" pin="2"/>
<wire x1="27.94" y1="109.22" x2="38.1" y2="109.22" width="0.1524" layer="91"/>
<pinref part="D1" gate="G$1" pin="A"/>
<wire x1="38.1" y1="109.22" x2="38.1" y2="104.14" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="C"/>
<pinref part="R1" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="S2" gate="1" pin="S"/>
<pinref part="R3" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="S2" gate="1" pin="O"/>
<pinref part="R2" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="R2" gate="G$1" pin="1"/>
<pinref part="D2" gate="G$1" pin="A"/>
<wire x1="38.1" y1="58.42" x2="53.34" y2="58.42" width="0.1524" layer="91"/>
<wire x1="53.34" y1="58.42" x2="53.34" y2="45.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="R3" gate="G$1" pin="1"/>
<pinref part="D3" gate="G$1" pin="A"/>
<wire x1="38.1" y1="53.34" x2="40.64" y2="53.34" width="0.1524" layer="91"/>
<wire x1="40.64" y1="53.34" x2="40.64" y2="45.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="S3" gate="G$1" pin="5"/>
<pinref part="R5" gate="G$1" pin="1"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="S3" gate="G$1" pin="2"/>
<pinref part="R4" gate="G$1" pin="1"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="R4" gate="G$1" pin="2"/>
<wire x1="149.86" y1="12.7" x2="132.08" y2="12.7" width="0.1524" layer="91"/>
<pinref part="D5" gate="G$1" pin="A"/>
<wire x1="132.08" y1="12.7" x2="132.08" y2="0" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="R5" gate="G$1" pin="2"/>
<wire x1="149.86" y1="5.08" x2="144.78" y2="5.08" width="0.1524" layer="91"/>
<pinref part="D4" gate="G$1" pin="A"/>
<wire x1="144.78" y1="5.08" x2="144.78" y2="0" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="6.3" minversion="6.2.2" severity="warning">
Since Version 6.2.2 text objects can contain more than one line,
which will not be processed correctly with this version.
</note>
</compatibility>
</eagle>
