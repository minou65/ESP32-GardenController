<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="yes" altdistance="0.01" altunitdist="inch" altunit="inch"/>
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
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="frames" urn="urn:adsk.eagle:library:229">
<description>&lt;b&gt;Frames for Sheet and Layout&lt;/b&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="A3L-LOC" urn="urn:adsk.eagle:symbol:13881/1" library_version="1">
<wire x1="288.29" y1="3.81" x2="342.265" y2="3.81" width="0.1016" layer="94"/>
<wire x1="342.265" y1="3.81" x2="373.38" y2="3.81" width="0.1016" layer="94"/>
<wire x1="373.38" y1="3.81" x2="383.54" y2="3.81" width="0.1016" layer="94"/>
<wire x1="383.54" y1="3.81" x2="383.54" y2="8.89" width="0.1016" layer="94"/>
<wire x1="383.54" y1="8.89" x2="383.54" y2="13.97" width="0.1016" layer="94"/>
<wire x1="383.54" y1="13.97" x2="383.54" y2="19.05" width="0.1016" layer="94"/>
<wire x1="383.54" y1="19.05" x2="383.54" y2="24.13" width="0.1016" layer="94"/>
<wire x1="288.29" y1="3.81" x2="288.29" y2="24.13" width="0.1016" layer="94"/>
<wire x1="288.29" y1="24.13" x2="342.265" y2="24.13" width="0.1016" layer="94"/>
<wire x1="342.265" y1="24.13" x2="383.54" y2="24.13" width="0.1016" layer="94"/>
<wire x1="373.38" y1="3.81" x2="373.38" y2="8.89" width="0.1016" layer="94"/>
<wire x1="373.38" y1="8.89" x2="383.54" y2="8.89" width="0.1016" layer="94"/>
<wire x1="373.38" y1="8.89" x2="342.265" y2="8.89" width="0.1016" layer="94"/>
<wire x1="342.265" y1="8.89" x2="342.265" y2="3.81" width="0.1016" layer="94"/>
<wire x1="342.265" y1="8.89" x2="342.265" y2="13.97" width="0.1016" layer="94"/>
<wire x1="342.265" y1="13.97" x2="383.54" y2="13.97" width="0.1016" layer="94"/>
<wire x1="342.265" y1="13.97" x2="342.265" y2="19.05" width="0.1016" layer="94"/>
<wire x1="342.265" y1="19.05" x2="383.54" y2="19.05" width="0.1016" layer="94"/>
<wire x1="342.265" y1="19.05" x2="342.265" y2="24.13" width="0.1016" layer="94"/>
<text x="344.17" y="15.24" size="2.54" layer="94">&gt;DRAWING_NAME</text>
<text x="344.17" y="10.16" size="2.286" layer="94">&gt;LAST_DATE_TIME</text>
<text x="357.505" y="5.08" size="2.54" layer="94">&gt;SHEET</text>
<text x="343.916" y="4.953" size="2.54" layer="94">Sheet:</text>
<frame x1="0" y1="0" x2="387.35" y2="260.35" columns="8" rows="5" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="A3L-LOC" urn="urn:adsk.eagle:component:13942/1" prefix="FRAME" uservalue="yes" library_version="1">
<description>&lt;b&gt;FRAME&lt;/b&gt;&lt;p&gt;
DIN A3, landscape with location and doc. field</description>
<gates>
<gate name="G$1" symbol="A3L-LOC" x="0" y="0"/>
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
<part name="FRAME1" library="frames" library_urn="urn:adsk.eagle:library:229" deviceset="A3L-LOC" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<wire x1="91.44" y1="170.18" x2="91.44" y2="132.08" width="0.1524" layer="94"/>
<wire x1="91.44" y1="132.08" x2="93.98" y2="132.08" width="0.1524" layer="94"/>
<wire x1="93.98" y1="132.08" x2="96.52" y2="132.08" width="0.1524" layer="94"/>
<wire x1="96.52" y1="132.08" x2="99.06" y2="132.08" width="0.1524" layer="94"/>
<wire x1="99.06" y1="132.08" x2="104.14" y2="132.08" width="0.1524" layer="94"/>
<wire x1="104.14" y1="132.08" x2="106.68" y2="132.08" width="0.1524" layer="94"/>
<wire x1="106.68" y1="132.08" x2="109.22" y2="132.08" width="0.1524" layer="94"/>
<wire x1="109.22" y1="132.08" x2="114.3" y2="132.08" width="0.1524" layer="94"/>
<wire x1="124.46" y1="132.08" x2="127" y2="132.08" width="0.1524" layer="94"/>
<wire x1="127" y1="132.08" x2="129.54" y2="132.08" width="0.1524" layer="94"/>
<wire x1="129.54" y1="132.08" x2="134.62" y2="132.08" width="0.1524" layer="94"/>
<wire x1="134.62" y1="132.08" x2="137.16" y2="132.08" width="0.1524" layer="94"/>
<wire x1="137.16" y1="132.08" x2="139.7" y2="132.08" width="0.1524" layer="94"/>
<wire x1="139.7" y1="132.08" x2="144.78" y2="132.08" width="0.1524" layer="94"/>
<wire x1="144.78" y1="132.08" x2="147.32" y2="132.08" width="0.1524" layer="94"/>
<wire x1="147.32" y1="132.08" x2="149.86" y2="132.08" width="0.1524" layer="94"/>
<wire x1="149.86" y1="132.08" x2="154.94" y2="132.08" width="0.1524" layer="94"/>
<wire x1="154.94" y1="132.08" x2="157.48" y2="132.08" width="0.1524" layer="94"/>
<wire x1="157.48" y1="132.08" x2="160.02" y2="132.08" width="0.1524" layer="94"/>
<wire x1="160.02" y1="132.08" x2="165.1" y2="132.08" width="0.1524" layer="94"/>
<wire x1="165.1" y1="132.08" x2="167.64" y2="132.08" width="0.1524" layer="94"/>
<wire x1="167.64" y1="132.08" x2="170.18" y2="132.08" width="0.1524" layer="94"/>
<wire x1="170.18" y1="132.08" x2="172.72" y2="132.08" width="0.1524" layer="94"/>
<wire x1="172.72" y1="132.08" x2="172.72" y2="157.48" width="0.1524" layer="94"/>
<wire x1="172.72" y1="157.48" x2="172.72" y2="160.02" width="0.1524" layer="94"/>
<wire x1="172.72" y1="160.02" x2="172.72" y2="170.18" width="0.1524" layer="94"/>
<wire x1="172.72" y1="170.18" x2="101.6" y2="170.18" width="0.1524" layer="94"/>
<wire x1="101.6" y1="170.18" x2="99.06" y2="170.18" width="0.1524" layer="94"/>
<wire x1="99.06" y1="170.18" x2="96.52" y2="170.18" width="0.1524" layer="94"/>
<wire x1="96.52" y1="170.18" x2="91.44" y2="170.18" width="0.1524" layer="94"/>
<wire x1="93.98" y1="132.08" x2="93.98" y2="129.54" width="0.1524" layer="94"/>
<wire x1="96.52" y1="132.08" x2="96.52" y2="129.54" width="0.1524" layer="94"/>
<wire x1="99.06" y1="132.08" x2="99.06" y2="129.54" width="0.1524" layer="94"/>
<wire x1="104.14" y1="132.08" x2="104.14" y2="129.54" width="0.1524" layer="94"/>
<wire x1="106.68" y1="132.08" x2="106.68" y2="129.54" width="0.1524" layer="94"/>
<wire x1="109.22" y1="132.08" x2="109.22" y2="129.54" width="0.1524" layer="94"/>
<wire x1="114.3" y1="129.54" x2="114.3" y2="132.08" width="0.1524" layer="94"/>
<wire x1="114.3" y1="132.08" x2="116.84" y2="132.08" width="0.1524" layer="94"/>
<wire x1="116.84" y1="129.54" x2="116.84" y2="132.08" width="0.1524" layer="94"/>
<wire x1="116.84" y1="132.08" x2="119.38" y2="132.08" width="0.1524" layer="94"/>
<wire x1="119.38" y1="129.54" x2="119.38" y2="132.08" width="0.1524" layer="94"/>
<wire x1="119.38" y1="132.08" x2="124.46" y2="132.08" width="0.1524" layer="94"/>
<wire x1="124.46" y1="132.08" x2="124.46" y2="129.54" width="0.1524" layer="94"/>
<wire x1="127" y1="132.08" x2="127" y2="129.54" width="0.1524" layer="94"/>
<wire x1="129.54" y1="132.08" x2="129.54" y2="129.54" width="0.1524" layer="94"/>
<wire x1="134.62" y1="132.08" x2="134.62" y2="129.54" width="0.1524" layer="94"/>
<wire x1="137.16" y1="132.08" x2="137.16" y2="129.54" width="0.1524" layer="94"/>
<wire x1="139.7" y1="132.08" x2="139.7" y2="129.54" width="0.1524" layer="94"/>
<wire x1="144.78" y1="132.08" x2="144.78" y2="129.54" width="0.1524" layer="94"/>
<wire x1="147.32" y1="132.08" x2="147.32" y2="129.54" width="0.1524" layer="94"/>
<wire x1="149.86" y1="132.08" x2="149.86" y2="129.54" width="0.1524" layer="94"/>
<wire x1="154.94" y1="132.08" x2="154.94" y2="129.54" width="0.1524" layer="94"/>
<wire x1="157.48" y1="132.08" x2="157.48" y2="129.54" width="0.1524" layer="94"/>
<wire x1="160.02" y1="132.08" x2="160.02" y2="129.54" width="0.1524" layer="94"/>
<text x="94.488" y="132.588" size="2.1844" layer="94" rot="R90">NO1</text>
<text x="104.648" y="132.588" size="2.1844" layer="94" rot="R90">NO2</text>
<text x="114.808" y="132.334" size="2.1844" layer="94" rot="R90">NO3</text>
<text x="124.968" y="132.588" size="2.1844" layer="94" rot="R90">NO4</text>
<text x="135.128" y="132.588" size="2.1844" layer="94" rot="R90">NO5</text>
<text x="145.288" y="132.588" size="2.1844" layer="94" rot="R90">NO6</text>
<text x="155.448" y="132.588" size="2.1844" layer="94" rot="R90">NO7</text>
<text x="165.608" y="132.588" size="2.1844" layer="94" rot="R90">NO8</text>
<text x="97.282" y="132.588" size="2.1844" layer="94" rot="R90">COM1</text>
<text x="107.188" y="132.588" size="2.1844" layer="94" rot="R90">COM2</text>
<text x="117.348" y="132.588" size="2.1844" layer="94" rot="R90">COM3</text>
<text x="127.508" y="132.588" size="2.1844" layer="94" rot="R90">COM4</text>
<text x="137.414" y="132.588" size="2.1844" layer="94" rot="R90">COM5</text>
<text x="147.828" y="132.588" size="2.1844" layer="94" rot="R90">COM6</text>
<text x="157.988" y="132.588" size="2.1844" layer="94" rot="R90">COM7</text>
<text x="168.148" y="132.588" size="2.1844" layer="94" rot="R90">COM8</text>
<text x="99.568" y="132.588" size="2.1844" layer="94" rot="R90">NC1</text>
<text x="109.728" y="132.334" size="2.1844" layer="94" rot="R90">NC2</text>
<text x="119.888" y="132.588" size="2.1844" layer="94" rot="R90">NC3</text>
<text x="130.048" y="132.588" size="2.1844" layer="94" rot="R90">NC4</text>
<text x="140.208" y="132.588" size="2.1844" layer="94" rot="R90">NC5</text>
<text x="150.368" y="132.588" size="2.1844" layer="94" rot="R90">NC6</text>
<text x="160.528" y="132.588" size="2.1844" layer="94" rot="R90">NC7</text>
<text x="170.688" y="132.588" size="2.1844" layer="94" rot="R90">NC8</text>
<wire x1="165.1" y1="132.08" x2="165.1" y2="129.54" width="0.1524" layer="94"/>
<wire x1="167.64" y1="132.08" x2="167.64" y2="129.54" width="0.1524" layer="94"/>
<wire x1="170.18" y1="132.08" x2="170.18" y2="129.54" width="0.1524" layer="94"/>
<wire x1="96.52" y1="170.18" x2="96.52" y2="172.72" width="0.1524" layer="94"/>
<wire x1="99.06" y1="170.18" x2="99.06" y2="172.72" width="0.1524" layer="94"/>
<wire x1="101.6" y1="170.18" x2="101.6" y2="172.72" width="0.1524" layer="94"/>
<text x="96.012" y="169.164" size="2.1844" layer="94" rot="R270">7-30v</text>
<text x="98.552" y="169.164" size="2.1844" layer="94" rot="R270">GND</text>
<text x="101.346" y="169.164" size="2.1844" layer="94" rot="R270">+5V</text>
<wire x1="40.64" y1="198.12" x2="40.64" y2="190.5" width="0.1524" layer="94"/>
<wire x1="40.64" y1="190.5" x2="43.18" y2="190.5" width="0.1524" layer="94"/>
<wire x1="43.18" y1="190.5" x2="45.72" y2="190.5" width="0.1524" layer="94"/>
<wire x1="45.72" y1="190.5" x2="48.26" y2="190.5" width="0.1524" layer="94"/>
<wire x1="48.26" y1="190.5" x2="50.8" y2="190.5" width="0.1524" layer="94"/>
<wire x1="50.8" y1="190.5" x2="53.34" y2="190.5" width="0.1524" layer="94"/>
<wire x1="53.34" y1="190.5" x2="53.34" y2="198.12" width="0.1524" layer="94"/>
<wire x1="53.34" y1="198.12" x2="40.64" y2="198.12" width="0.1524" layer="94"/>
<wire x1="43.18" y1="190.5" x2="43.18" y2="187.96" width="0.1524" layer="94"/>
<wire x1="45.72" y1="190.5" x2="45.72" y2="187.96" width="0.1524" layer="94"/>
<wire x1="48.26" y1="190.5" x2="48.26" y2="187.96" width="0.1524" layer="94"/>
<wire x1="50.8" y1="190.5" x2="50.8" y2="187.96" width="0.1524" layer="94"/>
<text x="43.688" y="191.008" size="1.27" layer="94" rot="R90">AC</text>
<text x="46.228" y="191.262" size="1.27" layer="94" rot="R90">AC</text>
<text x="48.768" y="191.008" size="1.27" layer="94" rot="R90">GND</text>
<text x="51.308" y="190.754" size="1.27" layer="94" rot="R90">+24V</text>
<text x="91.44" y="93.98" size="2.54" layer="95" rot="R90">K2:WH</text>
<text x="93.98" y="93.98" size="2.54" layer="95" rot="R90">K2:BL</text>
<text x="104.14" y="93.98" size="2.54" layer="95" rot="R90">K2:TQ</text>
<text x="114.3" y="93.98" size="2.54" layer="95" rot="R90">K2:VT</text>
<text x="121.92" y="93.98" size="2.54" layer="95" rot="R90">K3:WH</text>
<text x="124.46" y="93.98" size="2.54" layer="95" rot="R90">K3:BL</text>
<text x="134.62" y="93.98" size="2.54" layer="95" rot="R90">K3:TQ</text>
<text x="144.78" y="93.98" size="2.54" layer="95" rot="R90">K3:VT</text>
<text x="154.94" y="93.98" size="2.54" layer="95" rot="R90">K2:WH2</text>
<text x="157.48" y="93.98" size="2.54" layer="95" rot="R90">K2:OR</text>
<text x="165.1" y="93.98" size="2.54" layer="95" rot="R90">K3:WH2</text>
<text x="167.64" y="93.98" size="2.54" layer="95" rot="R90">K3:OR</text>
<wire x1="172.72" y1="160.02" x2="175.26" y2="160.02" width="0.1524" layer="94"/>
<wire x1="172.72" y1="157.48" x2="175.26" y2="157.48" width="0.1524" layer="94"/>
<text x="164.592" y="159.512" size="2.1844" layer="94">IO36</text>
<text x="164.338" y="156.972" size="2.1844" layer="94">IO39</text>
<text x="160.02" y="93.98" size="2.54" layer="95" rot="R90">K2:TQ2</text>
<text x="170.18" y="93.98" size="2.54" layer="95" rot="R90">K3:TQ2</text>
<wire x1="193.04" y1="111.76" x2="33.02" y2="111.76" width="0.1524" layer="94"/>
<wire x1="33.02" y1="111.76" x2="33.02" y2="205.74" width="0.1524" layer="94"/>
<wire x1="33.02" y1="205.74" x2="193.04" y2="205.74" width="0.1524" layer="94"/>
<wire x1="193.04" y1="205.74" x2="193.04" y2="111.76" width="0.1524" layer="94"/>
<text x="35.56" y="205.74" size="2.54" layer="94">ESP32-GardenController</text>
<wire x1="266.7" y1="182.88" x2="266.7" y2="177.8" width="0.1524" layer="94"/>
<wire x1="266.7" y1="177.8" x2="271.78" y2="180.34" width="0.1524" layer="94"/>
<wire x1="271.78" y1="180.34" x2="276.86" y2="182.88" width="0.1524" layer="94"/>
<wire x1="276.86" y1="182.88" x2="276.86" y2="177.8" width="0.1524" layer="94"/>
<wire x1="276.86" y1="177.8" x2="271.78" y2="180.34" width="0.1524" layer="94"/>
<wire x1="271.78" y1="180.34" x2="266.7" y2="182.88" width="0.1524" layer="94"/>
<wire x1="271.78" y1="180.34" x2="271.78" y2="182.88" width="0.1524" layer="94"/>
<wire x1="271.78" y1="180.34" x2="271.78" y2="177.8" width="0.1524" layer="94"/>
<wire x1="294.64" y1="182.88" x2="294.64" y2="177.8" width="0.1524" layer="94"/>
<wire x1="294.64" y1="177.8" x2="299.72" y2="180.34" width="0.1524" layer="94"/>
<wire x1="299.72" y1="180.34" x2="304.8" y2="182.88" width="0.1524" layer="94"/>
<wire x1="304.8" y1="182.88" x2="304.8" y2="177.8" width="0.1524" layer="94"/>
<wire x1="304.8" y1="177.8" x2="299.72" y2="180.34" width="0.1524" layer="94"/>
<wire x1="299.72" y1="180.34" x2="294.64" y2="182.88" width="0.1524" layer="94"/>
<wire x1="299.72" y1="180.34" x2="299.72" y2="182.88" width="0.1524" layer="94"/>
<wire x1="299.72" y1="180.34" x2="299.72" y2="177.8" width="0.1524" layer="94"/>
<wire x1="322.58" y1="182.88" x2="322.58" y2="177.8" width="0.1524" layer="94"/>
<wire x1="322.58" y1="177.8" x2="327.66" y2="180.34" width="0.1524" layer="94"/>
<wire x1="327.66" y1="180.34" x2="332.74" y2="182.88" width="0.1524" layer="94"/>
<wire x1="332.74" y1="182.88" x2="332.74" y2="177.8" width="0.1524" layer="94"/>
<wire x1="332.74" y1="177.8" x2="327.66" y2="180.34" width="0.1524" layer="94"/>
<wire x1="327.66" y1="180.34" x2="322.58" y2="182.88" width="0.1524" layer="94"/>
<wire x1="327.66" y1="180.34" x2="327.66" y2="182.88" width="0.1524" layer="94"/>
<wire x1="327.66" y1="180.34" x2="327.66" y2="177.8" width="0.1524" layer="94"/>
<wire x1="259.08" y1="185.42" x2="256.54" y2="185.42" width="0.1524" layer="94"/>
<wire x1="256.54" y1="185.42" x2="256.54" y2="175.26" width="0.1524" layer="94"/>
<wire x1="256.54" y1="175.26" x2="259.08" y2="175.26" width="0.1524" layer="94"/>
<wire x1="259.08" y1="175.26" x2="261.62" y2="175.26" width="0.1524" layer="94"/>
<wire x1="261.62" y1="175.26" x2="261.62" y2="185.42" width="0.1524" layer="94"/>
<wire x1="261.62" y1="185.42" x2="259.08" y2="185.42" width="0.1524" layer="94"/>
<wire x1="259.08" y1="185.42" x2="259.08" y2="187.96" width="0.1524" layer="94"/>
<wire x1="259.08" y1="175.26" x2="259.08" y2="172.72" width="0.1524" layer="94"/>
<text x="255.27" y="174.752" size="1.27" layer="94" rot="R90">RC-Schrubber</text>
<wire x1="287.02" y1="185.42" x2="284.48" y2="185.42" width="0.1524" layer="94"/>
<wire x1="284.48" y1="185.42" x2="284.48" y2="175.26" width="0.1524" layer="94"/>
<wire x1="284.48" y1="175.26" x2="287.02" y2="175.26" width="0.1524" layer="94"/>
<wire x1="287.02" y1="175.26" x2="289.56" y2="175.26" width="0.1524" layer="94"/>
<wire x1="289.56" y1="175.26" x2="289.56" y2="185.42" width="0.1524" layer="94"/>
<wire x1="289.56" y1="185.42" x2="287.02" y2="185.42" width="0.1524" layer="94"/>
<wire x1="287.02" y1="185.42" x2="287.02" y2="187.96" width="0.1524" layer="94"/>
<wire x1="287.02" y1="175.26" x2="287.02" y2="172.72" width="0.1524" layer="94"/>
<text x="283.21" y="174.752" size="1.27" layer="94" rot="R90">RC-Schrubber</text>
<wire x1="314.96" y1="185.42" x2="312.42" y2="185.42" width="0.1524" layer="94"/>
<wire x1="312.42" y1="185.42" x2="312.42" y2="175.26" width="0.1524" layer="94"/>
<wire x1="312.42" y1="175.26" x2="314.96" y2="175.26" width="0.1524" layer="94"/>
<wire x1="314.96" y1="175.26" x2="317.5" y2="175.26" width="0.1524" layer="94"/>
<wire x1="317.5" y1="175.26" x2="317.5" y2="185.42" width="0.1524" layer="94"/>
<wire x1="317.5" y1="185.42" x2="314.96" y2="185.42" width="0.1524" layer="94"/>
<wire x1="314.96" y1="185.42" x2="314.96" y2="187.96" width="0.1524" layer="94"/>
<wire x1="314.96" y1="175.26" x2="314.96" y2="172.72" width="0.1524" layer="94"/>
<text x="311.15" y="174.752" size="1.27" layer="94" rot="R90">RC-Schrubber</text>
<wire x1="284.48" y1="160.02" x2="284.48" y2="147.32" width="0.1524" layer="94"/>
<wire x1="284.48" y1="147.32" x2="294.64" y2="147.32" width="0.1524" layer="94"/>
<wire x1="294.64" y1="147.32" x2="294.64" y2="160.02" width="0.1524" layer="94"/>
<wire x1="294.64" y1="160.02" x2="284.48" y2="160.02" width="0.1524" layer="94"/>
<text x="265.684" y="184.15" size="1.27" layer="95">V1-1</text>
<text x="293.878" y="184.404" size="1.27" layer="95">V1-2</text>
<text x="321.818" y="184.658" size="1.27" layer="95">V1-3</text>
<text x="284.48" y="144.78" size="1.27" layer="95">S1-1</text>
<text x="290.576" y="150.876" size="1.27" layer="94" rot="R90">Soil
Moisture</text>
<wire x1="241.3" y1="205.74" x2="241.3" y2="142.24" width="0.1524" layer="94"/>
<wire x1="241.3" y1="142.24" x2="337.82" y2="142.24" width="0.1524" layer="94"/>
<wire x1="337.82" y1="142.24" x2="337.82" y2="205.74" width="0.1524" layer="94"/>
<wire x1="337.82" y1="205.74" x2="241.3" y2="205.74" width="0.1524" layer="94"/>
<text x="243.84" y="206.502" size="2.54" layer="94">Ventilbox 1</text>
<text x="226.06" y="167.64" size="2.1844" layer="95">K2:BL</text>
<text x="226.06" y="200.66" size="2.1844" layer="95">K2:WH</text>
<text x="226.06" y="165.1" size="2.1844" layer="95">K2:TQ</text>
<text x="226.06" y="162.56" size="2.1844" layer="95">K2:VT</text>
<text x="226.06" y="157.48" size="2.1844" layer="95">K2:WH2</text>
<text x="226.06" y="154.94" size="2.1844" layer="95">K2:OR</text>
<text x="226.06" y="152.4" size="2.1844" layer="95">K2:TQ2</text>
<wire x1="266.7" y1="99.06" x2="266.7" y2="93.98" width="0.1524" layer="94"/>
<wire x1="266.7" y1="93.98" x2="271.78" y2="96.52" width="0.1524" layer="94"/>
<wire x1="271.78" y1="96.52" x2="276.86" y2="99.06" width="0.1524" layer="94"/>
<wire x1="276.86" y1="99.06" x2="276.86" y2="93.98" width="0.1524" layer="94"/>
<wire x1="276.86" y1="93.98" x2="271.78" y2="96.52" width="0.1524" layer="94"/>
<wire x1="271.78" y1="96.52" x2="266.7" y2="99.06" width="0.1524" layer="94"/>
<wire x1="271.78" y1="96.52" x2="271.78" y2="99.06" width="0.1524" layer="94"/>
<wire x1="271.78" y1="96.52" x2="271.78" y2="93.98" width="0.1524" layer="94"/>
<wire x1="294.64" y1="99.06" x2="294.64" y2="93.98" width="0.1524" layer="94"/>
<wire x1="294.64" y1="93.98" x2="299.72" y2="96.52" width="0.1524" layer="94"/>
<wire x1="299.72" y1="96.52" x2="304.8" y2="99.06" width="0.1524" layer="94"/>
<wire x1="304.8" y1="99.06" x2="304.8" y2="93.98" width="0.1524" layer="94"/>
<wire x1="304.8" y1="93.98" x2="299.72" y2="96.52" width="0.1524" layer="94"/>
<wire x1="299.72" y1="96.52" x2="294.64" y2="99.06" width="0.1524" layer="94"/>
<wire x1="299.72" y1="96.52" x2="299.72" y2="99.06" width="0.1524" layer="94"/>
<wire x1="299.72" y1="96.52" x2="299.72" y2="93.98" width="0.1524" layer="94"/>
<wire x1="322.58" y1="99.06" x2="322.58" y2="93.98" width="0.1524" layer="94"/>
<wire x1="322.58" y1="93.98" x2="327.66" y2="96.52" width="0.1524" layer="94"/>
<wire x1="327.66" y1="96.52" x2="332.74" y2="99.06" width="0.1524" layer="94"/>
<wire x1="332.74" y1="99.06" x2="332.74" y2="93.98" width="0.1524" layer="94"/>
<wire x1="332.74" y1="93.98" x2="327.66" y2="96.52" width="0.1524" layer="94"/>
<wire x1="327.66" y1="96.52" x2="322.58" y2="99.06" width="0.1524" layer="94"/>
<wire x1="327.66" y1="96.52" x2="327.66" y2="99.06" width="0.1524" layer="94"/>
<wire x1="327.66" y1="96.52" x2="327.66" y2="93.98" width="0.1524" layer="94"/>
<wire x1="259.08" y1="101.6" x2="256.54" y2="101.6" width="0.1524" layer="94"/>
<wire x1="256.54" y1="101.6" x2="256.54" y2="91.44" width="0.1524" layer="94"/>
<wire x1="256.54" y1="91.44" x2="259.08" y2="91.44" width="0.1524" layer="94"/>
<wire x1="259.08" y1="91.44" x2="261.62" y2="91.44" width="0.1524" layer="94"/>
<wire x1="261.62" y1="91.44" x2="261.62" y2="101.6" width="0.1524" layer="94"/>
<wire x1="261.62" y1="101.6" x2="259.08" y2="101.6" width="0.1524" layer="94"/>
<wire x1="259.08" y1="101.6" x2="259.08" y2="104.14" width="0.1524" layer="94"/>
<wire x1="259.08" y1="91.44" x2="259.08" y2="88.9" width="0.1524" layer="94"/>
<text x="255.27" y="90.932" size="1.27" layer="94" rot="R90">RC-Schrubber</text>
<wire x1="287.02" y1="101.6" x2="284.48" y2="101.6" width="0.1524" layer="94"/>
<wire x1="284.48" y1="101.6" x2="284.48" y2="91.44" width="0.1524" layer="94"/>
<wire x1="284.48" y1="91.44" x2="287.02" y2="91.44" width="0.1524" layer="94"/>
<wire x1="287.02" y1="91.44" x2="289.56" y2="91.44" width="0.1524" layer="94"/>
<wire x1="289.56" y1="91.44" x2="289.56" y2="101.6" width="0.1524" layer="94"/>
<wire x1="289.56" y1="101.6" x2="287.02" y2="101.6" width="0.1524" layer="94"/>
<wire x1="287.02" y1="101.6" x2="287.02" y2="104.14" width="0.1524" layer="94"/>
<wire x1="287.02" y1="91.44" x2="287.02" y2="88.9" width="0.1524" layer="94"/>
<text x="283.21" y="90.932" size="1.27" layer="94" rot="R90">RC-Schrubber</text>
<wire x1="314.96" y1="101.6" x2="312.42" y2="101.6" width="0.1524" layer="94"/>
<wire x1="312.42" y1="101.6" x2="312.42" y2="91.44" width="0.1524" layer="94"/>
<wire x1="312.42" y1="91.44" x2="314.96" y2="91.44" width="0.1524" layer="94"/>
<wire x1="314.96" y1="91.44" x2="317.5" y2="91.44" width="0.1524" layer="94"/>
<wire x1="317.5" y1="91.44" x2="317.5" y2="101.6" width="0.1524" layer="94"/>
<wire x1="317.5" y1="101.6" x2="314.96" y2="101.6" width="0.1524" layer="94"/>
<wire x1="314.96" y1="101.6" x2="314.96" y2="104.14" width="0.1524" layer="94"/>
<wire x1="314.96" y1="91.44" x2="314.96" y2="88.9" width="0.1524" layer="94"/>
<text x="311.15" y="90.932" size="1.27" layer="94" rot="R90">RC-Schrubber</text>
<wire x1="284.48" y1="76.2" x2="284.48" y2="63.5" width="0.1524" layer="94"/>
<wire x1="284.48" y1="63.5" x2="294.64" y2="63.5" width="0.1524" layer="94"/>
<wire x1="294.64" y1="63.5" x2="294.64" y2="76.2" width="0.1524" layer="94"/>
<wire x1="294.64" y1="76.2" x2="284.48" y2="76.2" width="0.1524" layer="94"/>
<text x="265.684" y="100.33" size="1.27" layer="95">V2-1</text>
<text x="293.878" y="100.584" size="1.27" layer="95">V2-2</text>
<text x="321.818" y="100.838" size="1.27" layer="95">V2-3</text>
<text x="284.48" y="60.96" size="1.27" layer="95">S2-1</text>
<text x="290.576" y="67.056" size="1.27" layer="94" rot="R90">Soil
Moisture</text>
<wire x1="241.3" y1="121.92" x2="241.3" y2="58.42" width="0.1524" layer="94"/>
<wire x1="241.3" y1="58.42" x2="337.82" y2="58.42" width="0.1524" layer="94"/>
<wire x1="337.82" y1="58.42" x2="337.82" y2="121.92" width="0.1524" layer="94"/>
<wire x1="337.82" y1="121.92" x2="241.3" y2="121.92" width="0.1524" layer="94"/>
<text x="243.84" y="122.682" size="2.54" layer="94">Ventilbox 2</text>
<text x="226.06" y="83.82" size="2.1844" layer="95">K3:BL</text>
<text x="226.06" y="116.84" size="2.1844" layer="95">K3:WH</text>
<text x="226.06" y="81.28" size="2.1844" layer="95">K3:TQ</text>
<text x="226.06" y="78.74" size="2.1844" layer="95">K3:VT</text>
<text x="226.06" y="73.66" size="2.1844" layer="95">K3:WH2</text>
<text x="226.06" y="71.12" size="2.1844" layer="95">K3:OR</text>
<text x="226.06" y="68.58" size="2.1844" layer="95">K1:TQ2</text>
<text x="45.72" y="93.98" size="2.54" layer="95" rot="R90">24VAC</text>
</plain>
<instances>
<instance part="FRAME1" gate="G$1" x="0" y="0" smashed="yes">
<attribute name="DRAWING_NAME" x="344.17" y="15.24" size="2.54" layer="94"/>
<attribute name="LAST_DATE_TIME" x="344.17" y="10.16" size="2.286" layer="94"/>
<attribute name="SHEET" x="357.505" y="5.08" size="2.54" layer="94"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<wire x1="43.18" y1="187.96" x2="43.18" y2="124.46" width="0.1524" layer="91"/>
<wire x1="43.18" y1="124.46" x2="43.18" y2="106.68" width="0.1524" layer="91"/>
<wire x1="43.18" y1="124.46" x2="96.52" y2="124.46" width="0.1524" layer="91"/>
<junction x="43.18" y="124.46"/>
<wire x1="96.52" y1="124.46" x2="106.68" y2="124.46" width="0.1524" layer="91"/>
<wire x1="106.68" y1="124.46" x2="116.84" y2="124.46" width="0.1524" layer="91"/>
<wire x1="116.84" y1="124.46" x2="127" y2="124.46" width="0.1524" layer="91"/>
<wire x1="127" y1="124.46" x2="137.16" y2="124.46" width="0.1524" layer="91"/>
<wire x1="137.16" y1="124.46" x2="147.32" y2="124.46" width="0.1524" layer="91"/>
<wire x1="147.32" y1="124.46" x2="147.32" y2="129.54" width="0.1524" layer="91"/>
<wire x1="137.16" y1="124.46" x2="137.16" y2="129.54" width="0.1524" layer="91"/>
<junction x="137.16" y="124.46"/>
<wire x1="127" y1="124.46" x2="127" y2="129.54" width="0.1524" layer="91"/>
<junction x="127" y="124.46"/>
<wire x1="116.84" y1="124.46" x2="116.84" y2="129.54" width="0.1524" layer="91"/>
<junction x="116.84" y="124.46"/>
<wire x1="106.68" y1="124.46" x2="106.68" y2="129.54" width="0.1524" layer="91"/>
<junction x="106.68" y="124.46"/>
<wire x1="96.52" y1="124.46" x2="96.52" y2="129.54" width="0.1524" layer="91"/>
<junction x="96.52" y="124.46"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<wire x1="45.72" y1="187.96" x2="45.72" y2="121.92" width="0.1524" layer="91"/>
<wire x1="45.72" y1="121.92" x2="45.72" y2="106.68" width="0.1524" layer="91"/>
<wire x1="45.72" y1="121.92" x2="91.44" y2="121.92" width="0.1524" layer="91"/>
<junction x="45.72" y="121.92"/>
<wire x1="91.44" y1="121.92" x2="91.44" y2="91.44" width="0.1524" layer="91"/>
<wire x1="91.44" y1="121.92" x2="121.92" y2="121.92" width="0.1524" layer="91"/>
<wire x1="121.92" y1="121.92" x2="121.92" y2="91.44" width="0.1524" layer="91"/>
<junction x="91.44" y="121.92"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<wire x1="48.26" y1="187.96" x2="48.26" y2="177.8" width="0.1524" layer="91"/>
<wire x1="48.26" y1="177.8" x2="99.06" y2="177.8" width="0.1524" layer="91"/>
<wire x1="99.06" y1="177.8" x2="99.06" y2="172.72" width="0.1524" layer="91"/>
<wire x1="99.06" y1="177.8" x2="182.88" y2="177.8" width="0.1524" layer="91"/>
<wire x1="182.88" y1="177.8" x2="182.88" y2="119.38" width="0.1524" layer="91"/>
<junction x="99.06" y="177.8"/>
<wire x1="157.48" y1="91.44" x2="157.48" y2="119.38" width="0.1524" layer="91"/>
<wire x1="157.48" y1="119.38" x2="167.64" y2="119.38" width="0.1524" layer="91"/>
<wire x1="167.64" y1="119.38" x2="182.88" y2="119.38" width="0.1524" layer="91"/>
<wire x1="167.64" y1="91.44" x2="167.64" y2="119.38" width="0.1524" layer="91"/>
<junction x="167.64" y="119.38"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<wire x1="50.8" y1="187.96" x2="50.8" y2="180.34" width="0.1524" layer="91"/>
<wire x1="50.8" y1="180.34" x2="96.52" y2="180.34" width="0.1524" layer="91"/>
<wire x1="96.52" y1="180.34" x2="96.52" y2="172.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<wire x1="101.6" y1="172.72" x2="101.6" y2="180.34" width="0.1524" layer="91"/>
<wire x1="101.6" y1="180.34" x2="180.34" y2="180.34" width="0.1524" layer="91"/>
<wire x1="180.34" y1="180.34" x2="180.34" y2="124.46" width="0.1524" layer="91"/>
<wire x1="180.34" y1="124.46" x2="167.64" y2="124.46" width="0.1524" layer="91"/>
<wire x1="167.64" y1="124.46" x2="157.48" y2="124.46" width="0.1524" layer="91"/>
<wire x1="157.48" y1="124.46" x2="157.48" y2="129.54" width="0.1524" layer="91"/>
<wire x1="167.64" y1="124.46" x2="167.64" y2="129.54" width="0.1524" layer="91"/>
<junction x="167.64" y="124.46"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<wire x1="93.98" y1="129.54" x2="93.98" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<wire x1="104.14" y1="129.54" x2="104.14" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<wire x1="114.3" y1="129.54" x2="114.3" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<wire x1="124.46" y1="129.54" x2="124.46" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<wire x1="134.62" y1="129.54" x2="134.62" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<wire x1="144.78" y1="129.54" x2="144.78" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<wire x1="154.94" y1="129.54" x2="154.94" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<wire x1="165.1" y1="129.54" x2="165.1" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<wire x1="175.26" y1="160.02" x2="185.42" y2="160.02" width="0.1524" layer="91"/>
<wire x1="185.42" y1="160.02" x2="185.42" y2="116.84" width="0.1524" layer="91"/>
<wire x1="185.42" y1="116.84" x2="160.02" y2="116.84" width="0.1524" layer="91"/>
<wire x1="160.02" y1="116.84" x2="160.02" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<wire x1="170.18" y1="114.3" x2="170.18" y2="91.44" width="0.1524" layer="91"/>
<wire x1="170.18" y1="114.3" x2="187.96" y2="114.3" width="0.1524" layer="91"/>
<wire x1="187.96" y1="114.3" x2="187.96" y2="157.48" width="0.1524" layer="91"/>
<wire x1="187.96" y1="157.48" x2="175.26" y2="157.48" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<wire x1="271.78" y1="182.88" x2="271.78" y2="187.96" width="0.1524" layer="91"/>
<wire x1="271.78" y1="187.96" x2="271.78" y2="200.66" width="0.1524" layer="91"/>
<wire x1="271.78" y1="200.66" x2="236.22" y2="200.66" width="0.1524" layer="91"/>
<wire x1="259.08" y1="187.96" x2="271.78" y2="187.96" width="0.1524" layer="91"/>
<junction x="271.78" y="187.96"/>
<wire x1="271.78" y1="200.66" x2="299.72" y2="200.66" width="0.1524" layer="91"/>
<junction x="271.78" y="200.66"/>
<wire x1="299.72" y1="200.66" x2="327.66" y2="200.66" width="0.1524" layer="91"/>
<wire x1="299.72" y1="182.88" x2="299.72" y2="187.96" width="0.1524" layer="91"/>
<junction x="299.72" y="200.66"/>
<wire x1="299.72" y1="187.96" x2="299.72" y2="200.66" width="0.1524" layer="91"/>
<wire x1="327.66" y1="182.88" x2="327.66" y2="187.96" width="0.1524" layer="91"/>
<wire x1="327.66" y1="187.96" x2="327.66" y2="200.66" width="0.1524" layer="91"/>
<wire x1="287.02" y1="187.96" x2="299.72" y2="187.96" width="0.1524" layer="91"/>
<junction x="299.72" y="187.96"/>
<wire x1="314.96" y1="187.96" x2="327.66" y2="187.96" width="0.1524" layer="91"/>
<junction x="327.66" y="187.96"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<wire x1="271.78" y1="177.8" x2="271.78" y2="172.72" width="0.1524" layer="91"/>
<wire x1="271.78" y1="172.72" x2="271.78" y2="167.64" width="0.1524" layer="91"/>
<wire x1="271.78" y1="167.64" x2="233.68" y2="167.64" width="0.1524" layer="91"/>
<wire x1="259.08" y1="172.72" x2="271.78" y2="172.72" width="0.1524" layer="91"/>
<junction x="271.78" y="172.72"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<wire x1="233.68" y1="165.1" x2="299.72" y2="165.1" width="0.1524" layer="91"/>
<wire x1="299.72" y1="165.1" x2="299.72" y2="172.72" width="0.1524" layer="91"/>
<wire x1="299.72" y1="172.72" x2="299.72" y2="177.8" width="0.1524" layer="91"/>
<wire x1="287.02" y1="172.72" x2="299.72" y2="172.72" width="0.1524" layer="91"/>
<junction x="299.72" y="172.72"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<wire x1="233.68" y1="162.56" x2="327.66" y2="162.56" width="0.1524" layer="91"/>
<wire x1="327.66" y1="162.56" x2="327.66" y2="172.72" width="0.1524" layer="91"/>
<wire x1="327.66" y1="172.72" x2="327.66" y2="177.8" width="0.1524" layer="91"/>
<wire x1="314.96" y1="172.72" x2="327.66" y2="172.72" width="0.1524" layer="91"/>
<junction x="327.66" y="172.72"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<wire x1="233.68" y1="157.48" x2="284.48" y2="157.48" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$21" class="0">
<segment>
<wire x1="233.68" y1="154.94" x2="284.48" y2="154.94" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$22" class="0">
<segment>
<wire x1="233.68" y1="152.4" x2="284.48" y2="152.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$23" class="0">
<segment>
<wire x1="271.78" y1="99.06" x2="271.78" y2="104.14" width="0.1524" layer="91"/>
<wire x1="271.78" y1="104.14" x2="271.78" y2="116.84" width="0.1524" layer="91"/>
<wire x1="271.78" y1="116.84" x2="236.22" y2="116.84" width="0.1524" layer="91"/>
<wire x1="259.08" y1="104.14" x2="271.78" y2="104.14" width="0.1524" layer="91"/>
<junction x="271.78" y="104.14"/>
<wire x1="271.78" y1="116.84" x2="299.72" y2="116.84" width="0.1524" layer="91"/>
<junction x="271.78" y="116.84"/>
<wire x1="299.72" y1="116.84" x2="327.66" y2="116.84" width="0.1524" layer="91"/>
<wire x1="299.72" y1="99.06" x2="299.72" y2="104.14" width="0.1524" layer="91"/>
<junction x="299.72" y="116.84"/>
<wire x1="299.72" y1="104.14" x2="299.72" y2="116.84" width="0.1524" layer="91"/>
<wire x1="327.66" y1="99.06" x2="327.66" y2="104.14" width="0.1524" layer="91"/>
<wire x1="327.66" y1="104.14" x2="327.66" y2="116.84" width="0.1524" layer="91"/>
<wire x1="287.02" y1="104.14" x2="299.72" y2="104.14" width="0.1524" layer="91"/>
<junction x="299.72" y="104.14"/>
<wire x1="314.96" y1="104.14" x2="327.66" y2="104.14" width="0.1524" layer="91"/>
<junction x="327.66" y="104.14"/>
</segment>
</net>
<net name="N$24" class="0">
<segment>
<wire x1="271.78" y1="93.98" x2="271.78" y2="88.9" width="0.1524" layer="91"/>
<wire x1="271.78" y1="88.9" x2="271.78" y2="83.82" width="0.1524" layer="91"/>
<wire x1="271.78" y1="83.82" x2="233.68" y2="83.82" width="0.1524" layer="91"/>
<wire x1="259.08" y1="88.9" x2="271.78" y2="88.9" width="0.1524" layer="91"/>
<junction x="271.78" y="88.9"/>
</segment>
</net>
<net name="N$25" class="0">
<segment>
<wire x1="233.68" y1="81.28" x2="299.72" y2="81.28" width="0.1524" layer="91"/>
<wire x1="299.72" y1="81.28" x2="299.72" y2="88.9" width="0.1524" layer="91"/>
<wire x1="299.72" y1="88.9" x2="299.72" y2="93.98" width="0.1524" layer="91"/>
<wire x1="287.02" y1="88.9" x2="299.72" y2="88.9" width="0.1524" layer="91"/>
<junction x="299.72" y="88.9"/>
</segment>
</net>
<net name="N$26" class="0">
<segment>
<wire x1="233.68" y1="78.74" x2="327.66" y2="78.74" width="0.1524" layer="91"/>
<wire x1="327.66" y1="78.74" x2="327.66" y2="88.9" width="0.1524" layer="91"/>
<wire x1="327.66" y1="88.9" x2="327.66" y2="93.98" width="0.1524" layer="91"/>
<wire x1="314.96" y1="88.9" x2="327.66" y2="88.9" width="0.1524" layer="91"/>
<junction x="327.66" y="88.9"/>
</segment>
</net>
<net name="N$27" class="0">
<segment>
<wire x1="233.68" y1="73.66" x2="284.48" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$28" class="0">
<segment>
<wire x1="233.68" y1="71.12" x2="284.48" y2="71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$29" class="0">
<segment>
<wire x1="233.68" y1="68.58" x2="284.48" y2="68.58" width="0.1524" layer="91"/>
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
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
