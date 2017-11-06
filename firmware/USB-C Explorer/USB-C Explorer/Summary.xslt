<?xml version="1.0" encoding="utf-8"?>
<!-- Edited by XMLSpy® -->
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" >
  <xsl:template match="/">
    <html>
      <head>
        <style type="text/css">

          .header
          {
          height: 30px;
          MARGIN-TOP: 10px;
          BACKGROUND-COLOR: #cecf9c;
          BORDER-BOTTOM: #ffffff 1px solid;
          BORDER-LEFT: #ffffff 1px solid;
          BORDER-RIGHT: #ffffff 1px solid;
          BORDER-TOP: #ffffff 1px solid;
          COLOR: #000000;
          FONT-FAMILY: "Verdana", sans-serif;
          FONT-WEIGHT: bold;
          FONT-SIZE: 80%;
          }
          .infotable
          {
          MARGIN-TOP: 20px;
          TEXT-ALIGN: LEFT;
          BACKGROUND-COLOR:  #f0f0e0;
          BORDER-BOTTOM: #ffffff 0px solid;
          BORDER-COLLAPSE: collapse;
          BORDER-LEFT: #ffffff 0px solid;
          BORDER-RIGHT: #ffffff 0px solid;
          BORDER-TOP: #ffffff 0px solid;
          FONT-FAMILY: "Verdana", sans-serif;
          FONT-SIZE: 70%;
          }


          #color1
          {

          BACKGROUND-COLOR: #ffffff;
          }

          #color2
          {

          BACKGROUND-COLOR:#f8f8ff ;
          }

        </style>
      </head>
      <body>
        <table cellspacing="5" width="50%" marign-left="100px" border="1" bordercolor="white" class="infotable">
          <col width="50%" />
          <col width="50%" />
          <tr>
            <td nowrap="1" class ="header">Device Information</td>
            <td nowrap="1" class ="header"></td>
          </tr>

          <tr id="color1">
            <td>
              Device Name
            </td>
            <td >
              <xsl:value-of select="SUMMARY/DeviceInformation/DeviceName"/>
            </td>
          </tr>
          <tr id="color2">
            <td >
              Device Variant
            </td>
            <td>
              <xsl:value-of select="SUMMARY/DeviceInformation/DeviceVariant"/>
            </td>
          </tr>

          <tr id="color1">
            <td >
              Technology
            </td>
            <td>
              <xsl:value-of select="SUMMARY/DeviceInformation/Technology"/>
            </td>
          </tr>
          <tr>
            <td nowrap="1" class ="header">Sensor Information</td>
            <td nowrap="1" class ="header"></td>
          </tr>

          <tr id="color1">
            <td>
              Number Of Buttons
            </td>
            <td >
              <xsl:value-of select="SUMMARY/SensorInformation/NumberOfButtons"/>
            </td>
          </tr>
          <tr id="color2">
            <td >
              Number Of Wheels
            </td>
            <td>
              <xsl:value-of select="SUMMARY/SensorInformation/NumberOfWheels"/>
            </td>
          </tr>

          <tr id="color1">
            <td >
              Number Of Sliders
            </td>
            <td>
              <xsl:value-of select="SUMMARY/SensorInformation/NumberOfSliders"/>
            </td>
          </tr>
          <tr>
            <td nowrap="1" class ="header">Channel Information</td>
            <td nowrap="1" class ="header"></td>
          </tr>
          <tr id="color1">
            <td >
              Total Channels Consumed
            </td>
            <td>
              <xsl:value-of select="SUMMARY/ChannelInformation/TotalChannelsConsumed"/>
            </td>
          </tr>
          <tr>
            <td nowrap="1" class ="header">Pin Configuration</td>
            <td nowrap="1" class ="header"></td>
          </tr>
          <tr id="color1">
            <td >
              Available Ports
            </td>
            <td>
              <xsl:value-of select="SUMMARY/PinConfiguration/AvailablePorts"/>
            </td>
          </tr>
          <tr id="color2">
            <td >
              Total Pins Used
            </td>
            <td>
              <xsl:value-of select="SUMMARY/PinConfiguration/TotalPinsUsed"/>
            </td>
          </tr>        
        </table>

        <table cellspacing="5" width="100%" border="1" bordercolor="white" class="infotable">
          <col width="20%" />
          <col width="20%" />
          <col width="20%" />
          <col width="20%" />
          <col width="20%" />

          <tr>
            <td nowrap="1" class ="header">Channel No</td>
            <td nowrap="1" class ="header">SeriesResistor</td>
            <td nowrap="1" class ="header">Prescalar</td>
            <td nowrap="1" class ="header">AutoOS</td>
            <td nowrap="1" class ="header">FilterLevel</td>
          </tr>

          <xsl:for-each select="//SUMMARY/ChannelInformationDetailed/Channel">

            <tr id="color1">
              <td>
                <xsl:value-of select="@No"/>
              </td>
              <td>
                <xsl:value-of select="@SeriesResistor"/>
              </td>
              <td>
                <xsl:value-of select="@Prescalar"/>
              </td>
              <td>
                <xsl:value-of select="@AutoOS"/>
              </td>
              <td>
                <xsl:value-of select="@FilterLevel"/>
              </td>
            </tr>
          </xsl:for-each>
        </table>
        
        <table cellspacing="5" width="100%" border="1" bordercolor="white" class="infotable">
          <col width="17%" />
          <col width="17%" />
          <col width="17%" />
          <col width="17%" />
          <col width="17%" />
          <col width="15%" />
          <tr>
            <td nowrap="1" class ="header">Name of the sensor</td>
            <td nowrap="1" class ="header">Channel Number</td>
            <td nowrap="1" class ="header">SensingPair</td>
            <td nowrap="1" class ="header">PortPin</td>
          </tr>
          <xsl:for-each select="//SUMMARY/PinConfiguration/Sensors/Sensor">
            <xsl:choose>
              <xsl:when test="(position() mod 2) = 0">
                <tr id="color1">
                  <td>
                    <xsl:value-of select="@Name"/>
                  </td>
                  <td>
                    <xsl:value-of select="@ChannelNumber"/>
                  </td>
                  <td>
                    <xsl:value-of select="@SensingPair"/>
                  </td>
                  <td>
                    <xsl:value-of select="@PortPin"/>
                  </td>
                </tr>
              </xsl:when>
              <xsl:otherwise>
                <tr id="color2">
                  <td>
                    <xsl:value-of select="@Name"/>
                  </td>
                  <td>
                    <xsl:value-of select="@ChannelNumber"/>
                  </td>
                  <td>
                    <xsl:value-of select="@SensingPair"/>
                  </td>
                  <td>
                    <xsl:value-of select="@PortPin"/>
                  </td>
                </tr>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </table>

        <xsl:if test="//SUMMARY/MoistureGrouping//@IsEnabled='true'">
          <table cellspacing="5" width="100%" border="1" bordercolor="white" class="infotable">
            <col width="50%" />
            <col width="50%" />
            <tr>
              <td nowrap="1" class ="header">Sensor</td>
              <td nowrap="1" class ="header">Moisture Group</td>
            </tr>
            <xsl:for-each select="//SUMMARY/MoistureGrouping/MoistSensors/MoistSensor">
              <xsl:choose>
                <xsl:when test="(position() mod 2) = 0">
                  <tr id="color1">
                    <td>
                      <xsl:value-of select="@SensorName"/>
                    </td>
                    <td>
                      <xsl:value-of select="@MoistGroup"/>
                    </td>
                  </tr>
                </xsl:when>
                <xsl:otherwise>
                  <tr id="color2">
                    <td>
                      <xsl:value-of select="@SensorName"/>
                    </td>
                    <td>
                      <xsl:value-of select="@MoistGroup"/>
                    </td>
                  </tr>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
          </table>
        </xsl:if>

        <xsl:if test="//SUMMARY/SensorMoistureGroupDetail//@IsEnabled='true'">
          <table cellspacing="5" width="100%" border="1" bordercolor="white" class="infotable">
            <col width="33%" />
            <col width="33%" />
            <col width="34%" />
            <tr>
              <td nowrap="1" class ="header">Moisture Group</td>
              <td nowrap="1" class ="header">Sensor Moisture Lock Threshold</td>
              <td nowrap="1" class ="header">System Moisture Lock Threshold</td>
            </tr>
            <xsl:for-each select="//SUMMARY/SensorMoistureGroupDetail/MoistGroupDetail/MoistGroupInfo">
              <xsl:choose>
                <xsl:when test="(position() mod 2) = 0">
                  <tr id="color1">
                    <td>
                      <xsl:value-of select="@MoistGroup"/>
                    </td>
                    <td>
                      <xsl:value-of select="@SensorMoistureLockThreshold"/>
                    </td>
                    <td>
                      <xsl:value-of select="@SystemMoistureLockThreshold"/>
                    </td>
                  </tr>
                </xsl:when>
                <xsl:otherwise>
                  <tr id="color2">
                    <td>
                      <xsl:value-of select="@MoistGroup"/>
                    </td>
                    <td>
                      <xsl:value-of select="@SensorMoistureLockThreshold"/>
                    </td>
                    <td>
                      <xsl:value-of select="@SystemMoistureLockThreshold"/>
                    </td>
                  </tr>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
          </table>
        </xsl:if>
        <xsl:if test="//SUMMARY/PinConfiguration/DebugInterface/@Enabled='True'">
          <table cellspacing="5" width="100%" border="1" bordercolor="white" class="infotable">
            <col width="100" />
            <col width="100" />
            <tr>
              <td nowrap="1" class ="header">
                Debug Interface- <xsl:value-of select="//SUMMARY/PinConfiguration/DebugInterface/@Type"/>
              </td>
              <td nowrap="1" class ="header">Port Pin</td>
            </tr>
            <xsl:for-each select="//SUMMARY/PinConfiguration/DebugInterface/Port">
              <xsl:choose>
                <xsl:when test="(position() mod 2) = 0">
                  <tr id="color1">
                    <td>
                      <xsl:value-of select="@Name"/>
                    </td>
                    <td>
                      <xsl:value-of select="@PortPin"/>
                    </td>
                  </tr>
                </xsl:when>
                <xsl:otherwise>
                  <tr id="color2">
                    <td>
                      <xsl:value-of select="@Name"/>
                    </td>
                    <td>
                      <xsl:value-of select="@PortPin"/>
                    </td>
                  </tr>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
            <tr id="color1">
              <td >
                Power Analyzer
              </td>
              <td>
                <xsl:value-of select="SUMMARY/PinConfiguration/DebugInterface/PowerAnalyzer"/>
              </td>
            </tr>
          </table>
        </xsl:if>

        <table cellspacing="5" width="100%" border="1" bordercolor="white" class="infotable">
          <col width="100" />
          <col width="100" />
          <col width="100" />
          <col width="100" />
          <tr>
            <td nowrap="1" class ="header">Memory type</td>
            <td nowrap="1" class ="header">Total</td>
            <td nowrap="1" class ="header">Used</td>
            <td nowrap="1" class ="header">Free</td>
          </tr>
          <br/>
          <tr id="color1">
            <td>
              Data Memory
            </td>
            <td>
              <xsl:value-of select="SUMMARY/MemoryInformation/DataMemory/TotalDataMemory"/>
            </td>
            <td>
              <xsl:value-of select="SUMMARY/MemoryInformation/DataMemory/DataMemoryUsed"/>
            </td>
            <td>
              <xsl:value-of select="SUMMARY/MemoryInformation/DataMemory/DataMemoryFree"/>
            </td>
          </tr>
          <tr id="color2">
            <td>
              Code Memory
            </td>
            <td>
              <xsl:value-of select="SUMMARY/MemoryInformation/CodeMemory/TotalCodeMemory"/>
            </td>
            <td>
              <xsl:value-of select="SUMMARY/MemoryInformation/CodeMemory/CodeMemoryUsed"/>
            </td>
            <td>
              <xsl:value-of select="SUMMARY/MemoryInformation/CodeMemory/CodeMemoryFree"/>
            </td>
          </tr>
        </table>

        <table cellspacing="5" width="100%" border="1" bordercolor="white" class="infotable">
          <col width="100" />
          <col width="100" />
          <tr>
            <td nowrap="1" class ="header">Library Information</td>
            <td nowrap="1" class ="header"></td>
          </tr>
          <tr id="color1">
            <td >
              Tool Chain Name
            </td>
            <td>
              <xsl:value-of select="SUMMARY/LibraryInformation/@ToolChainName"/>
            </td>
          </tr>
          <tr id="color2">
            <td >
              Default Library
            </td>
            <td>
              <xsl:value-of select="SUMMARY/LibraryInformation/DefaultLibrary"/>
            </td>
          </tr>
          <tr>
            <td nowrap="1" class ="header">Other Options</td>
            <td nowrap="1" class ="header"></td>
          </tr>
          <tr id="color1">
            <td >
              Frequency Mode
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/FrequencyMode"/>
            </td>
          </tr>
          <!--<tr id="color2">
            <td >
              Filter Level
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/FilterLevel"/>
            </td>
          </tr>-->
          <!--<tr id="color1">
            <td >
              Auto OS
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/AutoOS"/>
            </td>
          </tr>-->
          <tr id="color1">
            <td >
              Auto Tuning
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/AutoTuning"/>
            </td>
          </tr>
          <!--<tr id="color2">
            <td >
              PreScalar
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/PreScalar"/>
            </td>
          </tr>
          <tr id="color1">
            <td >
              Series Resistor
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/SeriesResistor"/>
            </td>
          </tr>-->
          <tr id="color2">
            <td >
              Frequency Hops
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/FrequencyHops"/>
            </td>
          </tr>
          <tr id="color1">
            <td >
              PTC Interrupt Priority Level
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/PTCInterruptprioritylevel"/>
            </td>
          </tr>
          <xsl:if test="SUMMARY/OtherOptions/SensorPostProcessingMode">
            <tr id="color2">
              <td >
                Sensor Post Processing Mode
              </td>
              <td>
                <xsl:value-of select="SUMMARY/OtherOptions/SensorPostProcessingMode"/>
              </td>
            </tr>
          </xsl:if>
          <xsl:if test="SUMMARY/OtherOptions/QuickReburstEnabled">
            <tr id="color2">
              <td >
                Quick Reburst Enabled Status
              </td>
              <td>
                <xsl:value-of select="SUMMARY/OtherOptions/QuickReburstEnabled"/>
              </td>
            </tr>
          </xsl:if>
          <xsl:if test="SUMMARY/OtherOptions/TouchFMEATest">
            <tr id="color1">
              <td >
                Touch FMEA Test Status
              </td>
              <td>
                <xsl:value-of select="SUMMARY/OtherOptions/TouchFMEATest"/>
              </td>
            </tr>
          </xsl:if>
          <xsl:if test="SUMMARY/OtherOptions/SafetyFMEATimePeriod">
            <tr id="color2">
              <td >
                Safety FMEA Time Period
              </td>
              <td>
                <xsl:value-of select="SUMMARY/OtherOptions/SafetyFMEATimePeriod"/>
              </td>
            </tr>
          </xsl:if>
          <tr>
            <td nowrap="1" class ="header">Frequency Autotune Parameters</td>
            <td nowrap="1" class ="header"></td>
          </tr>
          <tr id="color1">
            <td >
              Frequency Autotune Status
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/FrequencyAutoTuneEnabled"/>
            </td>
          </tr>
          <tr id="color2">
            <td >
              Frequency Autotuning Signal Limit
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/FrequencyAutoTuneSignalLimit"/>
            </td>
          </tr>

          <tr id="color1">
            <td >
              Frequency Autotune in Count
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/FrequencyAutoTuneInCount"/>
            </td>
          </tr>
          <tr>
            <td nowrap="1" class ="header">Noise Measurement Parameters</td>
            <td nowrap="1" class ="header"></td>
          </tr>
          <tr id="color2">
            <td >
              Noise Measurement Status
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/NoiseMeasurementStatus"/>
            </td>
          </tr>

          <tr id="color1">
            <td >
              Noise Measurement Signal Stability Limit
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/NoiseMeasurementSignalStabilityLimit"/>
            </td>
          </tr>
          <tr id="color2">
            <td >
              Noise Measurement Noise Limit
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/NoiseMeasurementNoiseLimit"/>
            </td>
          </tr>

          <tr id="color1">
            <td >
              Noise Measurement Buffer Count
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/NoiseMeasurementNoiseBufferCount"/>
            </td>
          </tr>
          <tr>
            <td nowrap="1" class ="header">Lockout Parameters</td>
            <td nowrap="1" class ="header"></td>
          </tr>

          <tr id="color2">
            <td >
              Lockout Status
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/LockoutStatus"/>
            </td>
          </tr>

          <tr id="color1">
            <td >
              Lockout Countdown
            </td>
            <td>
              <xsl:value-of select="SUMMARY/OtherOptions/LockoutCountdown"/>
            </td>
          </tr>

        </table>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>

